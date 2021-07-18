/*
    ESP
*/

#include <SoftwareSerial.h>
#include "definicoes_sistema.h"
#include "leitor.h"
#include "ultra.h"
#include "tampa.h"
#include "led.h"
#include "task_switcher.h"

//#define _SS_MAX_RX_BUFF 256 
#define ESP8266_rxPin 15
#define ESP8266_txPin 16

//SSID + KEY
const char SSID_ESP[] = "DeMartins";
const char SSID_KEY[] = "loja2512demartins";

// URLs
// String URL_webhost = "GET /lixeira_decrementa/16/";
const char payload_opener[] = "GET /lixeira_decrementa/16/";

//MODES
const char CWMODE = '1';//CWMODE 1=STATION, 2=APMODE, 3=BOTH
const char CIPMUX = '1';//CWMODE 0=Single Connection, 1=Multiple Connections

SoftwareSerial ESP8266(ESP8266_rxPin, ESP8266_txPin);// rx tx
//auto& ESP8266 = Serial;

//DEFINE ALL FUNCTIONS HERE
boolean setup_ESP();
boolean read_until_ESP(const char keyword1[], int key_size, int timeout_val, byte mode);
void timeout_start();
boolean timeout_check(int timeout_ms);
void serial_dump_ESP();
boolean connect_ESP();
void connect_webhost();

//DEFINE ALL GLOBAL VAARIABLES HERE
unsigned long timeout_start_val;
char scratch_data_from_ESP[20];//first byte is the length of bytes
char payload[150];
byte payload_size=0; //, counter=0;
char ip_address[16];
// String URL_withPacket = "";
// String payload_closer = " HTTP/1.0\r\n\r\n";
const char payload_closer[] = " HTTP/1.0\r\n\r\n";

char resposta_site[10];

//DEFINE KEYWORDS HERE
const char keyword_OK[] = "OK";
const char keyword_Ready[] = "Ready";
const char keyword_no_change[] = "no change";
const char keyword_blank[] = "#&";
const char keyword_ip[] = "192.";
const char keyword_rn[] = "\r\n";
const char keyword_quote[] = "\"";
const char keyword_carrot[] = ">";
const char keyword_sendok[] = "SEND OK";
const char keyword_linkdisc[] = "Unlink";
//const char keyword_resposta[] = "Resposta: ";
const char keyword_doublehash[] = "##";


/*
    LIXEIRA INTELIGENTE
*/

/***********************************************************************
 Componentes
 ***********************************************************************/
// Leitor leitor;
Ultra ultra(ECHO_PIN, TRIGGER_PIN, DIST_ATIVA_ULTRA);
Tampa tampa(SERVO_TAMPA, TAMPA_ABERTA, TAMPA_FECHADA, DELAY_TAMPA);
Led ledVerde(LED_VERDE);
Led ledVermelho(LED_VERMELHO);

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
char idxTaskMantemVerde;
char idxTaskMantemVermelho;

// char fimDoCod;

Leitor leitor(codigoDeBarras);

/************************************************************************
 executarAcao
 Executa uma acao
 Parametros de entrada:
    (int) codigo da acao a ser executada
 Retorno: (int) codigo do evento interno ou NENHUM_EVENTO
*************************************************************************/
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
        // codigoDeBarras = leitor.retornarCodigo();
        // fimDoCod = leitor.fimDoCodigo();
        ledVerde.ligar();
        TaskController.ativaTask(idxTaskPiscaLeds, 200, 0);
        connect_webhost();         
        leitor.resetar();
        TaskController.desativaTask(idxTaskPiscaLeds);
        ledVermelho.desligar();
        ledVerde.desligar();      
        if ((resposta_site[1]-48) == DECREMENTOU){
//          ledVerde.ligar();
          retval = SUCESSO;
        } else {
//          ledVermelho.ligar();
          retval = ERRO;
        }
        break;
    case A04:
        // ignora código lido
        leitor.resetar();
        break;
    case A05:
        // confirma upload
