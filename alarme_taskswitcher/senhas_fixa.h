#ifndef SENHAS_FIXA_H_INCLUDED
#define SENHAS_FIXA_H_INCLUDED

#include "senhas.h"

class SenhasFixa: public Senhas {
  public:
  SenhasFixa();
  int validar(char* texto);
};



#endif // SENHAS_FIXA_H_INCLUDED
