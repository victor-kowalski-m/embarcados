#ifndef DEFINICOES_SISTEMA_H_INCLUDED
#define DEFINICOES_SISTEMA_H_INCLUDED

#define true  1
#define false 0

#define MAX_TAREFAS 3

#define NUM_ESTADOS 5
#define NUM_EVENTOS 4

// ESTADOS
#define ESPERA   0
#define SAIDA    1
#define ALERTA   2
#define ENTRADA  3
#define ACIONADO 4

// EVENTOS
#define NENHUM_EVENTO -1
#define ACIONAR        0
#define TIMEOUT        1
#define DISPARAR       2
#define DESACIONAR     3

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
