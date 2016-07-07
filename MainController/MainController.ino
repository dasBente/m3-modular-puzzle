/*
  Mainprogramm of the controll unit
*/

#include <Servo.h>
#include <Wire.h>

#define DEBUG // Debug build

// Analog pin connected to the pot washer controlling the servo position
#define POT_PIN 0

// Pin connected to the servo (for controlling the servo)
#define SERVO_PIN 9

// Pin connected to the button for controlling the game "menu"
#define BUTTON_PIN 2

// Pin connected to indicators for tries
#define TRY_1_LED 5
#define TRY_2_LED 6
#define TRY_3_LED 7

// Defines and variables for the sake of debugging
#ifdef DEBUG
  #define DEBUG_LED_PIN 13 // Pin connected to a (onboard) LED
#endif

// Constants for the minimal and maximal values related to pot washer and servo
#define POT_MIN   0
#define POT_MAX   1023
#define SERVO_MIN 0
#define SERVO_MAX 180

// Constants for maximal and minimal values the timer can be set to
#define TIME_MIN 60
#define TIME_MAX 300

// Flags indicating acknowledgement or the lack thereof
#define ACK  1
#define NACK 0
#define BUSY 0xFF // Module can't respond to the query right now

// Maximal number of modules expected
#define NUM_MODULES 6

// Values for communication protocol
#define I2C_RESERVED  0 // Not yet in use
#define I2C_RESET     1 // Tell modules to reinitialize
#define I2C_GET_STATE 2 // Tell modules to prepare informations about their state
#define I2C_END       3 // Tell modules, the game has ended

// Highest and lowest possible I2C addresses
#define MIN_I2C_ADDR 8
#define MAX_I2C_ADDR 127

// Enum that keeps track of the current game state
typedef enum {
  GS_RESET,   // Reinitialize game
  GS_SETUP,   // allows the game master to set a time for the player to solve the game in 
  GS_HOLD,    // state after setting the game time. Game starts on next button press
  GS_STARTUP, // initializing a new game 
  GS_ONGOING, // game is currently ongoing
  GS_LOST,    // the game is over, the player has lost the game
  GS_WON      // the game is over, the player has won the game
} GameState;

// Servo-object for the time-indicating needle
Servo needle_servo;

// Variable containing current game state
GameState game_state = GS_RESET;

// Is set to true if the push button on BUTTON_PIN is currently held down
bool held = false;

int button_state = 0;

// Time still remaining in the current round of the game
int time_remaining = 0;

int timer = 0;
int ms = 0;

// Maximal number of mistakes before Game Over is triggered
int tries = 0;

// Array of up to 6 module addresses. A empty module slot gets the address 0
int modules[NUM_MODULES] = {0};

// The number of modules found 
char modules_found; 

// Resets the game to some initial state without having to reboot the mC
void reset_game() 
{
  tries = 0;
  timer = 0;
  ms = 0;
  time_remaining = 0;
  modules_found = 0;
  
  // Switch of indicator LED's for now
  digitalWrite(TRY_1_LED, LOW);
  digitalWrite(TRY_2_LED, LOW);
  digitalWrite(TRY_3_LED, LOW);

  game_state = GS_SETUP;
}

void setup()
{
  pinMode(BUTTON_PIN, INPUT);

  #ifdef DEBUG
    pinMode(DEBUG_LED_PIN, OUTPUT);
    Serial.begin(9800);
    Serial.write("Serial monitor initialized!");
  #endif

  // Enable try LEDs
  pinMode(TRY_1_LED, OUTPUT);
  pinMode(TRY_2_LED, OUTPUT);
  pinMode(TRY_3_LED, OUTPUT);
  
  needle_servo.attach(SERVO_PIN);

  // Initialize I2C connection and join bus as master
  Wire.begin();

  reset_game();
}

// Sets up the maximal number of times, the player can make a mistake
void setup_tries() 
{
  // Update tries according to the time currently remaining
  if (time_remaining <= 120) 
  {
    tries = 0b001;
  } else if (time_remaining <= 240) {
    tries = 0b011;
  } else {
    tries = 0b111;
  }

  update_try_leds();
}

// Reduces number of tries by 1
void lose_try() 
{
  tries = tries >> 1;
  update_try_leds();

  #ifdef DEBUG 
    Serial.print("Remaining tries: ");
    Serial.print(tries, HEX);
    Serial.print("\n");
  #endif
  
  if (tries == 0) 
  {
    game_state = GS_LOST;  
  }
}

// Updates the indicator leds for tries
void update_try_leds() 
{
  digitalWrite(TRY_1_LED, bitRead(tries,0));
  digitalWrite(TRY_2_LED, bitRead(tries,1));
  digitalWrite(TRY_3_LED, bitRead(tries,2));
}

// End all non-solved modules
void shutdown_modules()
{
  char i;
  for (i = 0; i < NUM_MODULES; i++)
  {
    if (modules[i] >= MIN_I2C_ADDR && modules[i] <= MAX_I2C_ADDR) 
    {
      Wire.beginTransmission(modules[i]);
      Wire.write(I2C_END);
      Wire.endTransmission();

      char c;

      #ifdef DEBUG
        Serial.print("Shutdown sent to ");
        Serial.print(modules[i]);
        Serial.print("\n");
      #endif
      
      do {
        Wire.requestFrom(modules[i], 1);

        while (Wire.available()) 
        {
          c = Wire.read();
        }
        //if (c == NACK) doSomething(); // Error handling
      } while (c == BUSY);
    }
  }
}

