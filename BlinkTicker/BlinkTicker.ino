/*
    Ticker ESP8266
    Hardware: NodeMCU
    Circuits4you.com
    2018
    LED Blinking using Ticker
*/
#include <ESP8266WiFi.h>
#include <Ticker.h>  //Ticker Library

Ticker blinker;

#define LED 2  //On board LED
#define PERIODO_LED 100

//=======================================================================
void changeState()
{
  static int execs = 0;
  digitalWrite(LED, !(digitalRead(LED)));  //Invert Current State of LED
  if (++execs>10)
    blinker.detach();
    
}
//=======================================================================
//                               Setup
//=======================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println("");

    pinMode(LED,OUTPUT);

//    //Initialize Ticker every 0.5s
    blinker.attach_ms(PERIODO_LED, changeState); //Use attach_ms if you need time in ms
}
//=======================================================================
//                MAIN LOOP
//=======================================================================
void loop()
{ 
}
//=======================================================================
