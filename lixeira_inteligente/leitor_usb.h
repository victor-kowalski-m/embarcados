#ifndef LEITOR_USB_H_INCLUDED
#define LEITOR_USB_H_INCLUDED

#include "definicoes_sistema.h"
#include "leitor.h"
#include <hidboot.h>
#include <usbhub.h>

class LeitorUSB : public Leitor, public KeyboardReportParser{
  public:
    LeitorUSB(char *_codigoDeBarras);
    void PrintKey(byte mod, byte key);
    void setup();
    void ler();  
    bool completouCodigo();
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
