/*
  Hauptprogramm der Steuereinheit
*/

#include <Servo.h>

// Analogpin, an dem das Potentiometer angeschlossen ist
#define POT_PIN 0

// Pin, an dem der Servo angeschlossen ist.
#define SERVO_PIN 9

// Servo-Objekt für Zeiger
Servo needle_servo;

void setup()
{
  needle_servo.attach(SERVO_PIN);
}

// Konstanten für max- und min-Werte von Servo und Poti
#define POT_MIN   0
#define POT_MAX   1023
#define SERVO_MIN 0
#define SERVO_MAX 180

// rechnet die Eingabe von einer Potiausgabe in einen für den Servo brachbaren Wert um.
int time_to_servo(int val) {
  return map(val, POT_MIN, POT_MAX, SERVO_MIN, SERVO_MAX);
}

// Die dem Spieler noch verbleibende Spielzeit
int time_remaining = 0;

void loop()
{
  time_remaining = analogRead(POT_PIN);
  needle_servo.write(time_to_servo(time_remaining));
  delay(15);  
}
