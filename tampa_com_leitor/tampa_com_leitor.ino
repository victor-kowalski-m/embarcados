/*
    LIXEIRA INTELIGENTE
*/

#include "leitor.h"
#include "ultra.h"
#include "tampa.h"
#include "led.h"
#include "task_switcher.h"
#include "esp8266.h"

/***********************************************************************
 Estaticos
 ***********************************************************************/
int codigoEvento;
int eventoInterno;
int estado;
int codigoAcao;
int acao_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
int proximo_estado_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];

int tentativas_conexao;
char codigoDeBarras[20];
char resposta_site[2];

char idxTaskMaqEstados;
char idxTaskObterEvento;
char idxTaskPiscaLeds;
char idxTaskPiscaVerde;
char idxTaskPiscaVermelho;

/***********************************************************************
 Componentes
 ***********************************************************************/
Ultra ultra(ECHO_PIN, TRIGGER_PIN, DIST_ATIVA_ULTRA);
Tampa tampa(SERVO_TAMPA, TAMPA_ABERTA, TAMPA_FECHADA, DELAY_TAMPA);
Led ledVerde(LED_VERDE);
Led ledVermelho(LED_VERMELHO);
Leitor leitor(codigoDeBarras);
ESP8266 esp8266(ESP8266_rxPin, ESP8266_txPin);


int executarAcao(int codigoAcao) {
    
    int retval = NENHUM_EVENTO;
    if (codigoAcao == NENHUMA_ACAO)
        return retval;

    switch(codigoAcao)
    {
      case A08: // tenta_conectar à rede
          TaskController.ativaTask(idxTaskPiscaLeds, 500, 0);
          tampa.detach();
          if (tentativas_conexao++ < 3)
            if(esp8266.conectaRede()) {
              TaskController.desativaTask(idxTaskPiscaLeds);
              tampa.attach(SERVO_TAMPA);
              retval = SUCESSO;
            } else {
              retval = TENTAR_CONEXAO;
          } else {
            TaskController.desativaTask(idxTaskPiscaLeds);
            retval = SEM_INTERNET;
          }
          break;
      case A09: // pisca led vermelho eternamente
          TaskController.ativaTask(idxTaskPiscaVermelho, 200, 0);
          break;
      case A01: // abre tampa
          tampa.abrir();
          break;
      case A02: // fecha tampa
          tampa.fechar();
          break;
      case A03: // inicia upload de codigo lido
          leitor.resetar();
          TaskController.ativaTask(idxTaskPiscaLeds, 100, 0);
          tampa.detach();
          esp8266.fazRequest(codigoDeBarras);
          tampa.attach(SERVO_TAMPA);         
          TaskController.desativaTask(idxTaskPiscaLeds);     
          if ((resposta_site[1]-48) == DECREMENTOU){
            retval = SUCESSO;
          } else {
            retval = ERRO;
          }
          break;
      case A04: // ignora código lido
          leitor.resetar();
          break;
      case A05: // indica sucesso na conexão/upload
          TaskController.ativaTask(idxTaskPiscaVerde, 100, 20);
          break;
      case A06: // indica erro na conexão/upload
          TaskController.ativaTask(idxTaskPiscaVermelho, 100, 20);
          break;
    }

    return retval;
}


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

  proximo_estado_matrizTransicaoEstados[SETUP][SUCESSO] = ESPERA;
  acao_matrizTransicaoEstados[SETUP][SUCESSO] = A05;
  acao_matrizTransicaoEstados[SETUP][TENTAR_CONEXAO] = A08;

  proximo_estado_matrizTransicaoEstados[SETUP][SEM_INTERNET] = DESCONECTADO;
  acao_matrizTransicaoEstados[SETUP][SEM_INTERNET] = A09;

  proximo_estado_matrizTransicaoEstados[ESPERA][PRESENCA] = LEITURA;
  acao_matrizTransicaoEstados[ESPERA ][PRESENCA] = A01;
  acao_matrizTransicaoEstados[LEITURA][PRESENCA] = A01;
  acao_matrizTransicaoEstados[UPLOAD ][PRESENCA] = A01;

  proximo_estado_matrizTransicaoEstados[LEITURA][CODIGO] = UPLOAD;
  acao_matrizTransicaoEstados[LEITURA][CODIGO] = A03;
  acao_matrizTransicaoEstados[UPLOAD ][CODIGO] = A04;
  acao_matrizTransicaoEstados[ESPERA ][CODIGO] = A04;

  proximo_estado_matrizTransicaoEstados[UPLOAD][SUCESSO] = LEITURA;
  acao_matrizTransicaoEstados[UPLOAD][SUCESSO] = A05;

  proximo_estado_matrizTransicaoEstados[UPLOAD][ERRO] = LEITURA;
  acao_matrizTransicaoEstados[UPLOAD][ERRO] = A06;

  proximo_estado_matrizTransicaoEstados[LEITURA][AUSENCIA] = ESPERA;
  acao_matrizTransicaoEstados[LEITURA][AUSENCIA] = A02;
  acao_matrizTransicaoEstados[UPLOAD ][AUSENCIA] = A02;

}


int obterAcao(int estado, int codigoEvento) {
  return acao_matrizTransicaoEstados[estado][codigoEvento];
}


int obterProximoEstado(int estado, int codigoEvento) {
  return proximo_estado_matrizTransicaoEstados[estado][codigoEvento];
}


void MaqEstados() {
  if (eventoInterno == NENHUM_EVENTO) {
      obterEvento();
  } else {
      codigoEvento = eventoInterno;
  }
  if (codigoEvento != NENHUM_EVENTO)
  {
      codigoAcao = obterAcao(estado, codigoEvento);
      estado = obterProximoEstado(estado, codigoEvento);
      eventoInterno = executarAcao(codigoAcao);
  }
}


int obterEvento() {

  codigoEvento = NENHUM_EVENTO;
    if (leitor.completouCodigo()) {
    codigoEvento = CODIGO;
    return;
  }
  if (ultra.algoProximo()) {
    codigoEvento = PRESENCA;
    return;
  }
  if (tampa.passouDelay()) {
    codigoEvento = AUSENCIA;
    return;
  }

  return;
}

void iniciaSistema()
{
   iniciaMaquinaEstados();
   estado = SETUP;
   eventoInterno = TENTAR_CONEXAO;
   tentativas_conexao = 0;
} 

void piscaLeds() {
  ledVerde.toggle();
  if (ledVerde.estaAceso() == ledVermelho.estaAceso())
    ledVermelho.toggle();
}

void piscaVerde() {
  ledVerde.toggle();
}

void piscaVermelho() {
  ledVermelho.toggle();
}

void fimDaTaskLeds(){
  ledVerde.desligar();
  ledVermelho.desligar();
}

void inicioVazio(){
  ;
}

void setup() {

  leitor.setup();
  ultra.setup();
  tampa.setup();
  ledVerde.setup();
  ledVermelho.setup();
  esp8266.setup();

  idxTaskPiscaLeds = TaskController.createTask(&piscaLeds, 500, 0, false, &inicioVazio, &fimDaTaskLeds);
  idxTaskPiscaVerde = TaskController.createTask(&piscaVerde, 2000, 0, false, &inicioVazio, &fimDaTaskLeds);
  idxTaskPiscaVermelho = TaskController.createTask(&piscaVermelho, 2000, 0, false, &inicioVazio, &fimDaTaskLeds);

  TaskController.begin(1000); // tick @1ms (1000 us)
  iniciaSistema();

}

void loop() {
  MaqEstados();
}
