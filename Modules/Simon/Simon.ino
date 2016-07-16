#include <Wire.h>

#define R_BTN_PIN 13
#define G_BTN_PIN 12
#define Y_BTN_PIN 11
#define B_BTN_PIN 10

#define R_LED_PIN 7
#define G_LED_PIN 6
#define Y_LED_PIN 5
#define B_LED_PIN 4

#define SOLVED_1_LED A2
#define SOLVED_2_LED A1
#define SOLVED_3_LED A0

#define SOLVED_ALL_LED 3
#define FAIL_LED       2

#define I2C_ADDR 10 

// keeps track of how many iterations of the permutation still have to be solved
int to_solve;

#define BUSY 0xff // Byte to be send if module is busy and has no answer to provide
// Answer to be send to the controller
byte i2c_answer = BUSY;

byte game_state;

typedef enum {
  GS_STOPPED, // Module is idle and waiting for start of a game
  GS_RUNNING, // The module is currently running
  GS_SOLVED, // Module has been succesfully solved
  GS_FAILED, // A fail has been triggered
  GS_ERROR // Module suffered from internal error
} GameState;

// Defines for possible button colors
#define R 0
#define G 1
#define B 2
#define Y 3

#define PERM(W,X,Y,Z) (((byte)(W)<<6) | ((byte)(X)<<4) | ((byte)(Y)<<2) | ((byte)(Z)))

#define NUM_PERMUTATIONS 4

const int sigma[][NUM_PERMUTATIONS] = 
  {{PERM(R,G,Y,B), PERM(Y,B,G,R), PERM(G,Y,B,R), PERM(B,Y,G,R)},
   {PERM(B,G,Y,R), PERM(R,Y,G,B), PERM(Y,G,B,R), PERM(G,B,R,Y)},
   {PERM(G,Y,B,R), PERM(B,R,G,Y), PERM(R,B,Y,G), PERM(Y,B,R,G)},
   {PERM(Y,R,G,B), PERM(G,R,B,Y), PERM(B,G,R,Y), PERM(R,G,B,Y)}};

const int comp[] = 
  {PERM(0,1,2,3), PERM(1,0,2,3), PERM(2,1,0,3), PERM(3,2,1,0),
   PERM(0,3,1,2), PERM(1,3,0,2), PERM(2,0,1,3), PERM(3,1,0,2),
   PERM(0,2,3,1)};

#define BTN_NONE 42

byte pressed_color = BTN_NONE;

bool input_started; // Has the player started to input the sequence?

// Resets module state into idle mode
void reset() 
{
  digitalWrite(R_LED_PIN, LOW);
  digitalWrite(Y_LED_PIN, LOW);
  digitalWrite(G_LED_PIN, LOW);
  digitalWrite(B_LED_PIN, LOW);
  
  digitalWrite(SOLVED_ALL_LED, LOW);
  digitalWrite(FAIL_LED, LOW);

  to_solve = 0b0;
  update_to_solve();

  pressed_color = BTN_NONE;

  input_started = false;
  
  game_state = GS_STOPPED;
}

// Updates the state of the Solution-indicator leds
void update_to_solve() 
{
  digitalWrite(SOLVED_1_LED, bitRead(to_solve, 0));
  digitalWrite(SOLVED_2_LED, bitRead(to_solve, 1));
  digitalWrite(SOLVED_3_LED, bitRead(to_solve, 2));
}

// Defines for different requests according to the MC-Communication Protocol
#define I2C_RESERVED   0
#define I2C_GAME_START 1
#define I2C_GET_STATE  2
#define I2C_GAME_OVER  3

// Defines for module answers (on any request)
#define ACK  1
#define NACK 0

// Extract the (N-1)th pair of bytes from SEQ (left to right)
#define GET(SEQ,N) ((((unsigned byte)(SEQ)) >> (2*(3 - (N)))) & 0b11)

// Function representing the circle operator (composition of permutations)
byte compose(byte lhs, byte rhs)
{
  byte res = 0, i, substitute;
  for (i = 0; i < 4; i++) res |= (GET(lhs,GET(rhs, i))) << (2*i);
  return res;
}

byte composed_perm = 0b00000000;

