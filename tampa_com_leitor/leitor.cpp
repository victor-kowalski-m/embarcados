#include "leitor.h"

USB Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);

Leitor::Leitor(char *_codigoDeBarras){
//  codigoLido = "";
  codigoDeBarras = _codigoDeBarras;
  idx_codBar = 0;
  leituraRealizada = false;
}

void Leitor::PrintKey(byte m, byte key){
  MODIFIERKEYS mod;
  *((byte*)&mod) = m;
}

void Leitor::setup(){
  // //##Serial.println("Iniciando USB Host Shield");
  if (Usb.Init() == -1) {
    // //##Serial.println("Falha ao iniciar USB");
  }
  delay( 200 );

  // //##Serial.println("Iniciando Leitor de Código de Barras");
  HidKeyboard.SetReportParser(0, this);
  // //##Serial.println("Setup concluído");

  codigoDeBarras[0] = 0;
  
}

void Leitor::ler(){
  Usb.Task();
}

bool Leitor::completouCodigo(){
  this->ler();
  return leituraRealizada;
}

void Leitor::resetar(){
    idx_codBar = 0;
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
      codigoDeBarras[idx_codBar++] = 0;
      leituraRealizada = true;
    } else {
      codigoDeBarras[idx_codBar++] = (char)key;  
    }
  }
}
