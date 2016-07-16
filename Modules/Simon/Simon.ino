#include <Wire.h>

#define R_BTN_PIN 12
#define R_LED_PIN 13
#define Y_BTN_PIN 10
#define Y_LED_PIN 11
#define G_BTN_PIN 8
#define G_LED_PIN 9
#define B_BTN_PIN 6
#define B_LED_PIN 7

#define SOLVED_1_PIN 5
#define SOLVED_2_PIN 4
#define SOLVED_3_PIN 3

// Maybe some controll LED to indicate, that mC has a connection to the controller?

// keeps track of how many iterations of the permutation still have to be solved
int to_solve;

// Answer to be send to the controller
byte i2cAnswer = 0xff;

// Seed received by controller for randomization
byte randomByte;

// Resets module state into idle mode
void reset() 
{
  digitalWrite(R_LED_PIN, LOW);
  digitalWrite(Y_LED_PIN, LOW);
  digitalWrite(G_LED_PIN, LOW);
  digitalWrite(B_LED_PIN, LOW);

  to_solve = 0;
  update_to_solve();
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
