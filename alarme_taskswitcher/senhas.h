#ifndef SENHAS_H_INCLUDED
#define SENHAS_H_INCLUDED

class Senhas {
  public:
  /************************
   validar
  Verifica se senha esta cadastrada
  entradas
    texto : texto a ser validado como senha
  saidas
    TRUE:senha valida
    FALSE:senha invalida
  *************************/
  virtual int validar(char* texto) = 0;
};


#endif // SENHAS_H_INCLUDED
