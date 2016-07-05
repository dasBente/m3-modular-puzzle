#include <Wire.h>

#define PINBUTTON 2
#define PINLEDGREEN 3
#define PINLEDRED 4
#define PINADC A0

byte i2cAnswer=0xFF;
byte randomByte=0;
byte gameState=0; //0 - stopped, 1 - running, 2 - fail (wait for request before resume), 3 - solved, x - internal error
int expectedADCvalMin=0;
int expectedADCvalMax=1023;


void setup()
{
    pinMode(PINBUTTON, INPUT_PULLUP);
    pinMode(PINLEDGREEN, OUTPUT);
    pinMode(PINLEDRED, OUTPUT);

    Wire.begin(8);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void loop()
{
    if (gameState==1) //game is running
    {
        if (digitalRead(PINBUTTON)==0) //button pressed
        {
            int adc=analogRead(PINADC);
            if (adc>=expectedADCvalMin && adc<=expectedADCvalMax)
            {
                gameState=3;
            }
            else
            {
                gameState=2;
            }
            
            while (digitalRead(PINBUTTON)==0) delay(10); //wait for button release
        }
    }
    delay(10);
}

void receiveEvent(int numBytes)
{
    byte buffer[numBytes];
    for (int i=0; i<numBytes; i++) buffer[i]=(byte)Wire.read();

    switch(buffer[0])
    {
        case 0: //reserved
            i2cAnswer=0; //fail
            break;
        case 1: //game start / reset
            randomByte=buffer[1];
            //set ADC min/max (compare resistor is 180 Ohm)
            if (randomByte&0x80) //resistor 1 (1 Ohm)
            {
                expectedADCvalMin=4;
                expectedADCvalMax=7;
            }
            else if (randomByte&0x40) //resistor 2 (10 Ohm)
            {
                expectedADCvalMin=52;
                expectedADCvalMax=56;
            }
            else if (randomByte&0x20) //resistor 3 (180 Ohm)
            {
                expectedADCvalMin=510;
                expectedADCvalMax=514;
            }
            else if (randomByte&0x10) //resistor 4 (1 kOhm)
            {
                expectedADCvalMin=865;
                expectedADCvalMax=870;
            }
            else if (randomByte&0x08) //resistor 5 (10 kOhm)
            {
                expectedADCvalMin=1004;
                expectedADCvalMax=1008;
            }
            else if (randomByte&0x04) //resistor 6 (100 kOhm)
            {
                expectedADCvalMin=1020;
                expectedADCvalMax=1023;
            }
            
            gameState=1; //running
            i2cAnswer=1; //ack
            break;
        case 2: //game information request
            if (gameState<=1) i2cAnswer=0; //nothing
            else if (gameState==2) i2cAnswer=1; //user fail
            else if (gameState==3) i2cAnswer=2; //user solved
            else i2cAnswer=3; //internal error
            break;
        case 3: //game end
            gameState=0;
            i2cAnswer=1; //ack
            break;
        default:
            i2cAnswer=0; //fail
            break;
    }
}

void requestEvent()
{
    //if (i2cAnswer==0xFF) doSomething(); //answer not ready
    Wire.write(i2cAnswer);
    i2cAnswer=0xFF;
}