// Executed if the button was just pressed
void onButtonPress() {}

// Executed if the button was just released
void onButtonRelease() 
{
  switch (game_state) {
    case GS_SETUP: 
      game_state = GS_HOLD;
      break;
    case GS_HOLD:
      game_state = GS_STARTUP;
      break;
    case GS_WON:
    case GS_LOST:
      game_state = GS_RESET; 
      shutdown_modules();
      break;
  }
}

// Executed if the button is currently held down
void onButtonHold() {}

// Manages logic related to button. Press-Events are handled by the functions above
void handle_button() 
{
  button_state = digitalRead(BUTTON_PIN);
  
  if (button_state == HIGH) 
  {
    if (!held) 
    {
      held = true;
      onButtonPress();
    } else {
      onButtonHold();
    }
  } else {
    if (held) 
    {
      held = false;
      onButtonRelease();
    }
  }
}

// Maps a value of game time to a value used by the servo 
inline int time_to_servo(int val) 
{
  return map(val, 0, TIME_MAX, SERVO_MAX, SERVO_MIN);
}

// Maps a value retrieved from the pot washer to a game time value
inline int poti_to_time(int val)
{
  return map(val, POT_MIN, POT_MAX, TIME_MIN, TIME_MAX);
}

void start_timer() {
  timer = millis(); 
}

// Manages remaining time, decrements timer each second, updates servo accordingly
void handle_time() 
{
  if (timer == 0) start_timer();

  ms = millis();
  if (ms - timer > 1000) 
  {
    time_remaining -= (ms - timer) / 1000;
    timer = ms; 
  }
  
  needle_servo.write(constrain(time_to_servo(time_remaining), 0, TIME_MAX));
}

void init_modules()
{
  // Generate random number
  srand(millis());
  char random_value = rand();
  
  // Find all plugged in modules and initialize them with a random value
  unsigned char i;

  char res;
  for (i = MIN_I2C_ADDR; i <= MAX_I2C_ADDR; i++) 
  {
    Wire.beginTransmission(i);
      Wire.write(I2C_RESET);
      Wire.write(random_value);
    res = Wire.endTransmission();

    if (res == 0) // successful connection
    {
      modules[modules_found] = i;
      modules_found++;

      #ifdef DEBUG
        Serial.print("Module found at ");
        Serial.print(modules[modules_found - 1]);
        Serial.print("\n");
      #endif
    }

    if (modules_found == NUM_MODULES) break;
  }

  // Check if all modules are ready
  for (i = 0; i < modules_found; i++) 
  {
    char c;

    do {
      Wire.requestFrom(modules[i], 1);
      
      while (Wire.available())
      { 
        c = Wire.read();
      }
      // if (c == NACK) doSomething(); 
    } while (c == BUSY); // Request data untill module is ready 
  }
}

// Defines for possible return values of a module
#define IDLE_STATE     0
#define FAIL           1
#define SOLVE          2
#define INTERNAL_ERROR 3

void handle_modules()
{
    // Loop through modules and request information about their state
    unsigned char i;
    char c;
    
    for (i = 0; i < NUM_MODULES; i++)
    {
      if (modules[i] >= MIN_I2C_ADDR && modules[i] <= MAX_I2C_ADDR)
      {
        Wire.beginTransmission(modules[i]);
        Wire.write(I2C_GET_STATE);
        Wire.endTransmission();
        
        Wire.requestFrom(modules[i], 1);
        
        while (Wire.available())
        {
          c = Wire.read();
        }

        #ifdef DEBUG
          if (c == 1 || c == 2 || c == 3)
          {
            Serial.print("Received: ");
            Serial.print((int)(c));
            Serial.print("\n");
          }
        #endif
        
        switch (c) 
        {
          case IDLE_STATE:
            break;
          case FAIL:
            #ifdef DEBUG
              Serial.print("Am I called?\n");
            #endif
            
            lose_try();
            break;
          case SOLVE:
            modules_found--; // Disconnect module
            break;
          case INTERNAL_ERROR:
            // Error handling goes here
            break;
        }
      }
    }
}

void loop()
{
  handle_button();
  
  switch (game_state) {
    case GS_SETUP:
      time_remaining = poti_to_time(analogRead(POT_PIN));
      needle_servo.write(time_to_servo(time_remaining));
      
      setup_tries();
      
      delay(15); // Notwendig?
      break;
    case GS_STARTUP:
      init_modules();
      game_state = GS_ONGOING;
    case GS_ONGOING:
      handle_time();
      handle_modules();

      if (time_remaining <= 0) game_state = GS_LOST;
      if (modules_found == 0) game_state = GS_WON;
      
      break;
    case GS_LOST:
      // Notify player of his loss
      break;
    case GS_WON:
      // Notify player of his win
      break;
    case GS_RESET:
      reset_game();
      break;
  }
  
  #ifdef DEBUG
    digitalWrite(DEBUG_LED_PIN, held);
    if (Serial.available()) 
    {
      Serial.print(time_remaining);
      Serial.print("\n");
    }
  #endif
}