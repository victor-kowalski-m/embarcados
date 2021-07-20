#ifndef LEITOR_H_INCLUDED
#define LEITOR_H_INCLUDED

class Leitor {
  public:   
    virtual void setup();
    virtual void ler();  
    virtual bool completouCodigo();
    virtual void resetar();
};

#endif
