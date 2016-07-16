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

#define SOLVED_ALL_LED 4
#define FAIL_LED       3

#define I2C_ADDR 10 

// keeps track of how many iterations of the permutation still have to be solved
int to_solve;

// Answer to be send to the controller
byte i2cAnswer = 0xff;

// Seed received by controller for randomization
byte randomByte;

byte game_state;

typedef enum {
  GS_STOPPED, // Module is idle and waiting for start of a game
  GS_RUNNING, // The module is currently running
  GS_SOLVED, // Module has been succesfully solved
  GS_FAILED, // A fail has been triggered
  GS_ERROR // Module suffered from internal error
} GameState;

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

  game_state = GS_STOPPED;
}

// Updates the state of the Solution-indicator leds
void update_to_solve() 
{
  digitalWrite(SOLVED_1_PIN, bitRead(to_solve, 0));
  digitalWrite(SOLVED_2_PIN, bitRead(to_solve, 1));
  digitalWrite(SOLVED_3_PIN, bitRead(to_solve, 2));
}

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

  pinMode(SOVLED_1_PIN, OUTPUT);
  pinMode(SOLVED_2_PIN, OUTPUT);
  pinMode(SOLVED_2_PIN, OUTPUT);
  Wire.begin(I2C_ADDR);

  reset();
}

// Switches solved-LEDs on, generates some sequence
void start_game() 
{
  to_solve = 0b111;
  update_to_solve();
}

void loop() 
{
  
}
