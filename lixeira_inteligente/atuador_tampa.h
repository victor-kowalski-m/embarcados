#ifndef ATUADOR_TAMPA_INCLUDED
#define ATUADOR_TAMPA_INCLUDED

class AtuadorTampa {
  public:
    virtual void setup();
    virtual void abrir();
    virtual void fechar();
    virtual bool estaAberta();
    virtual bool passouDelay();
};

#endif