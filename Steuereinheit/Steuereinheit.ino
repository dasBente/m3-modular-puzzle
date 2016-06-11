/*
  Hauptprogramm der Steuereinheit
  
Umfasst Ansteuerung eines LCDs (kompatibel mit HD44780 Treiber. Z.B. Qapass 1602A).

===============================================
| PIN Belegung des Micro Kontrollers          |     
===============================================
 mC Pin |  Verbinden mit
-----------------------------------------------
  P1.0  | ---
  P1.1  | ---
  P1.2  | ---
  P1.3  | ---
  P1.4  | --- 
  P1.5  | ---
  P1.6  | ---
  P1.7  | ---
  P2.0  | RS (Register Select) (LCD)
  P2.1  | E  (Enable)          (LCD)
  P2.2  | D4                   (LCD)
  P2.3  | D5                   (LCD)
  P2.4  | D6                   (LCD)
  P2.5  | D7                   (LCD)
===============================================

===============================================
| Schaltung LCD (für HD44780)                 |
===============================================
 LCD Pin    | Verbinden mit
 ==============================================
  P1  (VSS) | GND
  P2  (VDD) | VCC
  P3  (V0)  | mittlerer Pin von Potentiometer (andere Pins: GND und VCC)
  P4  (RS)  | P2.0
  P5  (RW)  | GND
  P6  (E)   | P2.1
  P7  (D0)  | ---
  P8  (D1)  | ---
  P9  (D2)  | ---
  P10 (D3)  | --- 
  P11 (D4)  | P2.2
  P12 (D5)  | P2.3
  P13 (D6)  | P2.4
  P14 (D7)  | P2.5
  P15 (A)   | mit 220 Ohm an VCC
  P16 (K)   | GND
*/

#include <LiquidCrystal.h>
#include "utils.h"

// LCD mit den gegebenen Pins aufsetzen
LiquidCrystal lcd(P2_0, P2_1, P2_2, P2_3, P2_4, P2_5);

void setup()
{
  // Dimensionen des genutzten LCDs festlegen
  lcd.begin(16, 2);

  // Titel ausgeben, einen Moment warten und dann ins Menü gehen
  lcd.print(PSTR("Modulares"));
  lcd.setCursor(0,1);
  lcd.print(PSTR("Puzzle"));
  
  sleep(1);
  lcd.clear();
}

void loop()
{
  // put your main code here, to run repeatedly:
  
}
