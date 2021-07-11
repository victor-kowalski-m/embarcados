#ifndef DEFINICOES_SISTEMA_H_INCLUDED
#define DEFINICOES_SISTEMA_H_INCLUDED

#define MAX_TAREFAS 3

// PINOS
#define BOTAO            9
#define LED_VERDE        2
#define LED_AMARELO      8
#define SERVO_TAMPA      3
#define TAMPA_ABERTA     30
#define TAMPA_FECHADA    180
#define DELAY_TAMPA      2000
#define ECHO_PIN         4  
#define TRIGGER_PIN      5
#define DIST_ATIVA_ULTRA 5

// MAQUINA DE ESTADOS
#define NUM_ESTADOS 3
#define NUM_EVENTOS 7

// ESTADOS
#define ESPERA   0
#define LEITURA    1
#define UPLOAD   2

// EVENTOS
#define NENHUM_EVENTO -1
#define APROXIMAR      0
#define LER            1
#define ENVIAR         2
#define CONFIRMAR      3
#define AFASTAR        4
#define TIMEOUT        5

// ACOES
#define NENHUMA_ACAO -1
#define A01  0
#define A02  1
#define A03  2
#define A04  3
#define A05  4
#define A06  5
#define A07  6

#endif // DEFINICOES_SISTEMA_H_INCLUDED