#define RNG_LED_NUM 8 // Number of LEDs representing random number on main controller

#define MAX_REPEATS   8 // Maximal length of the sequence
#define START_REPEATS 4 // The length of the first sequence to be entered
#define SEQ_INCREMENT 2 // Number of colors added at the end of the sequence on success

// A sequence of 8 random button presses (R, G, B, Y)
byte sequence[MAX_REPEATS];

byte progress; // progress in sequence (colors already typed)
byte current_max; // current maximum of colors of the sequence to be typed 

int timer; // used to time blinking through sequence
byte progress_in_seq; // saves at which point in the sequence the handle_sequence function is 
bool toggled; // checks if LED is currently on or not (for alternating between on and of)
int wait_time = 0; // Time to be waited after end of sequence

// Starts the timer required for blinking the sequence and initialize timer related code
void start_timer()
{
  timer = millis();
  progress_in_seq = 0;
  toggled = false;
  wait_time = 0;
}

byte init_game(byte random_byte) 
{
  
  // Determine the order in which the permutations are composed
  char i, count = 0;
  for (i = 0; i < RNG_LED_NUM; i++) { count += bitRead(random_byte, i); }
  
  byte order = comp[count]; 

  // Get permutations in the order determined before
  byte perms[NUM_PERMUTATIONS];
  for (i = 0; i < NUM_PERMUTATIONS; i++) perms[i] = sigma[GET(order,i)][GET(random_byte,i)];

  // Calculate the result of composing the chosen permutations in the given order 
  byte res = perms[NUM_PERMUTATIONS - 1];
  for (i = NUM_PERMUTATIONS - 2; i >= 0; i--) res = compose(perms[i], res);
  
  composed_perm = res;

  for (i = 0; i < MAX_REPEATS; i++) sequence[i] = random(4);

  progress = 0; current_max = START_REPEATS;

  start_timer();

  
  return ACK;
}

// Defines for module answers (on state request)
#define GAME_IDLE           0 
#define GAME_FAIL           1 
#define GAME_SOLVED         2
#define GAME_INTERNAL_ERROR 3

// Reaction of this module if some message was received
void receive_event(int num_bytes)
{
   byte msg_buffer[num_bytes];
   int i;
   for (i = 0; i < num_bytes; i++) msg_buffer[i] = (byte)(Wire.read());

   switch (msg_buffer[0])
   {
    case I2C_RESERVED:
      i2c_answer = NACK;
      break;
    case I2C_GAME_START:
      i2c_answer = init_game(msg_buffer[1]);
      game_state = GS_RUNNING;
      break;
    case I2C_GET_STATE:
      if (game_state == GS_RUNNING || game_state == GS_STOPPED)
        i2c_answer = GAME_IDLE;
      else if (game_state == GS_FAILED)
        i2c_answer = GAME_FAIL;
      else if (game_state == GS_SOLVED)
        i2c_answer = GAME_SOLVED;
      else i2c_answer = GAME_INTERNAL_ERROR;
      break;
    case I2C_GAME_OVER:
      reset();
      i2c_answer = ACK;
      break;
   }
}

void resume_game()
{
  game_state = GS_RUNNING;
  digitalWrite(FAIL_LED, LOW);

}

// Reaction of this module if information was requested
void request_event()
{
  Wire.write(i2c_answer); 
  i2c_answer = BUSY;

  if (game_state == GS_FAILED) resume_game();
}

// empty Analog pin to generate seed for RNG
#define RANDOM_PIN 3

void setup()
{
  // Initialize buttons and leds
  pinMode(R_BTN_PIN, INPUT);
  pinMode(Y_BTN_PIN, INPUT);
  pinMode(G_BTN_PIN, INPUT);
  pinMode(B_BTN_PIN, INPUT);

  pinMode(R_LED_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);

  pinMode(SOLVED_1_LED, OUTPUT);
  pinMode(SOLVED_2_LED, OUTPUT);
  pinMode(SOLVED_2_LED, OUTPUT);

  Wire.begin(I2C_ADDR);
  Wire.onReceive(receive_event);
  Wire.onRequest(request_event);

  randomSeed(analogRead(RANDOM_PIN));
  reset();
}

