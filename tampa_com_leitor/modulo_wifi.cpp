#include <Arduino.h>
#include "modulo_wifi.h"

#include <SoftwareSerial.h>
SoftwareSerial serial_do_esp(8, 9);

void configura_wifi(){
  
  String SSIDstring = ("\"DeMartins\"");
  String PASSstring = ("\"loja2512demartins\"");


  serial_do_esp.begin(9600);
  sendData("AT+RST\r\n", 2000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWJAP=" + SSIDstring + "," + PASSstring+"\r\n", 2000, DEBUG);
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  // Mostra o endereco IP
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}

void envia_codigo(String codigo){
    if (serial_do_esp.available())
  {
    if (serial_do_esp.find("+IPD,"))
    {
      delay(300);
      int connectionId = serial_do_esp.read() - 48;
 
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head><h1><u>ESP8266 - Web Server</u></h1><h2>";
      webpage += "Código lido: ";
      webpage += codigo;
      webpage += "</h2>";
 
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 3000, DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  serial_do_esp.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (serial_do_esp.available())
    {
      // The esp has data so display its output to the serial window
      char c = serial_do_esp.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
