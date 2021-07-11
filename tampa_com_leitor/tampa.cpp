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
  this->fecha();
}

void Tampa::abre(){
  aberta = true;
  instanteAbertura = millis();
  this->write(angAberta);
}

void Tampa::fecha(){
  aberta = false;
  this->write(angFechada);
}

bool Tampa::estaAberta(){
  return aberta;
}

bool Tampa::passouDelay(long instante){
  return (instante-instanteAbertura) > delayFechamento;
}