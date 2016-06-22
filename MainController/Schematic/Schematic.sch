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
	750  6250 11100 6250
Wire Wire Line
	650  750  11100 750 
Wire Wire Line
	1150 2150 1150 750 
Connection ~ 1150 750 
Wire Wire Line
	1150 4450 1150 6250
Connection ~ 1150 6250
Connection ~ 4500 6250
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
	3900 5750 3900 6250
Connection ~ 3900 6250
Wire Wire Line
	3900 5550 3900 750 
Connection ~ 3900 750 
$EndSCHEMATC
