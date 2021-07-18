#ifndef DEFINICOES_SISTEMA_H_INCLUDED
#define DEFINICOES_SISTEMA_H_INCLUDED

#define MAX_TAREFAS 5

// RESPOSTAS DA WEB
#define SEM_RESPOSTA 0
#define DECREMENTOU 1
#define COD_INEXISTENTE 2
#define DESPENSA_SEM_PROD 3

// PINOS
#define TRIGGER_PIN      2
#define ECHO_PIN         3  
#define LED_VERDE        4
#define LED_VERMELHO     5
#define SERVO_TAMPA      19
#define TX_COM_ESP       8
#define RX_COM_ESP       9
#define ESP8266_rxPin    15
#define ESP8266_txPin    16

// PARAMETROS NUMERICOS
#define TAMPA_ABERTA     90
#define TAMPA_FECHADA    180
#define DELAY_TAMPA      2000
#define DIST_ATIVA_ULTRA 20

// MAQUINA DE ESTADOS
#define NUM_ESTADOS 3
#define NUM_EVENTOS 7

// ESTADOS
#define ESPERA   0
#define LEITURA  1
#define UPLOAD   2

// EVENTOS
#define NENHUM_EVENTO -1
#define PRESENCA       0
#define CODIGO         1
#define SUCESSO        2
#define ERRO           3
#define AUSENCIA       4

// ACOES
#define NENHUMA_ACAO -1
#define A01           0
#define A02           1
#define A03           2
#define A04           3
#define A05           4
#define A06           5
#define A07           6

#endif
