#include "definicoes_sistema.h"
#include "leitor.h"
#include "ultra.h"
#include "tampa.h"

// DECLARAÇÃO DE VARIÁVEIS GLOBAIS
long t0_tampa = 0;
long t1_tampa = 0;
int estado_tampa = 0;

String codigo;

// Componentes;
Leitor leitor;
Ultra ultra(ECHO_PIN, TRIGGER_PIN, DIST_ATIVA_ULTRA);
Tampa tampa(SERVO_TAMPA, TAMPA_ABERTA, TAMPA_FECHADA, DELAY_TAMPA); 

void setup() {
  Serial.begin(9600);
  leitor.setup();
  ultra.setup();
  tampa.setup();
}

void loop() {
  leitor.ler();
  if(leitor.completouCodigo()){
    codigo = leitor.retornarCodigo();
    Serial.print("Código lido: ");
    Serial.println(codigo);
    leitor.resetar();
  }

  if (ultra.algoProximo()) {
        tampa.abre();
    } else {
        if (tampa.estaAberta()) {
           if (tampa.passouDelay(millis())){
              tampa.fecha();
           }
        }
    }
}
