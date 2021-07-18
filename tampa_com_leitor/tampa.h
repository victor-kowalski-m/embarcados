#ifndef TAMPA_INCLUDED
#define TAMPA_INCLUDED

#include <Servo.h>
#include "definicoes_sistema.h"

class Tampa : public Servo {
  public:
    Tampa(int _pin, int _angAberta, int _angFechada,
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
