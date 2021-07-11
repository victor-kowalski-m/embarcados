#include "leitor.h"

USB Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);

Leitor::Leitor(){
  codigoLido = "";
  leituraRealizada = false;
}

void Leitor::PrintKey(byte m, byte key){
  MODIFIERKEYS mod;
  *((byte*)&mod) = m;
}

void Leitor::setup(){
  Serial.println("Iniciando USB Host Shield");
  if (Usb.Init() == -1) {
    Serial.println("Falha ao iniciar USB");
  }
  delay( 200 );

  Serial.println("Iniciando Leitor de Código de Barras");
  HidKeyboard.SetReportParser(0, this);
  Serial.println("Setup concluído");
  
}

void Leitor::ler(){
  Usb.Task();
}

bool Leitor::completouCodigo(){
  return leituraRealizada;
}

String Leitor::retornarCodigo(){
    return codigoLido;
}

void Leitor::resetar(){
    codigoLido = "";
    leituraRealizada = false;
}

void Leitor::OnKeyDown(byte mod, byte key){
  PrintKey(mod, key);
  byte c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void Leitor::OnKeyUp(byte mod, byte key){
  PrintKey(mod, key);
}

void Leitor::OnKeyPressed(byte key){
  if (!leituraRealizada){
    if(key == 19){
      leituraRealizada = true;
    } else {
      codigoLido += (char)key;  
    }
  }
}


