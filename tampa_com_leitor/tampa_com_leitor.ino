// INCLUSÃO DE BIBLIOTECAS
#include <hidboot.h>
#include <usbhub.h>
#include <SPI.h>
# include <Servo.h>

// DEFINIÇÕES
#define PINO_BOTAO 9
#define PINO_LED_VERDE 2
#define PINO_LED_AMARELO 8
#define PINO_SERVO1 3
#define ABERTO_SERVO1 75
#define FECHADO_SERVO1 180
//#define APERTA_SERVO2 180
//#define SOLTA_SERVO2 0
//#define PINO_SERVO2 7
#define ECHO_PIN    4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// DECLARAÇÃO DE VARIÁVEIS GLOBAIS
String codigoLido="";
bool leituraRealizada = false;
int on_off = 1;
long t0_tampa = 0;
long t1_tampa = 0;
int estado_tampa = 0;
long t_leitor = 0;
int codigo_recebido = 0;
int estado_leitor = 0;
Servo servo1;
//Servo servo2;

// INCLUSÃO DOS DEMAIS ARQUIVOS
#include "Leitor.h"

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

// ***************** INÍCIO DO SETUP *************************
void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando USB Host Shield");

  if (Usb.Init() == -1) {
    Serial.println("Algo deu errado :( ");
    Serial.println("Confira o Sketch");
  }
  delay( 200 );

  // CONFIGURA O OBJETO DO LEITOR DE CODIGO DE BARRAS
  Serial.println("Iniciando Leitor de Código de Barras");
  HidKeyboard.SetReportParser(0, &Prs);

  Serial.println("Setup concluído");

  pinMode(PINO_BOTAO, INPUT); // configura o pino com o botão 1 como entrada
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_AMARELO, OUTPUT);
  servo1.attach(PINO_SERVO1);
  servo1.write(FECHADO_SERVO1);
//  servo2.attach(PINO_SERVO2);
//  servo2.write(SOLTA_SERVO2);
  
}
// ***************** FIM DO SETUP ***************************

// ***************** INÍCIO DO LOOP *************************
void loop() {
  // REALIZA A LEITURA DA PORTA USB DA SHIELD
  Usb.Task();

  // EXECUTA AS FUNÇÕES APÓS TER IDENTIFICADO UMA LEITURA COMPLETA
  if(leituraRealizada){
      
    Serial.print("Código lido: ");
    Serial.println(codigoLido);

    codigo_recebido = 1;
    t_leitor = millis();
   
    leituraRealizada = false;
    codigoLido = "";
  }

    if (digitalRead(PINO_BOTAO)) {
      codigo_recebido = 0;
      for (int i=0; i<5; i++){
        digitalWrite(PINO_LED_AMARELO, HIGH);
        delay(100);
        digitalWrite(PINO_LED_AMARELO, LOW);
        delay(100);
      }
    } 

  if (on_off && ((0.01723 * readUltrasonicDistance(TRIGGER_PIN, ECHO_PIN) < 20))) {
        servo1.write(ABERTO_SERVO1);
        digitalWrite(PINO_LED_VERDE, HIGH);
//        servo2.write(APERTA_SERVO2);
        t0_tampa = millis();
//        if (!estado_tampa)
//          t_servo2 = millis();
        estado_tampa = 1;
    } else {
        if (estado_tampa) {
           t1_tampa = millis();
           if ((t1_tampa - t0_tampa) > 3000){
              servo1.write(FECHADO_SERVO1);
//              servo2.write(SOLTA_SERVO2);
              digitalWrite(PINO_LED_VERDE, LOW);
  //            tone(PINO_BUZZER, 100, 500);
              estado_tampa = 0;   
           }
        }
    }
    if (codigo_recebido) {
        if ((millis()-t_leitor> 500)){
          if (estado_leitor){
            digitalWrite(PINO_LED_AMARELO, HIGH);
          }
          else{
            digitalWrite(PINO_LED_AMARELO, LOW);
          }
          estado_leitor = !estado_leitor;
          t_leitor = millis();               
      }
    }

}
// ***************** FIM DO LOOP ***************************
