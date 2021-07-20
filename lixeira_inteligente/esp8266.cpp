#include "esp8266.h"

const char payload_opener[] = REQUEST_SITE;
const char payload_closer[] = " HTTP/1.0\r\n\r\n";
const char keyword_OK[] = "OK";
const char keyword_Ready[] = "Ready";
const char keyword_no_change[] = "no change";
const char keyword_blank[] = "#&";
const char keyword_ip[] = "192.";
const char keyword_rn[] = "\r\n";
const char keyword_quote[] = "\"";
const char keyword_carrot[] = ">";
const char keyword_sendok[] = "SEND OK";
const char keyword_linkdisc[] = "Unlink";
const char keyword_doublehash[] = "##";

ESP8266::ESP8266(int _RX_PIN, int _TX_PIN):Serial(_RX_PIN, _TX_PIN), payload_size(0), RX_PIN(_RX_PIN), TX_PIN(_TX_PIN){}

void ESP8266::setup(){
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  Serial.begin(9600);
}

boolean ESP8266::conectaRede(){
  
  Serial.print("AT\r\n");
  if(this->buscaTrecho(keyword_OK,sizeof(keyword_OK),5000,0))
    ;//##Serial.println("ESP CHECK OK");
  else
    ;//##Serial.println("ESP CHECK FAILED");
  this->limpaSerial();

  Serial.print("AT+RST\r\n");
  if(this->buscaTrecho(keyword_Ready,sizeof(keyword_Ready),5000,0))
    ;//##Serial.println("ESP RESET OK");
  else
    ;//##Serial.println("ESP RESET FAILED");
  this->limpaSerial();
  
  Serial.print("AT+CWMODE=");
  Serial.print(CWMODE);
  Serial.print("\r\n");
  if(this->buscaTrecho(keyword_OK,sizeof(keyword_OK),1000,0))
    ;//##Serial.println("ESP CWMODE SET");
  else
    ;//##Serial.println("ESP CWMODE SET FAILED");
  this->limpaSerial();  
   
  Serial.print("AT+CWJAP=\"");
  Serial.print(SSID);
  Serial.print("\",\"");
  Serial.print(SENHA);
  Serial.print("\"\r\n");
  if(this->buscaTrecho(keyword_OK,sizeof(keyword_OK),10000,0))
    ;//##Serial.println("ESP SSID SET OK");
  else
    ;//##Serial.println("ESP SSID SET FAILED");  
  this->limpaSerial();
  
  Serial.print("AT+CIFSR\r\n");
  if(this->buscaTrecho(keyword_rn,sizeof(keyword_rn),10000,0)){
    if(this->buscaTrecho(keyword_rn,sizeof(keyword_rn),1000,1)){
      for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(keyword_rn)+1); i++)
        ip_address[i] = scratch_data_from_ESP[i];
      ip_address[0] = (scratch_data_from_ESP[0]-sizeof(keyword_rn)+1);
      //##Serial.print("IP ADDRESS = ");
      for(int i=1; i<=ip_address[0]; i++)
        ;//##Serial.print(ip_address[i]);
      //##Serial.println("");
    }
  }
  else
    ;//##Serial.print("IP ADDRESS FAIL");
  this->limpaSerial();
  
  Serial.print("AT+CIPMUX=");
  Serial.print(CIPMUX);
  Serial.print("\r\n");
  if(this->buscaTrecho(keyword_OK,sizeof(keyword_OK),5000,0)){
    ;//##Serial.println("ESP CIPMUX SET");
  } else {
    //##Serial.println("ESP CIPMUX SET FAILED");
    return false;
  } 
  this->limpaSerial();
  return true;
   
}


void ESP8266::limpaSerial(){
  char temp;
  while(Serial.available()){
    temp =Serial.read();
    delay(1);
  }
  
}


