/*
  Hauptprogramm der Steuereinheit
*/

#include <Servo.h>

#define DEBUG // Debug build

// Analogpin, an dem das Potentiometer angeschlossen ist
#define POT_PIN 0

// Pin, an dem der Servo angeschlossen ist.
#define SERVO_PIN 9

// Pin, an dem der Button zum festlegen der Spielzeit angeschlossen ist
#define BUTTON_PIN 2

// Zusätzliche Variablen etc. für Debug-Zwecke
#ifdef DEBUG
  #define DEBUG_LED_PIN 13 // Pin für Debug LED
#endif

// enum das den Zustand des Spiels angibt
typedef enum {
  GS_INIT,    // mC ist am initialisieren
  GS_STARTUP, // neue Spielrunde wird initialisiert 
  GS_SETUP,   // Einstellen der Spielzeit möglich  
  GS_HOLD,    // Spielzeit ist gesetzt, warten auf Start des Spiels
  GS_ONGOING, // Spiel läuft derzeit
  GS_LOST,    // Spiel zuende, Spieler hat verloren
  GS_WON      // Spiel zuende, Spieler hat gewonnen
} GameState;

// Servo-Objekt für Zeiger
Servo needle_servo;

GameState game_state = GS_INIT;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);

  #ifdef DEBUG
    pinMode(DEBUG_LED_PIN, OUTPUT);
    Serial.begin(9800);
  #endif
  
  needle_servo.attach(SERVO_PIN);
  game_state = GS_SETUP;
}

// gibt an, ob der Push-Button derzeit gedrückt ist
bool held = false;

int button_state = 0;

// Wird beim herunterdrücken des Buttons ausgeführt
void onButtonPress() {}

// Wird beim loslassen des Buttons ausgeführt
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
      game_state = GS_SETUP; // TODO Logik für starten einer neuen Runde
      break;
  }
}

// Wird ausgeführt, wenn der Button gehalten wird
void onButtonHold() {}

// Verwaltet an den Button gebundene Logik
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

// Konstanten für max- und min-Werte von Servo und Poti
#define POT_MIN   0
#define POT_MAX   1023
#define SERVO_MIN 0
#define SERVO_MAX 180

// Maximale und minimale Einstellung für die Spielzeit
#define TIME_MIN 60
#define TIME_MAX 300

// rechnet die verbleibende Spielzeit in eine Servoeinstellung um.
inline int time_to_servo(int val) 
{
  return map(val, 0, TIME_MAX, SERVO_MAX, SERVO_MIN);
}

inline int poti_to_time(int val)
{
  return map(val, POT_MIN, POT_MAX, TIME_MIN, TIME_MAX);
}

// Die dem Spieler noch verbleibende Spielzeit
int time_remaining = 0;

int timer = 0;
int ms = 0;

void start_timer() {
  timer = millis(); 
}

// Verwaltet die verbleibende Zeit, zählt im Sekundentakt runter, updated Servo
void handle_time() 
{
  if (timer == 0) start_timer();

  ms = millis();
  if (ms - timer > 1000) 
  {
    time_remaining -= (ms - timer) / 1000;
    timer = ms; 
  }

  int dev = random(-2,2);
  needle_servo.write(constrain(time_to_servo(time_remaining) + dev, 0, TIME_MAX));
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

      if (remaining_time <= 0) game_state = GS_LOST;
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
