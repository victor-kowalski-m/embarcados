boolean setup_ESP(){//returns a '1' if successful
  
  ESP8266.print("AT\r\n");// Send just 'AT' to make sure the ESP is responding
  //this read_until_... function is used to find a keyword in the ESP response - more on this later and in the function itself
  if(read_until_ESP(keyword_OK,sizeof(keyword_OK),5000,0))//go look for keyword "OK" with a 5sec timeout
    //##Serial.println("ESP CHECK OK");
    ;
  else
    //##Serial.println("ESP CHECK FAILED");
    ;
  serial_dump_ESP();//this just reads everything in the buffer and what's still coming from the ESP

   ESP8266.print("AT+RST\r\n");// Give it a reset - who knows what condition it was in, better to start fresh
  if(read_until_ESP(keyword_Ready,sizeof(keyword_Ready),5000,0))//go look for keyword "Ready" - takes a few seconds longer to complete
    //##Serial.println("ESP RESET OK");//depneding on the FW version on the ESP, sometimes the Ready is with a lowercase r - ready
    ;
  else
    //##Serial.println("ESP RESET FAILED");
    ; 
  serial_dump_ESP();
  
   ESP8266.print("AT+CWMODE=");// set the CWMODE
   ESP8266.print(CWMODE);//just send what is set in the constant
   ESP8266.print("\r\n");
  if(read_until_ESP(keyword_OK,sizeof(keyword_OK),1000,0))//go look for keyword "OK"
    //##Serial.println("ESP CWMODE SET");
    ;
  else
    //##Serial.println("ESP CWMODE SET FAILED"); //probably going to fail, since a 'no change' is returned if already set - would be nice to check for two words
    ;
  serial_dump_ESP();  
   
   //Here's where the SSID and PW are set
   ESP8266.print("AT+CWJAP=\"");// set the SSID AT+CWJAP="SSID","PW"
   ESP8266.print(SSID_ESP);//from constant 
   ESP8266.print("\",\"");
   ESP8266.print(SSID_KEY);//form constant
   ESP8266.print("\"\r\n");
  if(read_until_ESP(keyword_OK,sizeof(keyword_OK),10000,0))//go look for keyword "OK"
    //##Serial.println("ESP SSID SET OK");
    ;
  else
    //##Serial.println("ESP SSID SET FAILED");
    ;   
  serial_dump_ESP();
  
  //This checks for and stores the IP address
  //##Serial.println("CHECKING FOR AN IP ADDRESS");
  ESP8266.print("AT+CIFSR\r\n");//command to retrieve IP address from ESP
  if(read_until_ESP(keyword_rn,sizeof(keyword_rn),10000,0)){//look for first \r\n after AT+CIFSR echo - note mode is '0', the ip address is right after this
  if(read_until_ESP(keyword_rn,sizeof(keyword_rn),1000,1)){//look for second \r\n, and store everything it receives, mode='1'
    //store the ip adress in its variable, ip_address[]
    for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(keyword_rn)+1); i++)//that i<=... is going to take some explaining, see next lines
       ip_address[i] = scratch_data_from_ESP[i];//fill up ip_address with the scratch data received
//i=1 because i=0 is the length of the data found between the two keywords, BUT this includes the length of the second keyword, so i<= to the length minus
//size of teh keyword, but remember, sizeof() will return one extra, which is going to be subtracted, so I just added it back in +1
    ip_address[0] = (scratch_data_from_ESP[0]-sizeof(keyword_rn)+1);//store the length of ip_address in [0], same thing as before
    //##Serial.print("IP ADDRESS = ");//print it off to verify
    for(int i=1; i<=ip_address[0]; i++)//send out the ip address
    //##Serial.print(ip_address[i]);
    //##Serial.println("");
    ;
  }}//if first \r\n
  else
  //##Serial.print("IP ADDRESS FAIL");
  ;
  serial_dump_ESP();
  
   ESP8266.print("AT+CIPMUX=");// set the CIPMUX
   ESP8266.print(CIPMUX);//from constant
   ESP8266.print("\r\n");
  if(read_until_ESP(keyword_OK,sizeof(keyword_OK),5000,0)){//go look for keyword "OK" or "no change 
    //##Serial.println("ESP CIPMUX SET");
    ;
  } else {
    //##Serial.println("ESP CIPMUX SET FAILED");
    return false;
  } 
  serial_dump_ESP();
  return true;
 //that's it!  Could be done by nesting everything together, so if one thing fails, it returns '0', and if it gets all the way through it returns '1'...oh well
   
}//setup ESP


