#ifndef ESP8266_H_INCLUDED
#define ESP8266_H_INCLUDED

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "definicoes_sistema.h"

#define CWMODE '1'
#define CIPMUX '1'

boolean read_until_ESP(const char keyword1[], int key_size, int timeout_val, byte mode);
void timeout_start();
boolean timeout_check(int timeout_ms);
void serial_dump_ESP();
boolean connect_ESP();

extern char resposta_site[2];
extern SoftwareSerial ESP8266;
extern void connect_webhost(char codigoDeBarras[]);

#endif