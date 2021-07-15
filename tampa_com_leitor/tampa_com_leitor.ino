/*
    LIXEIRA INTELIGENTE
*/

#include "definicoes_sistema.h"
#include "leitor.h"
#include "ultra.h"
#include "tampa.h"
#include "led.h"
#include "modulo_wifi.h"

/***********************************************************************
 Componentes
 ***********************************************************************/
Leitor leitor;
Ultra ultra(ECHO_PIN, TRIGGER_PIN, DIST_ATIVA_ULTRA);
Tampa tampa(SERVO_TAMPA, TAMPA_ABERTA, TAMPA_FECHADA, DELAY_TAMPA);
Led ledVerde(LED_VERDE);
Led ledVermelho(LED_VERMELHO);

/***********************************************************************
 Estaticos
 ***********************************************************************/
const int num_leds = 2;
Led leds[] = {ledVerde, ledVermelho};
int codigoEvento = NENHUM_EVENTO;
int eventoInterno = NENHUM_EVENTO;
int estado = ESPERA;
int codigoAcao;
int acao_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
int proximo_estado_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
long t0_tampa = 0;
long t1_tampa = 0;
int estado_tampa = 0;
String codigoDeBarras;

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
        codigoDeBarras = leitor.retornarCodigo();
        Serial.print("Código lido: ");
        Serial.println(codigoDeBarras);
        ledVerde.ligar();
        ledVermelho.ligar();
        leitor.resetar();
        break;
    case A04:
        // ignora código lido
        leitor.resetar();
        break;
    case A05:
        // confirma upload
        ledVermelho.desligar();
        ledVerde.ligar();
        break;
    case A06:
        // erro no upload
        ledVermelho.ligar();
        ledVerde.desligar();
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

  proximo_estado_matrizTransicaoEstados[LEITURA][CODIGO] = UPLOAD;
  acao_matrizTransicaoEstados[LEITURA][CODIGO] = A03;
  acao_matrizTransicaoEstados[UPLOAD ][CODIGO] = A04;
  acao_matrizTransicaoEstados[ESPERA ][CODIGO] = A04;

  proximo_estado_matrizTransicaoEstados[UPLOAD][SUCESSO] = LEITURA;
  acao_matrizTransicaoEstados[UPLOAD][SUCESSO] = A05;

  proximo_estado_matrizTransicaoEstados[UPLOAD][TIMEOUT] = LEITURA;
  acao_matrizTransicaoEstados[UPLOAD][TIMEOUT] = A06;

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
      codigoEvento = obterEvento();
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

  if (ultra.algoProximo()) {
    return PRESENCA;
  }
  if (leitor.completouCodigo()) {
    return CODIGO;
  }
  if (tampa.passouDelay(millis())) {
    return AUSENCIA;
  }
  // if (wifi.sucesso()) {
  //   codigoEvento = CONFIRMAR;
  //   return;
  // }
  // if (tmr.timeout()) {
  //   codigoEvento = TIMEOUT;
  //   return;
  // }

  return NENHUM_EVENTO;
}


void setup() {
  Serial.begin(9600);
  leitor.setup();
  ultra.setup();
  tampa.setup();
  ledVerde.setup();
  ledVermelho.setup();
  configura_wifi();
  iniciaMaquinaEstados();

}

void loop() {
  MaqEstados();
}
