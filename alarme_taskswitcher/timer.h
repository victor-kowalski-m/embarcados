#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

class Timer {
  public:
   /*******************************
    iniciar
   Aciona ou desaciona o timer
   entradas
      controle: TRUE:liga FALSE:desliga
   saidas
      nenhuma
   ********************************/
  virtual void iniciar(int controle) = 0;

   /*******************************
    timeout
   Retorna se o timer esta em timeout.
   entradas
      nenhuma
   saidas
      FALSE: nao houve estouro do temporizador
      TRUE: houve estouro do temporizador
   ********************************/
  virtual int timeout() = 0;
};

#endif // TIMER_H_INCLUDED
