#ifndef IHM_H_INCLUDED
#define IHM_H_INCLUDED

#define NENHUMA_TECLA -1

class InterfaceHomemMaquina {
  public:
  /************************
   exibirMsg
  Exibe mensagem no display
  entradas
    texto : texto a ser validado como senha
  saidas
    nenhuma
  *************************/
  virtual void exibirMsg(char* texto) = 0;

  /************************
   obterTecla
  Obtem tecla do teclado
  entradas
    nenhuma
  saidas
    tecla lida do teclado ou NENHUMA_TECLA
  *************************/
  virtual char* obterTeclas() = 0;
};

#endif // IHM_H_INCLUDED
