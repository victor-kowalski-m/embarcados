#ifndef SIRENE_H_INCLUDED
#define SIRENE_H_INCLUDED

class Sirene {
  public:
  /************************
   bip
  Aciona momentaneamente a sirene
  entradas
    nenhuma
  saidas
    nenhuma
  *************************/
  virtual void bip() = 0;

  /************************
   acionar
  Aciona ou desaciona a sirene
  entradas
    controle: TRUE:ligar FALSE:desligar
  saidas
    nenhuma
  *************************/
  virtual void acionar(int controle) = 0;

  /************************
   estacionado
  Retorna se está acionado ou não
  entradas
    nenhuma
  saidas
    acionado: TRUE
    desacionado: FALSE
  *************************/
  virtual int estaAcionado() = 0;
};

#endif // SIRENE_H_INCLUDED
