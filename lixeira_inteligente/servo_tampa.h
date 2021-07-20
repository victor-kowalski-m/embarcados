#ifndef TAMPA_INCLUDED
#define TAMPA_INCLUDED

#include "definicoes_sistema.h"
#include "atuador_tampa.h"
#include <Servo.h>

class ServoTampa : public AtuadorTampa, public Servo { //Servo {
  public:
    ServoTampa(int _pin, int _angAberta, int _angFechada,
      int _delayFechamento);
    void setup();
    void abrir();
    void fechar();
    bool estaAberta();
    bool passouDelay();

  private:
    int pin;
    int angAberta;
    int angFechada;
    bool aberta;
    long instanteAbertura;
    int delayFechamento;
};

#endif
