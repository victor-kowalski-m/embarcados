#ifndef SENSOR_PRESENCA_H_INCLUDED
#define SENSOR_PRESENCA_H_INCLUDED

#include "definicoes_sistema.h"

class SensorPresenca {
  public:
    virtual void setup();
    virtual bool algoProximo();
};  

#endif