#ifndef FEEDBACK_USUARIO_H_INCLUDED
#define FEEDBACK_USUARIO_H_INCLUDED

class FeedbackUsuario {
  public:
    virtual void setup();
    virtual void ligar();
    virtual void desligar();
    virtual void toggle();
    virtual bool estaLigado();
};

#endif