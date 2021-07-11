#ifndef SIRENE_SERIAL_H_INCLUDED
#define SIRENE_SERIAL_H_INCLUDED

#include "sirene.h"

class SireneSerial: public Sirene {
  public:
    SireneSerial();
  	void bip();
  	void acionar(int controle);
    int estaAcionado();

  private:
    int acionado;
};


#endif // SIRENE_SERIAL_H_INCLUDED
