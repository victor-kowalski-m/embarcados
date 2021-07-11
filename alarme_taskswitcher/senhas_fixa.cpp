#include <Arduino.h>

#include "definicoes_sistema.h"
#include "senhas_fixa.h"

SenhasFixa::SenhasFixa()
{
}

/************************
 SenhasFixa::validar
 Verifica se senha esta cadastrada
 entradas
   texto : texto a ser validado como senha
 saidas
   TRUE:senha valida
   FALSE:senha invalida
*************************/
int SenhasFixa::validar(char* texto)
{
    if (texto[0] == '1' && texto[1] == '2')
        return true;
    else
        return false;
}
