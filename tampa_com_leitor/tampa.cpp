#include <Arduino.h>
#include "tampa.h"

Tampa::Tampa(int _pin, int _angAberta, int _angFechada,
   int _delayFechamento){
  pin = _pin;
  angAberta = _angAberta;
  angFechada = _angFechada;
  delayFechamento = _delayFechamento;
}

void Tampa::setup(){
  this->attach(pin);
  aberta = true;
  instanteAbertura = 0;
  this->fechar();
}

void Tampa::abrir(){
  if(!aberta) {
    this->write(angAberta);
    aberta = true;
  }
  instanteAbertura = millis();
}

void Tampa::fechar(){
  if(aberta) {
    this->write(angFechada);
    aberta = false;
  }
}

bool Tampa::estaAberta(){
  return aberta;
}

bool Tampa::passouDelay(){
  return (millis()-instanteAbertura) > delayFechamento;
}
