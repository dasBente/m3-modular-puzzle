#include <Wire.h>

#define PINBUTTON 2
#define PINLEDGREEN 3
#define PINLEDRED 4

byte i2cAnswer=0xFF;
byte randomByte=0;
byte gameState=0; //0 - stopped, 1 - running, 2 - fail (wait for request before resume), 3 - solved, x - internal error


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
        //TODO game
    }
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

