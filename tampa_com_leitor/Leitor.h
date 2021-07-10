// DECLARAÇÃO DE CLASSES
class KbdRptParser : public KeyboardReportParser{
    void PrintKey(byte mod, byte key);

  private:

    void OnKeyDown  (byte mod, byte key);
    void OnKeyUp  (byte mod, byte key);
    void OnKeyPressed(byte key);
};

// IMPLEMENTO DE MÉTODOS
void KbdRptParser::PrintKey(byte m, byte key){
  MODIFIERKEYS mod;
  *((byte*)&mod) = m;
}

void KbdRptParser::OnKeyDown(byte mod, byte key){
  PrintKey(mod, key);
  byte c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnKeyUp(byte mod, byte key){
  PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(byte key){
  
  if(key == 19){
    Serial.println("Leitura finalizada");
    leituraRealizada = true;
  }else{
    codigoLido += (char)key;  
  }
  
}

// INSTANCIANDO OBJETOS
USB     Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
KbdRptParser Prs;
