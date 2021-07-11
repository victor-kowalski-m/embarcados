#ifndef IHM_SERIAL_H_INCLUDED
#define IHM_SERIAL_H_INCLUDED

#include "ihm.h"

#define NENHUMA_TECLA -1

class InterfaceHomemMaquinaSerial: public InterfaceHomemMaquina {
  public:
  InterfaceHomemMaquinaSerial();
  void exibirMsg(char* texto);
  char* obterTeclas();

  private:
  char buf[10];
};

#endif // IHM_SERIAL_H_INCLUDED
