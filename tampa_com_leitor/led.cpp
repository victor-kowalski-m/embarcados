#include <Arduino.h>
#include "led.h"

Led::Led(int _pin){
  pin = _pin;
}

void Led::setup(){
  pinMode(pin, OUTPUT);
}

void Led::ligar(){
  digitalWrite(pin, HIGH);
  aceso = true;
  instanteToggle = millis();
}

void Led::desligar(){
  digitalWrite(pin, LOW);
  aceso = false;
  instanteToggle = millis();
}

void Led::toggle(){
  aceso? this->desligar():this->ligar();
}

bool Led::estaAceso(){
  return aceso;
}

long Led::ultimoToggle(){
  return instanteToggle;
}