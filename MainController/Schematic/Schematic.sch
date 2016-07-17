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
LIBS:servo
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
L ATMEGA328-P IC?
U 1 1 5761B816
P 2050 3250
F 0 "IC?" H 1300 4500 50  0000 L BNN
F 1 "ATMEGA328-P" H 2450 1850 50  0000 L BNN
F 2 "DIL28" H 2050 3250 50  0000 C CIN
F 3 "" H 2050 3250 50  0000 C CNN
	1    2050 3250
	1    0    0    -1  
$EndComp
$Comp
L SERVO P?
U 1 1 5761B9F2
P 4100 5450
F 0 "P?" H 4150 5505 60  0000 C CNN
F 1 "SERVO" H 4175 4995 60  0000 C CNN
F 2 "" H 4100 5450 60  0000 C CNN
F 3 "" H 4100 5450 60  0000 C CNN
	1    4100 5450
	1    0    0    -1  
$EndComp
$Comp
L POT RV?
U 1 1 5761BED6
P 3300 5550
F 0 "RV?" H 3300 5450 50  0000 C CNN
F 1 "POT" H 3300 5550 50  0000 C CNN
F 2 "" H 3300 5550 50  0000 C CNN
F 3 "" H 3300 5550 50  0000 C CNN
	1    3300 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  6250 11100 6250
Wire Wire Line
	800  750  11100 750 
Wire Wire Line
	1150 2150 1150 750 
Connection ~ 1150 750 
Wire Wire Line
	1150 4450 1150 6250
Connection ~ 1150 6250
Wire Wire Line
	3750 2250 3750 5650
Wire Wire Line
	3750 5650 3900 5650
Wire Wire Line
	3550 750  3550 5550
Wire Wire Line
	3050 5550 3050 6250
Connection ~ 3050 6250
Connection ~ 3550 750 
Wire Wire Line
	3050 3000 3300 3000
Wire Wire Line
	3300 3000 3300 5400
Wire Wire Line
	3750 2250 3050 2250
Wire Wire Line
	3900 6250 3900 5750
Connection ~ 3900 6250
Wire Wire Line
	3900 5550 3900 750 
Connection ~ 3900 750 
$Comp
L LED D?
U 1 1 577E6E9D
P 4500 4850
F 0 "D?" H 4500 4950 50  0000 C CNN
F 1 "LED" H 4500 4750 50  0000 C CNN
F 2 "" H 4500 4850 50  0000 C CNN
F 3 "" H 4500 4850 50  0000 C CNN
	1    4500 4850
	-1   0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 577E6F7A
P 4500 4550
F 0 "D?" H 4500 4650 50  0000 C CNN
F 1 "LED" H 4500 4450 50  0000 C CNN
F 2 "" H 4500 4550 50  0000 C CNN
F 3 "" H 4500 4550 50  0000 C CNN
	1    4500 4550
	-1   0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 577E6FBD
P 4500 4250
F 0 "D?" H 4500 4350 50  0000 C CNN
F 1 "LED" H 4500 4150 50  0000 C CNN
F 2 "" H 4500 4250 50  0000 C CNN
F 3 "" H 4500 4250 50  0000 C CNN
	1    4500 4250
	-1   0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 577E709E
P 5100 4850
F 0 "R?" V 5180 4850 50  0000 C CNN
F 1 "220" V 5100 4850 50  0000 C CNN
F 2 "" V 5030 4850 50  0000 C CNN
F 3 "" H 5100 4850 50  0000 C CNN
	1    5100 4850
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 577E71D5
P 5100 4550
F 0 "R?" V 5180 4550 50  0000 C CNN
F 1 "220" V 5100 4550 50  0000 C CNN
F 2 "" V 5030 4550 50  0000 C CNN
F 3 "" H 5100 4550 50  0000 C CNN
	1    5100 4550
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 577E721E
P 5100 4250
F 0 "R?" V 5180 4250 50  0000 C CNN
F 1 "220" V 5100 4250 50  0000 C CNN
F 2 "" V 5030 4250 50  0000 C CNN
F 3 "" H 5100 4250 50  0000 C CNN
	1    5100 4250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3050 4250 4300 4250
Wire Wire Line
	4700 4250 4950 4250
Wire Wire Line
	5250 4250 5250 6250
Connection ~ 5250 6250
Wire Wire Line
	3050 4350 4300 4350
Wire Wire Line
	4300 4350 4300 4550
Wire Wire Line
	4700 4550 4950 4550
Wire Wire Line
	4700 4850 4950 4850
Wire Wire Line
	3050 4450 4200 4450
Wire Wire Line
	4200 4450 4200 4850
Wire Wire Line
	4200 4850 4300 4850
Wire Bus Line
	800  7050 6500 7050
Wire Bus Line
	800  7500 6500 7500
Wire Bus Line
	3050 3500 5450 3500
Entry Bus Bus
	5350 7050 5450 6950
Wire Bus Line
	5450 3500 5450 6950
Entry Bus Bus
	5500 7500 5600 7400
Wire Bus Line
	3050 3400 5600 3400
Wire Bus Line
	5600 3400 5600 7400
$Comp
L SW_PUSH SW?
U 1 1 577E8C5E
P 6300 3950
F 0 "SW?" H 6450 4060 50  0000 C CNN
F 1 "SW_PUSH" H 6300 3870 50  0000 C CNN
F 2 "" H 6300 3950 50  0000 C CNN
F 3 "" H 6300 3950 50  0000 C CNN
	1    6300 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3950 6000 3950
$Comp
L R R?
U 1 1 577E909C
P 5900 4100
F 0 "R?" V 5980 4100 50  0000 C CNN
F 1 "10k" V 5900 4100 50  0000 C CNN
F 2 "" V 5830 4100 50  0000 C CNN
F 3 "" H 5900 4100 50  0000 C CNN
	1    5900 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4250 5900 6250
Connection ~ 5900 6250
Wire Wire Line
	6600 3950 6600 750 
Connection ~ 6600 750 
Text GLabel 800  7500 0    60   Input ~ 0
SDA
Text GLabel 800  7050 0    60   Input ~ 0
SCL
Text GLabel 800  6250 0    60   Input ~ 0
GND
Text GLabel 800  750  0    60   Input ~ 0
VCC
$EndSCHEMATC
