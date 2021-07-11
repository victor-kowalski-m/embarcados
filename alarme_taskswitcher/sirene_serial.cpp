#include <Arduino.h>

#include "definicoes_sistema.h"
#include "sirene_serial.h"

SireneSerial::SireneSerial() : acionado(false)
{
}

/************************
 SireneSerial::bip
 Aciona momentaneamente a sirene
 entradas
   nenhuma
 saidas
   nenhuma
*************************/
void SireneSerial::bip()
{
    Serial.println("Bip da sirene");
}


/************************
 SireneSerial::acionar
 Aciona ou desaciona a sirene
 entradas
   controle: TRUE:ligar FALSE:desligar
 saidas
   nenhuma
*************************/
void SireneSerial::acionar(int controle)
{
    acionado = controle;
    Serial.print("Acionamento da sirene: ");
    Serial.println(acionado);
}

/************************
 SireneSerial::estacionado
Retorna se está acionado ou não
entradas
  nenhuma
saidas
  acionado: TRUE
  desacionado: FALSE
*************************/
int SireneSerial::estaAcionado()
{
    return acionado;
}
