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
}


void Led::desligar(){
  digitalWrite(pin, LOW);
  aceso = false;
}


void Led::toggle(){
  aceso? this->desligar():this->ligar();
}


bool Led::estaLigado(){
  return aceso;
}