#include <Arduino.h>
#include "ultra.h"

Ultra::Ultra(int _echoPin, int _trigPin, int _distanciaAtivacao){
  echoPin = _echoPin;
  trigPin = _trigPin;
  distanciaAtivacao = _distanciaAtivacao;
}

void Ultra::setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

bool Ultra::algoProximo(){
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time cm
  if((pulseIn(echoPin, HIGH) * 0.034 / 2) < distanciaAtivacao)
    return true;
  return false;

}