//pretty simple function - read everything out of the serial buffer and whats coming and get rid of it
void serial_dump_ESP(){
  char temp;
  while(ESP8266.available()){
    temp =ESP8266.read();
    delay(1);//could play around with this value if buffer overflows are occuring
  }//while
  
}//serial dump


//This function goes and finds a keyword received from the ESP
// const char keyword1[] = the keyword that this function will look for and return '1' when found
// int key_size = size of that keyword - sizeof()
// int timeout_val - timeout if the keword is not found (in milliseconds 5000=5sec)
// byte mode - if '1', this function will save every byte received to 'scratch_data_from_ESP[]' with length of 'scratch_length' - useful for parsing between keywords
boolean read_until_ESP(const char keyword1[], int key_size, int timeout_val, byte mode){
  timeout_start_val=millis();//for the timeout
  char data_in[20];//this is the buffer - if keyword is longer than 20, then increase this
  int scratch_length=1;//the length of the scratch data array
  key_size--;//since we're going to get an extra charachter from the sizeof()
 
 //FILL UP THE BUFFER
 for(byte i=0; i<key_size; i++){//we only need a buffer as long as the keyword
  
            //timing control
            while(!ESP8266.available()){//wait until a new byte is sent down from the ESP - good way to keep in lock-step with the serial port
              if((millis()-timeout_start_val)>timeout_val){//if nothing happens within the timeout period, get out of here
                //##Serial.println("timeout");
                return 0;//this will end the function
              }//timeout
            }// while !avail
   
    data_in[i]=ESP8266.read();// save the byte to the buffer 'data_in[]
    //Serial.write(data_in[i]);
    if(mode==1){//this will save all of the data to the scratch_data_from
      scratch_data_from_ESP[scratch_length]=data_in[i];//starts at 1
      scratch_data_from_ESP[0]=scratch_length;// [0] is used to hold the length of the array
      scratch_length++;//increment the length
    }//mode 1
    
  }//for i
  
//THE BUFFER IS FULL, SO START ROLLING NEW DATA IN AND OLD DATA OUT
  while(1){//stay in here until the keyword found or a timeout occurs

     //run through the entire buffer and look for the keyword
     //this check is here, just in case the first thing out of the ESP was the keyword, meaning the buffer was actually filled with the keyword
     for(byte i=0; i<key_size; i++){
       if(keyword1[i]!=data_in[i])//if it doesn't match, break out of the search now
       break;//get outta here
       if(i==(key_size-1)){//we got all the way through the keyword without breaking, must be a match!
       return 1; //return a 1 and get outta here!
       }//if
     }//for byte i
     
    //start rolling the buffer
    for(byte i=0; i<(key_size-1); i++){// keysize-1 because everthing is shifted over - see next line
      data_in[i]=data_in[i+1];// so the data at 0 becomes the data at 1, and so on.... the last value is where we'll put the new data
  }//for 
         
   //timing control
    while(!ESP8266.available()){// same thing as done in the buffer
      if((millis()-timeout_start_val)>timeout_val){
        //##Serial.println("timeout");
        return 0;
      }//timeout
    }// while !avail
    
    data_in[key_size-1]=ESP8266.read();//save the new data in the last position in the buffer
     //Serial.write(data_in[key_size-1]);
      if(mode==1){//continue to save everything if thsi is set
      scratch_data_from_ESP[scratch_length]=data_in[key_size-1];
      scratch_data_from_ESP[0]=scratch_length;
      scratch_length++;
    }//mode 1
    
    //  JUST FOR DEBUGGING
//    if(ESP8266.overflow())
//    //##Serial.println("*OVER");
       
  }//while 1
 
}//read until ESP


