/*
    LIXEIRA INTELIGENTE
*/

#include "leitor.h"
#include "ultra.h"
#include "tampa.h"
#include "led.h"
#include "task_switcher.h"
#include "esp8266.h"

char resposta_site[2];
SoftwareSerial ESP8266(ESP8266_rxPin, ESP8266_txPin); // rx tx
// AltSoftSerial ESP8266;
// HardwareSerial& ESP8266  = Serial;
boolean setup_ESP();
void connect_webhost(char codigoDeBarras[]);

/***********************************************************************
 Estaticos
 ***********************************************************************/
int codigoEvento = NENHUM_EVENTO;
int eventoInterno = NENHUM_EVENTO;
int estado = ESPERA;
int codigoAcao;
int acao_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
int proximo_estado_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
char codigoDeBarras[20];

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


int executarAcao(int codigoAcao) {
    
    int retval = NENHUM_EVENTO;
    if (codigoAcao == NENHUMA_ACAO)
        return retval;

    switch(codigoAcao)
    {
    case A01:
        tampa.abrir();
        break;
    case A02:
        tampa.fechar();
        break;
    case A03:
        // inicia upload
        leitor.resetar();
        ledVerde.ligar(); 
        TaskController.ativaTask(idxTaskPiscaLeds, 200, 0);
        tampa.detach();
        connect_webhost(codigoDeBarras);
        tampa.attach(SERVO_TAMPA);         
        TaskController.desativaTask(idxTaskPiscaLeds);
        ledVermelho.desligar();
        ledVerde.desligar();      
        if ((resposta_site[1]-48) == DECREMENTOU){
          retval = SUCESSO;
        } else {
          retval = ERRO;
        }
        break;
    case A04:
        // ignora código lido
        leitor.resetar();
        break;
    case A05:
        // confirma upload
        TaskController.ativaTask(idxTaskPiscaVerde, 100, 20);
        break;
    case A06:
        // erro no upload
        TaskController.ativaTask(idxTaskPiscaVermelho, 100, 20);
        break;
    }

    return retval;
} // executarAcao


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
} // obterAcao


int obterProximoEstado(int estado, int codigoEvento) {
  return proximo_estado_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao


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


// void taskMaqEstados() {
//   if (eventoInterno != NENHUM_EVENTO) {
//       codigoEvento = eventoInterno;
//   }
//   if (codigoEvento != NENHUM_EVENTO)
//   {
//       codigoAcao = obterAcao(estado, codigoEvento);
//       estado = obterProximoEstado(estado, codigoEvento);
//       eventoInterno = executarAcao(codigoAcao);
//   }
// }

// void taskObterEvento() {
//   codigoEvento = NENHUM_EVENTO;

//   if (ultra.algoProximo()) {
//     codigoEvento = PRESENCA;
//     return;
//   }
//   if (leitor.completouCodigo()) {
//     codigoEvento = CODIGO;
//     return;
//   }
//   if (tampa.passouDelay()) {
//     codigoEvento = AUSENCIA;
//     return;
//   }
// }

void piscaLeds() {
  ledVerde.toggle();
  ledVermelho.toggle();
}

void piscaVerde() {
  ledVerde.toggle();
}

void piscaVermelho() {
  ledVermelho.toggle();
}

void setup() {
  pinMode(ESP8266_rxPin, INPUT);
  pinMode(ESP8266_txPin, OUTPUT);
  ESP8266.begin(9600);//default baudrate for ESP

  leitor.setup();
  ultra.setup();
  tampa.setup();
  ledVerde.setup();
  ledVermelho.setup();

//  idxTaskMaqEstados = TaskController.createTask(&taskMaqEstados, 200, 0, true);
//  idxTaskObterEvento =TaskController.createTask(&taskObterEvento, 200, 0, true);
  idxTaskPiscaLeds = TaskController.createTask(&piscaLeds, 500, 0, false);
  idxTaskPiscaVerde = TaskController.createTask(&piscaVerde, 2000, 0, false);
  idxTaskPiscaVermelho = TaskController.createTask(&piscaVermelho, 2000, 0, false);

  iniciaMaquinaEstados();
  TaskController.begin(1000); // tick @1ms (1000 us)
  
  char tentativas = 0;
  ledVerde.ligar();
  TaskController.ativaTask(idxTaskPiscaLeds, 0, 0);
  // tampa.abrir();
  // delay(500);
  tampa.detach();
  while(tentativas++ < 2)
    if(setup_ESP())
      break;
  if (tentativas == 3){
    TaskController.desativaTask(idxTaskPiscaLeds);
    ledVermelho.ligar();
    while(1);
  } else {
    tampa.attach(SERVO_TAMPA);
    TaskController.desativaTask(idxTaskPiscaLeds);
    ledVerde.ligar();
    ledVermelho.desligar();
  }
}

void loop() {
  MaqEstados();
}
