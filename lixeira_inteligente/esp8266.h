#ifndef ESP8266_H_INCLUDED
#define ESP8266_H_INCLUDED

#include "definicoes_sistema.h"
#include "modulo_wifi.h"
#include <SoftwareSerial.h>

#define CWMODE '1'
#define CIPMUX '1'

class ESP8266: public ModuloWifi {
  public:
    ESP8266(int RX_PIN, int TX_PIN);
    void setup();
    boolean conectaRede();
    boolean buscaTrecho(const char keyword1[], int key_size, int timeout_val, byte mode);
    void limpaSerial();
    boolean conectaServer();
    void fazRequest(char codigoDeBarras[]);

    SoftwareSerial Serial;

  private:
    int RX_PIN;
    int TX_PIN;
    unsigned long timeout_start_val;
    char scratch_data_from_ESP[20];
    char payload[150];
    byte payload_size;
    char ip_address[16]; 

};

extern char resposta_site[2];

#endif