boolean connect_ESP(){//returns 1 if successful or 0 if not

  //##Serial.println("CONNECTING");
  ESP8266.print("AT+CIPSTART=0,\"TCP\",\"192.168.0.25\",8080\r\n");//connect to your web server
  //read_until_ESP(keyword,size of the keyword,timeout in ms, data save 0-no 1-yes 'more on this later') 
  if(read_until_ESP(keyword_OK,sizeof(keyword_OK),5000,0)){//go look for 'OK' and come back
  serial_dump_ESP();//get rid of whatever else is coming
  //##Serial.println("CONNECTED");//yay, connected
  ESP8266.print("AT+CIPSEND=0,");//send AT+CIPSEND=0, size of payload
  ESP8266.print(payload_size);//the payload size
  serial_dump_ESP();//everything is echoed back, so get rid of it
  ESP8266.print("\r\n");//cap off that command with a carriage return and new line feed
  
  if(read_until_ESP(keyword_carrot,sizeof(keyword_carrot),5000,0)){//go wait for the '>' character, ESP ready for the payload
    //##Serial.println("READY TO SEND");
    for(int i=0; i<payload_size; i++)//print the payload to the ESP
    ESP8266.print(payload[i]);
    
    if(read_until_ESP(keyword_sendok,sizeof(keyword_sendok),5000,0)){//go wait for 'SEND OK'
    //##Serial.println("SENT");//yay, it was sent
    return 1;//get out of here, data is about to fly out of the ESP
  }// got the SEND OK
    else// SEND OK
  //##Serial.println("FAILED TO SEND");
  ;
  }//got the back carrot >
  else
  //##Serial.println("FAILED TO GET >");
  ;
  }//First OK
  else{
  //##Serial.println("FAILED TO CONNECT");//something went wrong
  setup_ESP();//optional, this will go setup the module and attempt to repair itself - connect to SSID, set the CIPMUX, etc...
  }

}// VOID CONNECT FUNCTION


void connect_webhost(){

  // This builds out the payload URL - not really needed here, but is very handy when adding different arrays to the payload
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
   
  if(connect_ESP()){//this calls 'connect_ESP()' and expects a '1' back if successful
  //nice, we're in and ready to look for data
  //first up, we need to parse the returned data  _t1123##_d15##_d210##

    if(read_until_ESP(keyword_doublehash,sizeof(keyword_doublehash),5000,0)){//go find t1 then stop
      if(read_until_ESP(keyword_doublehash,sizeof(keyword_doublehash),5000,1)){//our data is next, so change mode to '1' and stop at ##
        //got our data, so quickly store it away in d1
        for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(keyword_doublehash)+1); i++)//go see 'setup' and how this was done with the ip address for more info
        resposta_site[i] = scratch_data_from_ESP[i];
        resposta_site[0] = (scratch_data_from_ESP[0]-sizeof(keyword_doublehash)+1);
          
            //now that we have our data, go wait for the connection to disconnect - the ESP will eventually return 'Unlink'
            if(read_until_ESP(keyword_linkdisc,sizeof(keyword_linkdisc),5000,0)){
              // delay(10);
              //##Serial.println("FOUND DATA & DISCONNECTED");
              serial_dump_ESP();//now we can clear out the buffer and read whatever is still there
              //let's see how the data looks
              
               //##Serial.print("Resposta: ");
               for(int i=1; i<=resposta_site[0]; i++){
                //##Serial.print(resposta_site[i]);
               }
                          
               //##Serial.println("");
               
              //that's it!!
                }//link     
      }//##
    }//resposta
  }//connect ESP


}//connect web host
