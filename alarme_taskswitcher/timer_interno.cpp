#include <Arduino.h>

#include "definicoes_sistema.h"
#include "timer_interno.h"

#define TEMPO 10

TimerInterno::TimerInterno()
{
   situacao = false;
}

/*******************************
 TimerInterno::iniciar
 Aciona ou desaciona o timer
 entradas
   controle: TRUE:liga FALSE:desliga
 saidas
   nenhuma
********************************/
void TimerInterno::iniciar(int controle)
{
   situacao = controle;
   if (controle)
   {
      horaInicio = millis();
   }
}

/*******************************
 TimerInterno::timeout
 Retorna se o timer esta em timeout.
 entradas
    nenhuma
 saidas
    FALSE: nao houve estouro do temporizador
    TRUE: houve estouro do temporizador
********************************/
int TimerInterno::timeout()
{
    if (situacao == false)
    {
        return false;
    }
    if(millis() - horaInicio > TEMPO*1000)
    {
        return true;
    }
    return false;
}
