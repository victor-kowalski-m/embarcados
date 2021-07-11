/*
    ALARME RESIDENCIAL
*/

#include "definicoes_sistema.h"
#include "comunicacao_serial.h"
#include "ihm_serial.h"
#include "senhas_fixa.h"
#include "sirene_serial.h"
#include "timer_interno.h"
#include "task_switcher.h"

#define TASK_INTERVAL1 500
#define TASK_INTERVAL2 500
#define TASK_INTERVAL3 1000

/***********************************************************************
 Componentes
 ***********************************************************************/
ComunicacaoSerial com;
SireneSerial sne;
InterfaceHomemMaquinaSerial ihm;
SenhasFixa sha;
TimerInterno tmr;

/***********************************************************************
 Estaticos
 ***********************************************************************/
int codigoEvento = NENHUM_EVENTO;
int eventoInterno = NENHUM_EVENTO;
int estado = ESPERA;
int codigoAcao;
int acao_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
int proximo_estado_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];

/************************************************************************
 executarAcao
 Executa uma acao
 Parametros de entrada:
    (int) codigo da acao a ser executada
 Retorno: (int) codigo do evento interno ou NENHUM_EVENTO
*************************************************************************/
int executarAcao(int codigoAcao)
{
    int retval;

    retval = NENHUM_EVENTO;
    if (codigoAcao == NENHUMA_ACAO)
        return retval;

    switch(codigoAcao)
    {
    case A01:
        tmr.iniciar(true);
        break;
    case A02:
        sne.bip();
        com.notificar("Alarme em alerta");
        tmr.iniciar(false);
        break;
    case A03:
        com.notificar("Alarme desacionado");
        tmr.iniciar(false);
        break;
    case A04:
        com.notificar("Alarme desacionado");
        break;
    case A05:
        tmr.iniciar(true);
        break;
    case A06:
        sne.acionar(true);
        com.notificar("Invasao");
        tmr.iniciar(false);
        break;
    case A07:
        com.notificar("Alarme desacionado");
        tmr.iniciar(false);
        sne.acionar(false);
        break;
    } // switch

    return retval;
} // executarAcao

/************************************************************************
 iniciaMaquina de Estados
 Carrega a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void iniciaMaquinaEstados()
{
  int i;
  int j;

  for (i=0; i < NUM_ESTADOS; i++) {
    for (j=0; j < NUM_EVENTOS; j++) {
       acao_matrizTransicaoEstados[i][j] = NENHUMA_ACAO;
       proximo_estado_matrizTransicaoEstados[i][j] = i;
    }
  }
  proximo_estado_matrizTransicaoEstados[ESPERA][ACIONAR] = SAIDA;
  acao_matrizTransicaoEstados[ESPERA][ACIONAR] = A01;

  proximo_estado_matrizTransicaoEstados[SAIDA][DESACIONAR] = ESPERA;
  acao_matrizTransicaoEstados[SAIDA][DESACIONAR] = A03;

  proximo_estado_matrizTransicaoEstados[SAIDA][TIMEOUT] = ALERTA;
  acao_matrizTransicaoEstados[SAIDA][TIMEOUT] = A02;

  proximo_estado_matrizTransicaoEstados[ALERTA][DESACIONAR] = ESPERA;
  acao_matrizTransicaoEstados[ALERTA][DESACIONAR] = A04;

  proximo_estado_matrizTransicaoEstados[ALERTA][DISPARAR] = ENTRADA;
  acao_matrizTransicaoEstados[ALERTA][DISPARAR] = A05;

  proximo_estado_matrizTransicaoEstados[ENTRADA][TIMEOUT] = ACIONADO;
  acao_matrizTransicaoEstados[ENTRADA][TIMEOUT] = A06;

  proximo_estado_matrizTransicaoEstados[ENTRADA][DESACIONAR] = ESPERA;
  acao_matrizTransicaoEstados[ENTRADA][DESACIONAR] = A07;

  proximo_estado_matrizTransicaoEstados[ACIONADO][DESACIONAR] = ESPERA;
  acao_matrizTransicaoEstados[ACIONADO][DESACIONAR] = A07;


} // initStateMachine

/************************************************************************
 iniciaSistema
 Inicia o sistema ...
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void iniciaSistema()
{
   iniciaMaquinaEstados();
} // initSystem


/************************************************************************
 obterEvento
 Obtem um evento, que pode ser da IHM ou do alarme
 Parametros de entrada: nenhum
 Retorno: codigo do evento
*************************************************************************/
char* teclas;

int decodificarAcionar()
{
    if (teclas[2] == 'a')
    {
        if (sha.validar(teclas))
        {
            return true;
        }
    }
    return false;
}//decodificarAcionar

int decodificarDesacionar()
{
    if (teclas[2] == 'd')
    {
        if (sha.validar(teclas))
        {
            return true;
        }
    }
    return false;
}//decodificarDesacionar

int decodificarDisparar()
{
    if (teclas[0] == 'l')
    {
        return true;
    }
    return false;
}//decodificarDisparar

int decodificarTimeout()
{
    return tmr.timeout();
}

/************************************************************************
 obterAcao
 Obtem uma acao da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo da acao
*************************************************************************/
int obterAcao(int estado, int codigoEvento) {
  return acao_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao


/************************************************************************
 obterProximoEstado
 Obtem o proximo estado da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo do estado
*************************************************************************/
int obterProximoEstado(int estado, int codigoEvento) {
  return proximo_estado_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao



/***********************************************************************
 Tasks
 ***********************************************************************/

/************************************************************************
 taskMaqEstados
 Task principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void taskMaqEstados() {
  if (eventoInterno != NENHUM_EVENTO) {
      codigoEvento = eventoInterno;
  }
  if (codigoEvento != NENHUM_EVENTO)
  {
      codigoAcao = obterAcao(estado, codigoEvento);
      estado = obterProximoEstado(estado, codigoEvento);
      eventoInterno = executarAcao(codigoAcao);
      Serial.print("Estado: ");
      Serial.print(estado);
      Serial.print(" Evento: ");
      Serial.print(codigoEvento);
      Serial.print(" Acao: ");
      Serial.println(codigoAcao);
  }
}

/************************************************************************
 taskObterEvento
 Task que faz pooling de eventos
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void taskObterEvento() {
  codigoEvento = NENHUM_EVENTO;

  teclas = ihm.obterTeclas();
  if (decodificarAcionar()) {
    codigoEvento = ACIONAR;
    return;
  }
  if (decodificarDesacionar()) {
    codigoEvento = DESACIONAR;
    return;
  }
  if (decodificarTimeout()) {
    codigoEvento = TIMEOUT;
    return;
  }
  if (decodificarDisparar()) {
    codigoEvento = DISPARAR;
    return;
  }
}

/************************************************************************
 taskBipBip
 Task que bipa a sirene periodicamente caso acionada
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void taskBipBip() {
  if(sne.estaAcionado())
    sne.bip();
}

/************************************************************************
 Main
 Setup e Loop principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/

void setup() {
  Serial.begin(9600);

  // configure tasks
  TaskController.createTask(&taskMaqEstados, TASK_INTERVAL1);
  TaskController.createTask(&taskObterEvento, TASK_INTERVAL2);
  TaskController.createTask(&taskBipBip, TASK_INTERVAL3);
  
  // set up timer interrupt 
  TaskController.begin(1000); // tick @1ms (1000 us)

  iniciaSistema();
  Serial.println("Alarme iniciado");
} // setup

void loop() {
  TaskController.runCurrentTask();
} // loop
