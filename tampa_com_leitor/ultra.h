#ifndef ULTRA_H_INCLUDED
#define ULTRA_H_INCLUDED

#include "sensor_presenca.h"

class Ultra: public SensorPresenca {
  public:
    Ultra(int _echoPin, int _trigPin, int _distanciaAtivacao);
    void setup();
    bool algoProximo();

  private:
    int echoPin;
    int trigPin;
    int distanciaAtivacao;
    int cm;

};

#endif