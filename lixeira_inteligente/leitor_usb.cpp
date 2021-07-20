#include "leitor_usb.h"

USB Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);

LeitorUSB::LeitorUSB(char *_codigoDeBarras){
//  codigoLido = "";
  codigoDeBarras = _codigoDeBarras;
  idx_codBar = 0;
  leituraRealizada = false;
}

void LeitorUSB::PrintKey(byte m, byte key){
  MODIFIERKEYS mod;
  *((byte*)&mod) = m;
}

void LeitorUSB::setup(){
  if (Usb.Init() == -1) {}
  delay( 200 );
  HidKeyboard.SetReportParser(0, this);
  codigoDeBarras[0] = 0;
}

void LeitorUSB::ler(){
  Usb.Task();
}

bool LeitorUSB::completouCodigo(){
  this->ler();
  return leituraRealizada;
}

void LeitorUSB::resetar(){
    idx_codBar = 0;
    leituraRealizada = false;
}

void LeitorUSB::OnKeyDown(byte mod, byte key){
  PrintKey(mod, key);
  byte c = OemToAscii(mod, key);
  if (c)
    OnKeyPressed(c);
}

void LeitorUSB::OnKeyUp(byte mod, byte key){
  PrintKey(mod, key);
}

void LeitorUSB::OnKeyPressed(byte key){
  if (!leituraRealizada){
    if(key == 19){
      codigoDeBarras[idx_codBar++] = 0;
      leituraRealizada = true;
    } else {
      codigoDeBarras[idx_codBar++] = (char)key;  
    }
  }
}
