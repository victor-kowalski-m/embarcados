#include <Arduino.h>
#include "servo_tampa.h"

ServoTampa::ServoTampa(int _pin, int _angAberta, int _angFechada,
   int _delayFechamento){
  pin = _pin;
  angAberta = _angAberta;
  angFechada = _angFechada;
  delayFechamento = _delayFechamento;
}

void ServoTampa::setup(){
  this->attach(pin);
  aberta = true;
  instanteAbertura = 0;
  this->fechar();
  delay(500);
}

void ServoTampa::abrir(){
  if(!aberta) {
    this->write(angAberta);
    aberta = true;
  }
  instanteAbertura = millis();
}

void ServoTampa::fechar(){
  if(aberta) {
    this->write(angFechada);
    aberta = false;
  }
}

bool ServoTampa::estaAberta(){
  return aberta;
}

bool ServoTampa::passouDelay(){
  return (millis()-instanteAbertura) > delayFechamento;
}
