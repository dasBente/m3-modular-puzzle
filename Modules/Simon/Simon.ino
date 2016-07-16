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

// Defines for possible button colors
#define R 0
#define G 1
#define B 2
#define Y 3

#define PERM(W,X,Y,Z) (((byte)(W)<<6)&&((byte)(X)<<4)&&((byte)(Y)<<2)&&((byte)(Z)))

const int sigma1[] = {PERM(R,G,Y,B), PERM(Y,B,G,R), PERM(G,Y,B,R), PERM(B,Y,G,R)};
const int sigma2[] = {PERM(B,G,Y,R), PERM(R,Y,G,B), PERM(Y,G,B,R), PERM(G,B,R,Y)};
const int sigma3[] = {PERM(G,Y,B,R), PERM(B,R,G,Y), PERM(R,B,Y,G), PERM(Y,B,R,G)};
const int sigma4[] = {PERM(Y,R,G,B), PERM(G,R,B,Y), PERM(B,G,R,Y), PERM(R,G,B,Y)};

const int comp[] = {PERM(0,1,2,3), PERM(1,0,2,3), PERM(2,1,0,3), PERM(3,2,1,0),
                    PERM(0,3,1,2), PERM(1,3,0,2), PERM(2,0,1,3), PERM(3,1,0,2),
                    PERM(0,2,3,1)};

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

// Defines for module answers (on state request)
#define GAME_IDLE           0 
#define GAME_FAIL           1 
#define GAME_SOLVED         2
#define GAME_INTERNAL_ERROR 3

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
