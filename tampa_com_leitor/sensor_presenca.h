#ifndef SENSOR_PRESENCA_H_INCLUDED
#define SENSOR_PRESENCA_H_INCLUDED

class SensorPresenca {
  public:
    virtual void setup();
    virtual bool algoProximo();
};  

#endif