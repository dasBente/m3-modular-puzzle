#include <Wire.h>
#include <LiquidCrystal.h>

//LCD PINs 2, 3, 4 & 5
#define PINBUTTONCHANGE 6
#define PINBUTTONNEXT 7
#define PINBUTTONSUBMIT 8
#define PINLEDGREEN 9
#define PINLEDRED 10
//LCD PINs 12 & 11

#define LED_BLINK_TIME 100 //100*10ms before it switches on/off (10ms by delay in loop)

LiquidCrystal lcd(11, 12, 5, 4, 3, 2);

//custom arrow
byte implies[8] = {
    0b00000,
    0b00000,
    0b00100,
    0b00010,
    0b11111,
    0b00010,
    0b00100,
    0b00000
};

byte i2cAnswer=0xFF;
byte randomByte=0;
byte gameState=0; //0 - stopped, 1 - running, 2 - fail (wait for request before resume), 3 - solved, x - internal error

byte ops=0b000000; //each operator has 2 bits: 0-and, 1-or, 2-implies
byte changeop=0;


void setup()
{
    //define pin modes
    pinMode(PINBUTTONCHANGE, INPUT_PULLUP);
    pinMode(PINBUTTONNEXT, INPUT_PULLUP);
    pinMode(PINBUTTONSUBMIT, INPUT_PULLUP);
    pinMode(PINLEDGREEN, OUTPUT);
    pinMode(PINLEDRED, OUTPUT);
    //init lcd
    lcd.createChar(0, implies); //write((byte)0) to write the arrow
    lcd.begin(16,2);
    lcd.noCursor();
    lcd.noAutoscroll();
    lcd.leftToRight();
    lcd.blink();
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Cha Next Sub");
    //initialize i2c interface
    Wire.begin(9);
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
            digitalWrite(PINLEDGREEN,0);
            digitalWrite(PINLEDRED,0);
        }
        else if (gameState==1 || gameState==2) //not solved yet
        {
            //switch on/off
            ledsOn=!ledsOn;
            if (ledsOn)
            {
                digitalWrite(PINLEDGREEN,1);
                digitalWrite(PINLEDRED,1);
            }
            else
            {
                digitalWrite(PINLEDGREEN,0);
                digitalWrite(PINLEDRED,0);
            }
            ledTime=LED_BLINK_TIME;
        }
        else if (gameState==3)
        {
            ledsOn=true;
            digitalWrite(PINLEDGREEN,1);
            digitalWrite(PINLEDRED,0);
        }
    }
    else ledTime--;
    //game
    if (gameState==1) //game is running
    {
        if (digitalRead(PINBUTTONCHANGE)==0) //change operator button
        {
            //rotate through the operators
            byte op=(ops>>(4-changeop*2))&0b11;
            op=(op+1)%3;
            
            char c='^';
            if (op==1) c='v';
            else if (op==2) c=0;
            
            op=op<<(4-changeop*2);
            byte resetop=~(0b11<<(4-changeop*2));
            ops=(ops&resetop)|op;

            lcd.write((c==0 ? (byte)c : c));
            lcd.setCursor(2+4*changeop,0);
            
            while (digitalRead(PINBUTTONCHANGE)==0) delay(10); //wait for button release
        }

        if (digitalRead(PINBUTTONNEXT)==0) //next operator button
        {
            changeop=(changeop+1)%3;
            lcd.setCursor(2+4*changeop,0);
            
            while (digitalRead(PINBUTTONNEXT)==0) delay(10); //wait for button release
        }
        
        if (digitalRead(PINBUTTONSUBMIT)==0) //submit button pressed
        {
            bool correct=true;
            byte vars[4];
            byte checkop[3];
            
            //init check variables and operators
            checkop[0]=(ops>>4)&0b11;
            checkop[1]=(ops>>2)&0b11;
            checkop[2]=ops&0b11;
            vars[0]=randomByte&0b1000;
            vars[1]=randomByte&0b0100;
            vars[2]=randomByte&0b0010;
            vars[3]=randomByte&0b0001;

            //check for solution-correctness ("and" stronger as "or" stronger as "implies")
            for (int i=0; i<3; i++)
            {
                if (checkop[i]==0)
                {
                    if (vars[i] && vars[i+1])
                    {
                        vars[i]=vars[i+1]=1;
                        correct=true;
                    }
                    else
                    {
                        vars[i]=vars[i+1]=0;
                        correct=false;
                    }
                }
            }
            for (int i=0; i<3; i++)
            {
                if (checkop[i]==1)
                {
                    if (vars[i] || vars[i+1])
                    {
                        vars[i]=vars[i+1]=1;
                        correct=true;
                    }
                    else
                    {
                        vars[i]=vars[i+1]=0;
                        correct=false;
                    }
                }
            }
            for (int i=0; i<3; i++)
            {
                if (checkop[i]==2)
                {
                    if (!vars[i] || vars[i+1])
                    {
                        vars[i]=vars[i+1]=1;
                        correct=true;
                    }
                    else
                    {
                        vars[i]=vars[i+1]=0;
                        correct=false;
                    }
                }
            }
            
            //react to submission
            if (correct) //correct solution
            {
                gameState=3;
                lcd.noCursor();
                //light green LED only for completion indication
                ledsOn=true;
                digitalWrite(PINLEDGREEN,1);
                digitalWrite(PINLEDRED,0);
                ledTime=0; //displays infintely long unless gamestate changes
            }
            else //wrong solution
            {
                gameState=2;
                //light red LED only for two LED blink times
                ledsOn=true;
                digitalWrite(PINLEDGREEN,0);
                digitalWrite(PINLEDRED,1);
                ledTime=2*LED_BLINK_TIME;
            }
            
            while (digitalRead(PINBUTTONSUBMIT)==0) delay(10); //wait for button release
        }
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
            ops=0;
            changeop=0;
            lcd.home();
            lcd.print("A ^ B ^ C ^ D");
            lcd.setCursor(2+4*changeop,0);
            lcd.cursor();
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
            lcd.home();
            lcd.print("                "); //empty first line
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

