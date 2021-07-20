#ifndef MODULO_WIFI_H_INCLUDED
#define MODULO_WIFI_H_INCLUDED

class ModuloWifi {
  public:
    void setup();
    bool conectaRede();
    bool conectaServer();
    bool fazRequest(char codigoDeBarras[]);
};

#endif