// Returns the Color of any button that is currently pressed and BTN_NONE otherwise
byte check_buttons() 
{
  if (digitalRead(R_BTN_PIN) == HIGH) return R;
  if (digitalRead(G_BTN_PIN) == HIGH) return G;
  if (digitalRead(B_BTN_PIN) == HIGH) return B;
  if (digitalRead(Y_BTN_PIN) == HIGH) return Y;
  return BTN_NONE;
}

// calculates the color after applying the permutation composed earlier
inline byte permutation_of(byte color)
{
  return GET(composed_perm, color);
}

void solved()
{
  digitalWrite(SOLVED_ALL_LED, HIGH);
  game_state = GS_SOLVED;
}

// Turns on additional solve-LED, triggeres GS_SOLVED if conditions are met
inline void increment_solved()
{
  to_solve = (to_solve << 1) || 0b1;
  update_to_solve();

  if (to_solve == 0b111) solved();
}

// Handles entering the sequence and checking for fail/solve (basically the game logic)
void handle_release(byte color)
{
  byte color_perm = permutation_of(color); 

  if (color_perm == sequence[progress]) // Player pressed correct button
  {
    progress++;

    if (progress >= current_max)
    {
      current_max += SEQ_INCREMENT;
      increment_solved();
      input_started = false;
    }
  } 
  else // Player pressed wrong button
  {
    game_state = GS_FAILED;
    digitalWrite(FAIL_LED, HIGH);
    input_started = false;
  }
}

// Toggles a LED of given color
void toggle_LED_by_color(byte color, byte toggle)
{
  switch (color) 
  {
    case R:
      digitalWrite(R_LED_PIN, toggle);
      break;
    case G:
      digitalWrite(G_LED_PIN, toggle);
      break;
    case B:
      digitalWrite(B_LED_PIN, toggle);
      break;
    case Y:
      digitalWrite(Y_LED_PIN, toggle);
      break;
    default: // This should not happen if the function is used in an intended way
      game_state = GS_ERROR; 
      break;
  } 
}

// Switches LED of permutation of given button color on/off
inline void toggle_LED_by_perm_color(byte color, byte toggle)
{
  toggle_LED_by_color(permutation_of(color), toggle);
}

// Handling input via the four face buttons, including activating LEDs for button presses
void handle_buttons()
{
  byte btn = check_buttons();

  if (btn != BTN_NONE)
  {
    if (pressed_color == BTN_NONE) 
    {
      input_started = true;
      pressed_color = btn;
      
      #ifdef DEBUG
        Serial.print("Button pressed: ");
        Serial.print(btn);
        Serial.print("\n");
      #endif
      
      toggle_LED_by_perm_color(pressed_color, HIGH);
    }
  }
  else
  {
    if (pressed_color != BTN_NONE)
    {
      handle_release(pressed_color);
      toggle_LED_by_perm_color(pressed_color, LOW);
      pressed_color = BTN_NONE;
    }
  }
}

// The interval in which the LEDs are toggled
#define TOGGLE_INTERVAL 500

// Time to wait at the end of the sequence
#define WAIT_TIME 1000

// Handles blinking through the sequence
void handle_sequence()
{
  if (input_started) // suspend timer if player started entering the sequence
  {
    timer = millis();
  }
  else 
  {
    int ms = millis();
  
    if (ms - timer >= TOGGLE_INTERVAL + wait_time)
    {
      wait_time = 0;
      timer = ms;
  
      if (toggled)
      {
        toggle_LED_by_color(sequence[progress_in_seq], LOW);
        progress_in_seq++;
  
        if (progress_in_seq >= current_max)
        {
          wait_time = WAIT_TIME;
          progress_in_seq = 0;
        }
      }
      else 
      {
        #ifdef DEBUG
          Serial.println("Switch to next color!");
        #endif 
        toggle_LED_by_color(sequence[progress_in_seq], HIGH);
      }
  
      // Toggle LED
      toggled = !toggled;
    }
  }
}

void loop() 
{
  switch (game_state)
  {
    case GS_RUNNING:
      handle_buttons();
      handle_sequence();
    break;
    case GS_ERROR: 
      // If the code breaks the module, don't punish the player
      solved();
    break;
  }
}
