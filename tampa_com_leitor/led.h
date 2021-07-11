#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

class Led {
  public:
    Led(int _pin);
    void setup();
    void ligar();
    void desligar();
    void toggle();
    bool estaAceso();
    long ultimoToggle();

  private:
    int pin;
    bool aceso = false;
    long instanteToggle = millis();

};

#endif