boolean ESP8266::buscaTrecho(const char keyword1[], int key_size, int timeout_val, byte mode){
  timeout_start_val=millis();
  char data_in[20];
  int scratch_length=1;
  key_size--;
 

  for(byte i=0; i<key_size; i++){
    while(!Serial.available()){
      if((millis()-timeout_start_val)>timeout_val){
        //##Serial.println("timeout");
        return 0;
      }
    }
   
    data_in[i]=Serial.read();
    if(mode==1){
      scratch_data_from_ESP[scratch_length]=data_in[i];
      scratch_data_from_ESP[0]=scratch_length;
      scratch_length++;
    }
    
  }
  
  while(1){

     for(byte i=0; i<key_size; i++){
        if(keyword1[i]!=data_in[i])
          break;
        if(i==(key_size-1)){
          return 1;
        }
     }
     
    for(byte i=0; i<(key_size-1); i++){
      data_in[i]=data_in[i+1];
    }
         
    while(!Serial.available()){
      if((millis()-timeout_start_val)>timeout_val){
        //##Serial.println("timeout");
        return 0;
      }
    }
    
    data_in[key_size-1]=Serial.read();
      if(mode==1){
      scratch_data_from_ESP[scratch_length]=data_in[key_size-1];
      scratch_data_from_ESP[0]=scratch_length;
      scratch_length++;
    }     
  }
 
}


boolean ESP8266::conectaServer(){

  //##Serial.println("CONNECTING");
  Serial.print("AT+CIPSTART=0,\"TCP\",\"");
  Serial.print(IP_SERVER);
  Serial.print("\",8080\r\n"); 
  if(this->buscaTrecho(keyword_OK,sizeof(keyword_OK),5000,0)){
    this->limpaSerial();
    //##Serial.println("CONNECTED");
    Serial.print("AT+CIPSEND=0,");
    Serial.print(payload_size);
    this->limpaSerial();
    Serial.print("\r\n");
    
    if(this->buscaTrecho(keyword_carrot,sizeof(keyword_carrot),5000,0)){
      //##Serial.println("READY TO SEND");
      for(int i=0; i<payload_size; i++)
        Serial.print(payload[i]);
      
      if(this->buscaTrecho(keyword_sendok,sizeof(keyword_sendok),5000,0)){
        //##Serial.println("SENT");
        return 1;
      }
      else
        ;//##Serial.println("FAILED TO SEND");       
    }
    else
      ;//##Serial.println("FAILED TO GET >");
  }
  else{
    //##Serial.println("FAILED TO CONNECT");
    this->conectaRede();
  }

}


void ESP8266::fazRequest(char codigoDeBarras[]){

  char i;
  payload_size=0;
  for(i=0; payload_opener[i]!=0; i++){
    payload[payload_size++] = payload_opener[i];
  }
  for(i=0; codigoDeBarras[i]!=0; i++){
    payload[payload_size++] = codigoDeBarras[i];
  }
  for(i=0; payload_closer[i]!=0; i++){
    payload[payload_size++] = payload_closer[i]; 
  }
   
  if(this->conectaServer()){

    if(this->buscaTrecho(keyword_doublehash,sizeof(keyword_doublehash),5000,0)){
      if(this->buscaTrecho(keyword_doublehash,sizeof(keyword_doublehash),5000,1)){
        //got our data, so quickly store it away in d1
        for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(keyword_doublehash)+1); i++)
          resposta_site[i] = scratch_data_from_ESP[i];
        resposta_site[0] = (scratch_data_from_ESP[0]-sizeof(keyword_doublehash)+1);
          
        if(this->buscaTrecho(keyword_linkdisc,sizeof(keyword_linkdisc),5000,0)){
          //##Serial.println("FOUND DATA & DISCONNECTED");
          this->limpaSerial();
          
          //##Serial.print("Resposta: ");
          for(int i=1; i<=resposta_site[0]; i++){
          //##Serial.print(resposta_site[i]);
          }     
          //##Serial.println("");

        } 
      }
    }
  }
}
