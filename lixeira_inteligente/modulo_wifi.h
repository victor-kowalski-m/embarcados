#ifndef MODULO_WIFI_H_INCLUDED
#define MODULO_WIFI_H_INCLUDED

class ModuloWifi {
  public:
    void setup();
    boolean conectaRede();
    boolean conectaServer();
    void fazRequest(char codigoDeBarras[]);
};

#endif