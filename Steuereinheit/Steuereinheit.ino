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

// Servo-Objekt für Zeiger
Servo needle_servo;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);

  #ifdef DEBUG
    pinMode(DEBUG_LED_PIN, OUTPUT);
  #endif
  
  needle_servo.attach(SERVO_PIN);
}

// gibt an, ob der Wert von remaining_time über Pot manipuliert werden kann
bool hold = false;

int button_state = 0;

void handle_button() 
{
  if (!hold) {
    button_state = digitalRead(BUTTON_PIN);

    if (button_state == HIGH) 
    { 
      hold = true; 
    }
  }
}

// Konstanten für max- und min-Werte von Servo und Poti
#define POT_MIN   0
#define POT_MAX   1023
#define SERVO_MIN 0
#define SERVO_MAX 180

// rechnet die Eingabe von einer Potiausgabe in einen für den Servo brachbaren Wert um.
inline int time_to_servo(int val) 
{
  return map(val, POT_MIN, POT_MAX, SERVO_MIN, SERVO_MAX);
}

// Die dem Spieler noch verbleibende Spielzeit
int time_remaining = 0;

void loop()
{
  handle_button();
  if (!hold) time_remaining = analogRead(POT_PIN);

  #ifdef DEBUG
    digitalWrite(DEBUG_LED_PIN, hold);
  #endif

  // Stand des Servos updaten
  needle_servo.write(time_to_servo(time_remaining));
  delay(15);  
}