//        ledVermelho.desligar();
        ledVerde.desligar();
        idxTaskMantemVerde = TaskController.createTask(&piscaVerde, 5000, 2, true);
        break;
    case A06:
        // erro no upload
        ledVermelho.desligar();
//        ledVerde.desligar();
        idxTaskMantemVermelho = TaskController.createTask(&piscaVermelho, 5000, 2, true);
        break;
    }

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

  proximo_estado_matrizTransicaoEstados[ESPERA][PRESENCA] = LEITURA;
  acao_matrizTransicaoEstados[ESPERA ][PRESENCA] = A01;
  acao_matrizTransicaoEstados[LEITURA][PRESENCA] = A01;
  acao_matrizTransicaoEstados[UPLOAD ][PRESENCA] = A01;

  proximo_estado_matrizTransicaoEstados[LEITURA][CODIGO] = UPLOAD; // UPLOAD;
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

/************************************************************************
 obterEvento
 Obtem um evento, que pode ser da IHM ou do alarme
 Parametros de entrada: nenhum
 Retorno: codigo do evento
*************************************************************************/
int obterEvento() {

  codigoEvento = NENHUM_EVENTO;

  if (ultra.algoProximo()) {
    codigoEvento = PRESENCA;
    return;
  }
  if (leitor.completouCodigo()) {
    codigoEvento = CODIGO;
    return;
  }
  if (tampa.passouDelay()) {
    codigoEvento = AUSENCIA;
    return;
  }
  // if (wifi.sucesso()) {
  //   codigoEvento = CONFIRMAR;
  //   return;
  // }
  // if (tmr.timeout()) {
  //   codigoEvento = TIMEOUT;
  //   return;
  // }

  return;
}


//void taskMaqEstados() {
//  if (eventoInterno != NENHUM_EVENTO) {
//      codigoEvento = eventoInterno;
//  }
//  if (codigoEvento != NENHUM_EVENTO)
//  {
//      codigoAcao = obterAcao(estado, codigoEvento);
//      estado = obterProximoEstado(estado, codigoEvento);
//      eventoInterno = executarAcao(codigoAcao);
//  }
//}
//
//void taskObterEvento() {
//  codigoEvento = NENHUM_EVENTO;
//
//  if (ultra.algoProximo()) {
//    codigoEvento = PRESENCA;
//    return;
//  }
//  if (leitor.completouCodigo()) {
//    codigoEvento = CODIGO;
//    return;
//  }
//  if (tampa.passouDelay()) {
//    codigoEvento = AUSENCIA;
//    return;
//  }
//  // if (wifi.sucesso()) {
//  //   codigoEvento = CONFIRMAR;
//  //   return;
//  // }
//  // if (tmr.timeout()) {
//  //   codigoEvento = TIMEOUT;
//  //   return;
//  // }
//}

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
//  ESP8266.listen();//not needed unless using other software serial instances
  //Serial.begin(9600);

  leitor.setup();
  ultra.setup();
  tampa.setup();
  ledVerde.setup();
  ledVermelho.setup();

//  idxTaskPiscaLeds = TaskController.createTask(&taskMaqEstados, 200, 0, true);
//  idxTaskObterEvento =TaskController.createTask(&taskObterEvento, 200, 0, true);
  idxTaskPiscaLeds = TaskController.createTask(&piscaLeds, 500, 0, false);
  idxTaskMantemVerde = TaskController.createTask(&piscaVerde, 2000, 0, false);
  idxTaskMantemVermelho = TaskController.createTask(&piscaVermelho, 2000, 0, false);

  iniciaMaquinaEstados();
  TaskController.begin(1000); // tick @1ms (1000 us)
  
  char tentativas = 0;
  ledVerde.ligar();
  TaskController.ativaTask(idxTaskPiscaLeds, 0, 0);
  while(tentativas++ < 2)
    if(setup_ESP())
      break;
  if (tentativas == 3){
    ledVermelho.ligar();
    while(1);
  } else {
    TaskController.desativaTask(idxTaskPiscaLeds);
    ledVerde.ligar();
    ledVermelho.desligar();
  }
}

void loop() {
  MaqEstados();
}
