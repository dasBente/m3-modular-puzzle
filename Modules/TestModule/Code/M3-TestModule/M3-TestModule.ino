#include <Wire.h>

#define PINBUTTONSUCCESS 2
#define PINBUTTONFAIL 3
#define PINLED 13 //internal test LED

#define LED_BLINK_TIME 100 //100*10ms before it switches on/off (10ms by delay in loop)

byte i2cAnswer=0xFF;
byte randomByte=0;
byte gameState=0; //0 - stopped, 1 - running, 2 - fail (wait for request before resume), 3 - solved, x - internal error


void setup()
{
    //define pin modes
    pinMode(PINBUTTONSUCCESS, INPUT_PULLUP);
    pinMode(PINBUTTONFAIL, INPUT_PULLUP);
    pinMode(PINLED, OUTPUT);
    //initialize i2c interface
    Wire.begin(25);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void loop()
{
    //led control (blinking when not solved)
    static int ledTime=LED_BLINK_TIME;
    static bool ledsOn=false;
    if (ledTime==0)
    {
        if (gameState==0)
        {
            ledsOn=false;
            digitalWrite(PINLED,0);
        }
        else if (gameState==1 || gameState==2) //not solved yet
        {
            //switch on/off
            ledsOn=!ledsOn;
            if (ledsOn)
            {
                digitalWrite(PINLED,1);
            }
            else
            {
                digitalWrite(PINLED,0);
            }
            ledTime=LED_BLINK_TIME;
        }
        else if (gameState==3)
        {
            ledsOn=true;
            digitalWrite(PINLED,1);
        }
    }
    else ledTime--;
    //game
    if (gameState==1) //game is running
    {
        if (digitalRead(PINBUTTONSUCCESS)==0) //success button pressed
        {
            gameState=3;
            ledsOn=true;
            digitalWrite(PINLED,1);
            ledTime=0; //displays infintely long unless gamestate changes
        }
        else if (digitalRead(PINBUTTONFAIL)==0)
        {
            gameState=2;
            ledsOn=true;
            digitalWrite(PINLED,0);
            ledTime=2*LED_BLINK_TIME;
        }
        while (digitalRead(PINBUTTONSUCCESS)==0 || digitalRead(PINBUTTONFAIL)==0) delay(10); //wait for button release
    }
    //delay
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
    Wire.write(i2cAnswer); //send answer byte
    i2cAnswer=0xFF; //set answer byte not ready
    
    //resume game after fail was noticed
    if (gameState==2) gameState=1;
}

