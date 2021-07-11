#ifndef TIMER_INTERNO_H_INCLUDED
#define TIMER_INTERNO_H_INCLUDED

#include "timer.h"

class TimerInterno: public Timer {
  public:
  TimerInterno();
  void iniciar(int controle);
  virtual int timeout();

  private:
  int situacao;
  unsigned long horaInicio;
};

#endif // TIMER_INTERNO_H_INCLUDED
