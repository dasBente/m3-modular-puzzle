/*
  Mainprogramm of the controll unit
*/

#include <Servo.h>

#define DEBUG // Debug build

// Analog pin connected to the pot washer controlling the servo position
#define POT_PIN 0

// Pin connected to the servo (for controlling the servo)
#define SERVO_PIN 9

// Pin connected to the button for controlling the game "menu"
#define BUTTON_PIN 2

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

// Resets the game to some initial state without having to reboot the mC
void reset_game() 
{
  tries = 0;
  timer = 0;
  ms = 0;
  time_remaining = 0;
  
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
  #endif
  
  needle_servo.attach(SERVO_PIN);

  reset_game();
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
      game_state = GS_RESET;
    case GS_LOST:
      game_state = GS_RESET; 
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

void loop()
{
  handle_button();
  switch (game_state) {
    case GS_SETUP:
      time_remaining = poti_to_time(analogRead(POT_PIN));
      needle_servo.write(time_to_servo(time_remaining));
      delay(15); // Notwendig?
      break;
    case GS_STARTUP:
      handle_time();

      if (time_remaining <= 0) game_state = GS_LOST;
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
