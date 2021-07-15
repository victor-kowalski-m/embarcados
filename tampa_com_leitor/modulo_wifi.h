#ifndef WIFI_H_INCLUDED
#define WIFI_H_INCLUDED

#define DEBUG true

void configura_wifi();
void envia_codigo(String codigo);
String sendData(String command, const int timeout, boolean debug);


#endif
