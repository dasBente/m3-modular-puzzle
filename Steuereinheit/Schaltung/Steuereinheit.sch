EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:msp430
LIBS:Steuereinheit-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L VCC 5V
U 1 1 575B1067
P 900 700
F 0 "5V" H 900 550 50  0001 C CNN
F 1 "VCC" H 900 850 50  0000 C CNN
F 2 "" H 900 700 50  0000 C CNN
F 3 "" H 900 700 50  0000 C CNN
	1    900  700 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 575B1391
P 700 6250
F 0 "#PWR?" H 700 6000 50  0001 C CNN
F 1 "GND" H 700 6100 50  0000 C CNN
F 2 "" H 700 6250 50  0000 C CNN
F 3 "" H 700 6250 50  0000 C CNN
	1    700  6250
	1    0    0    -1  
$EndComp
$Comp
L LCD-016N002L DS?
U 1 1 575B143D
P 1750 4100
F 0 "DS?" H 950 4500 50  0000 C CNN
F 1 "LCD-016N002L" H 2450 4500 50  0000 C CNN
F 2 "WC1602A" H 1750 4050 50  0000 C CIN
F 3 "" H 1750 4100 50  0000 C CNN
	1    1750 4100
	1    0    0    -1  
$EndComp
$Comp
L R 220
U 1 1 575B1740
P 2800 4050
F 0 "220 Ohm" V 2880 4050 50  0000 C CNN
F 1 "R" V 2800 4050 50  0000 C CNN
F 2 "" V 2730 4050 50  0000 C CNN
F 3 "" H 2800 4050 50  0000 C CNN
	1    2800 4050
	1    0    0    -1  
$EndComp
$Comp
L POT RV?
U 1 1 575B192D
P 1800 3250
F 0 "RV?" H 1800 3150 50  0000 C CNN
F 1 "POT" H 1800 3250 50  0000 C CNN
F 2 "" H 1800 3250 50  0000 C CNN
F 3 "" H 1800 3250 50  0000 C CNN
	1    1800 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  2000 9950 2000
Wire Wire Line
	900  700  9950 700 
Connection ~ 7300 700 
Connection ~ 9550 700 
Connection ~ 3600 2000
Connection ~ 5650 2000
$Comp
L MSP430G2553IPW20 U?
U 1 1 575B12C5
P 5050 5350
F 0 "U?" H 3550 5950 50  0000 C CNN
F 1 "MSP430G2553IPW20" H 6300 4800 50  0000 C CNN
F 2 "20PW" H 3600 4800 50  0000 C CIN
F 3 "" H 5000 5350 50  0000 C CNN
	1    5050 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	700  6250 10900 6250
Wire Wire Line
	2500 4600 2500 6250
Connection ~ 2500 6250
Wire Wire Line
	2800 4200 2800 4700
Wire Wire Line
	2800 4700 2400 4700
Wire Wire Line
	2400 4700 2400 4600
Wire Wire Line
	2800 3900 2800 700 
Connection ~ 2800 700 
Wire Wire Line
	1200 700  1200 3350
Wire Wire Line
	1200 3350 800  3350
Wire Wire Line
	800  3350 800  4750
Wire Wire Line
	800  4750 1100 4750
Connection ~ 1200 700 
Wire Wire Line
	1100 4750 1100 4600
Wire Wire Line
	1000 4600 1000 6250
Connection ~ 1000 6250
Wire Wire Line
	5050 6000 5050 6250
Connection ~ 5050 6250
Wire Wire Line
	5050 4650 3050 4650
Wire Wire Line
	3050 4650 3050 2000
Connection ~ 3050 2000
Wire Wire Line
	3350 5050 3200 5050
Wire Wire Line
	3200 5050 3200 4750
Wire Wire Line
	3200 4750 1300 4750
Wire Wire Line
	1300 4750 1300 4600
Wire Wire Line
	3350 5150 3100 5150
Wire Wire Line
	3100 5150 3100 4800
Wire Wire Line
	3100 4800 1500 4800
Wire Wire Line
	1500 4800 1500 4600
Wire Wire Line
	1400 4600 1400 6250
Connection ~ 1400 6250
Wire Wire Line
	1200 4600 1200 4800
Wire Wire Line
	1200 4800 750  4800
Wire Wire Line
	750  4800 750  3100
Wire Wire Line
	750  3100 1800 3100
Wire Wire Line
	1600 4600 1600 6250
Connection ~ 1600 6250
Wire Wire Line
	1700 4600 1700 6250
Connection ~ 1700 6250
Wire Wire Line
	1800 4600 1800 6250
Connection ~ 1800 6250
Wire Wire Line
	1900 4600 1900 6250
Connection ~ 1900 6250
Wire Wire Line
	2000 4600 2000 5250
Wire Wire Line
	2000 5250 3350 5250
Wire Wire Line
	3350 5350 2100 5350
Wire Wire Line
	2100 5350 2100 4600
Wire Wire Line
	3350 5450 2200 5450
Wire Wire Line
	2200 5450 2200 4600
Wire Wire Line
	3350 5550 2300 5550
Wire Wire Line
	2300 5550 2300 4600
Wire Wire Line
	1550 3250 700  3250
Wire Wire Line
	700  3250 700  5350
Wire Wire Line
	700  5350 850  5350
Wire Wire Line
	850  5350 850  6250
Connection ~ 850  6250
Wire Wire Line
	2050 3250 2050 700 
Connection ~ 2050 700 
Text GLabel 850  2000 0    60   Input ~ 0
3.3V
Text GLabel 800  700  0    60   Input ~ 0
5V
$EndSCHEMATC
