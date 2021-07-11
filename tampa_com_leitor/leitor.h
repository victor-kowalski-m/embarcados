#ifndef LEITOR_H_INCLUDED
#define LEITOR_H_INCLUDED

#include <hidboot.h>
#include <usbhub.h>
#include <Arduino.h>

class Leitor : public KeyboardReportParser{
  public:
    Leitor();
    void PrintKey(byte mod, byte key);
    void setup();
    void ler();  
    bool completouCodigo();
    String retornarCodigo();
    void resetar();

  private:
    bool leituraRealizada;
    String codigoLido;

    void OnKeyDown  (byte mod, byte key);
    void OnKeyUp  (byte mod, byte key);
    void OnKeyPressed(byte key);

};

#endif

