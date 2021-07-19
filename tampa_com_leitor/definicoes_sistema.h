#ifndef DEFINICOES_SISTEMA_H_INCLUDED
#define DEFINICOES_SISTEMA_H_INCLUDED

#define MAX_TAREFAS 5

// CONFIGURAÇÕES DE REDE
#define SSID "DeMartins"
#define SENHA "loja2512demartins"
#define IP_SERVER "192.168.0.10"
#define REQUEST_SITE "GET /lixeira_decrementa/16/"

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
#define TAMPA_ABERTA     70
#define TAMPA_FECHADA    180
#define DELAY_TAMPA      2000
#define DIST_ATIVA_ULTRA 20

// MAQUINA DE ESTADOS
#define NUM_ESTADOS 5
#define NUM_EVENTOS 8

// ESTADOS
#define SETUP        0
#define DESCONECTADO 1
#define ESPERA       2
#define LEITURA      3
#define UPLOAD       4

// EVENTOS
#define NENHUM_EVENTO -1
#define TENTAR_CONEXAO 0
#define PRESENCA       1
#define CODIGO         2
#define SUCESSO        3
#define ERRO           4
#define AUSENCIA       5
#define SEM_INTERNET   6

// ACOES
#define NENHUMA_ACAO -1
#define A01           0
#define A02           1
#define A03           2
#define A04           3
#define A05           4
#define A06           5
#define A07           6
#define A08           7
#define A09           8

#endif
