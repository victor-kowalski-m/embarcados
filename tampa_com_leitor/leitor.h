#ifndef LEITOR_H_INCLUDED
#define LEITOR_H_INCLUDED

#include <hidboot.h>
#include <usbhub.h>
#include <Arduino.h>

class Leitor : public KeyboardReportParser{
  public:
    Leitor(char *_codigoDeBarras);
    void PrintKey(byte mod, byte key);
    void setup();
    void ler();  
    bool completouCodigo();
    // char fimDoCodigo();
    void resetar();

  private:
    bool leituraRealizada;
    char *codigoDeBarras;
    char idx_codBar;

    void OnKeyDown  (byte mod, byte key);
    void OnKeyUp  (byte mod, byte key);
    void OnKeyPressed(byte key);

};

#endif
