#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include "definicoes_sistema.h"
#include "feedback_usuario.h"

class Led: public FeedbackUsuario {
  public:
    Led(int _pin);
    void setup();
    void ligar();
    void desligar();
    void toggle();
    bool estaLigado();

  private:
    int pin;
    bool aceso = false;

};

#endif