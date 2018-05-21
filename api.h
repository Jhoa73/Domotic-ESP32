void IRAM_ATTR onTimer() 
{
  portENTER_CRITICAL_ISR(&timerMux);
  Bandera_time++;
  portEXIT_CRITICAL_ISR(&timerMux);
  hora[2]++;
   if(segundos_minutos()){minutos_horas();dia();} 
}
void setup_api()
{
  pinMode(led, OUTPUT);  LED_ALTO
  init_out();
   
  Serial.begin(115200);while(!Serial) {} Serial.println("\"Sytem Esp32-32bits-IoT-Core Arduino\" version 4.0 , for Nelson Jhoann Concha Canto (RC)\nactualizacion:15/03/18\n");

  init_device();// inicia sensor
  
    //configurar nombre de RED  
  WiFi.softAP(my_ssid, my_password);  //config para conexion punto a punto
  delay(200); info_device_red();
  server.begin();
 }
 int Maximo_intentos=0;
void loop_api()
{ 
  servidor();
  if(Wifi_disconect && status_router){if(test_conexion()){Maximo_intentos=0;}else {Maximo_intentos++;}}  //auto test de conexion 
  if(Maximo_intentos>3){status_router=0; EEPROM.write(BanderaConfig_RED,0); endwriteEEPROM Serial.println("Red no existente Borra en loop");Maximo_intentos=0;} 
  interfaz_LED_conect();
  
  loop_weatherStation();    
  loop_out();
    
}
 void init_device()
{ 
  //inicializacion timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
  //EEPROM
  init_EEPROM();
  //inicializa sensor
  #if HDC1080_C
  init_sensor();
  #endif
}
void init_EEPROM()
{
  if (!EEPROM.begin(EEPROM_SIZE)){Serial.println("failed to initialise EEPROM"); delay(1000000);}
  if(EEPROM.read(BanderaConfig_RED)==1)//verifica si ya se ha configurado la red anteriormente
  	{ 
  		Serial.print("RED guardad en EEPROM: ");init_EEPROMred(); 
  	}          
  else Serial.println("RED NO DEFINIDA!");//verifica si ya se ha configurado la frecuencia de envio anteriormente
  if(EEPROM.read(BanderaConfig_Frecuencia)==1)
  { 
  		Serial.print("Frecuencia guardad en EEPROM: ");init_EEPROMfrecuenicia();
   } 
  else Serial.println("FRECUENCIA DE ENVIO NO DEFINIDA!");
  Serial.println("");
}
int config_WIFI_password(char *buffer_N) //formato ip/ssid=jdshadjh&password=sdnsnd & ip/clear_red->borra memoria de EEPROM 
{  
  if(strstr(buffer_N,"/clear_red"))   //borra configuraciones de red
  { 
    status_router=0; status_config_routerClear=1; 
    EEPROM.write(BanderaConfig_RED,0); endwriteEEPROM  
    return 0;
  } 
  unsigned int init_pos1=0, init_pos2=0;String sub_buffer_N=""; status_config_routerClear=0;
  if(strstr(buffer_N,"password=")&&strstr(buffer_N,"ssid=")) //configura red
  {
    for (int i=0;i<size_buffer;i++){sub_buffer_N+=buffer_N[i];} sub_buffer_N+=0; //convertir char array to String
    init_pos1= sub_buffer_N.indexOf("ssid=")+5;
    init_pos2= sub_buffer_N.indexOf("password=")+9;
    
    ssid=sub_buffer_N.substring(init_pos1,sub_buffer_N.indexOf('&',init_pos1));
    password=sub_buffer_N.substring(init_pos2,sub_buffer_N.indexOf(' ',init_pos2));
    //test red
    Serial.print("ssid:");     Serial.println(ssid);
    Serial.print("password:"); Serial.println(password);
    if(ssid.length()>MaxCaracteresRED || password.length()>MaxCaracteresRED){Serial.println("El numero de caracteres para la configuracion de RED esta fuera de su rango"); return 0;}
    
    if(test_conexion()){write_EEPROMred(); return 1;}        // si la red ingresas esta conectada correctamente, se guarda las configuraciones en la EEPROM
    else { status_router=0; EEPROM.write(BanderaConfig_RED,0); endwriteEEPROM return 0;}
   }
   else return 0;
}
int test_conexion()
{	
	int intentos=0;
    Serial.print("Conectando a ");Serial.print(ssid);Serial.print("...");interfaz_LED_conect();
    do
    { 
	    delay(100);
	    WiFi.begin(ssid.c_str(), password.c_str());
	    
	    // Espera a conectar a RED
	    unsigned long timeout = millis();
	    while(Wifi_disconect)
	    { 
	    	if (millis() - timeout > 10000) { Serial.println("\nRouter BAD!!!");intentos++; break;}
	    }
	    if(Wifi_conect){  Serial.println("\nRouter OK!\n");interfaz_LED_conect(); status_router=1;info_device_red();  return 1; }
	   
	}while(intentos<3);
  Serial.println("intentos finalizados\n");
	return 0; 
}
void info_device_red()
{
  Serial.println("\ndatos del Wifi:");
  Serial.print("IP address API local: ");Serial.println(WiFi.softAPIP());
  Serial.print("IP address: ");Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address: ");Serial.println(WiFi.macAddress());
  Serial.print("Subnet Mask: ");Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: "); Serial.println(WiFi.dnsIP());  Serial.println("");
}
void interfaz_LED_conect()
{
  if(Wifi_disconect){LED_ALTO}
  else{LED_BAJO}
}

void servidor()//-------------------Servidor----------------
{
  WiFiClient client = server.available();
  k_buffer=0; clear_buffer
  if (client) 
  {
   Serial.println("\r\n--Peticion de cliente-- \r\n"); 
   while (client.connected()) 
   {      
      *buffer_E=read_buffer_wifi(buffer_E,client);
      
      if (!currentLineIsBlank)  //rutina para atender la peticion
      {
        Serial.println(buffer_E);
        //servicio de API
        peticiones(buffer_E);
        JSON_respuesta(client);
        Serial.println("\r\n--Envio de respuesta al cliente-- \r\n"); 
        cerrar_cliente
        currentLineIsBlank= true;                  
      }
      else {k_buffer=0; clear_buffer currentLineIsBlank= true; return;}        
   }     
      
   if(status_post_thingspeak){ Serial.println("\nEnvio por peticion!!!"); POST_servicio(); return;} //envio por peticion                                                             //thingspeak por peticion    
   if(s_Configfrecuencia_post)  //envio inicial por configuracion de frecuencia
    {
      Serial.println("\nEnvio por Configuracion de Frecuencia!!!"); POST_servicio(); 
      s_frecuencia=1; s_send_postFrecuencia=1; return;
    }
   if(status_config_routerClear){delay(500); WiFi.disconnect(true);delay(500); return;}
  
 } // fin client    
}
void peticiones(char *buffer_E){
        status_config_router= config_WIFI_password(buffer_E);                    //config ssid & password wifi o borra info de red  
         
        //servicion de estacion
        status_post_thingspeak=POST_thingspeak();                                                        //peticion para envio a thingspeak
        s_Configfrecuencia_post=ConfigFrecuecniaPost(buffer_E);                                         //configuracion frecuencia envio o borra configaracion
         
         //servicio de domotica
       if(!JSON_OPCION){                                                                        
          status_controlOut=def_control_Wifi_out(buffer_E);                         //modifica estatus y salidas de OUT domotica                
          status_config_temp=controlSatus_temporizador_out(buffer_E);          // control de varibles para la tarea temporizador  
          status_config_hora= def_hora_Wifi(buffer_E);                                                         // rutina de configuracion de hora                                                        
          status_controlOutJSON=false;status_config_tempJSON=false;status_config_horaJSON=false;
        }                                                                                                       
        else{                                                                                                         
        status_controlOutJSON=def_control_Wifi_out(buffer_E);                        //modifica estatus y salidas de OUT domotica                
        status_config_tempJSON=controlSatus_temporizador_out(buffer_E);            // control de varibles para la tarea temporizador  
        status_config_horaJSON = def_hora_Wifi(buffer_E); 
        status_controlOut=false;status_config_temp=false;status_config_hora=false;
        }
}
unsigned long timeout;         
char read_buffer_wifi(char * buffer_E, WiFiClient client)
{
    timeout = millis();
    while (!strstr(buffer_E,"\n\r\n")){  
        //c=CharMayus((char)client.read());   //aplicacion homologa de caracteres
        if(client.available()){
            c=(char)client.read();
            if (exep_1&&c>0){buffer_E[k_buffer]=c; k_buffer+=1;}
        }
        if(millis() - timeout > 3000) { /*Serial.println("timeout!")*/; break;}
      }
      if(buffer_E[0]>0){currentLineIsBlank=false;}
      return *buffer_E;
 }
void JSON_respuesta(WiFiClient client) //JSON
{
  client.flush();
  if(status_config_router)        //respuesta a peticion para configuracion de red         
  {
    JSONconfig_WIFI_password(client); return;
  }
  else if(status_post_thingspeak) //respuesta a peticion para envio a thingspeak
  {  
    JSONPOST_thingspeak(client);  return;
  }
  else if(s_Configfrecuencia_post) //respuesta a peticion para configurar frecuencia de envio 
  {
    JSONConfigFrecuecniaPost(client); return;
  }
  else if(status_controlOutJSON)         //control out
  {
    JSONdef_control_Wifi_out(client);return;
    }
  else if(status_config_tempJSON)        //configuracion de temporizador
  {
    JSONcontrolSatus_temporizador_out(client);return;
   }
   else if(status_config_horaJSON)        //configuracion de hora
   {
    JSOdef_hora_Wifi(client);return;
   }
   else if(status_config_tempClearJSON)   //eliminar temporizador de apagado
   {
    JSONclear_status_temporizador_out(client);return;
   }
  else if(status_config_routerClear)         //desconectar router
  {
    JSONclearRED(client); return;
  }
  else if(s_Configfrecuencia_postClear)     //sin frecuencia denvio
  {
    JSONclearFrecuencia(client);return;
  }
   else if(status_controlOut || status_config_temp || status_config_hora|| status_config_tempClear) //vistas_out
  {
    HTML_respuesta_out(client); return;
  }
  else //info (vista para borra RED y frecuencia internas en su peticion)
  {
    JSON_infoESP32(client);
  }
}
void init_EEPROMred()
{ 
  for (int i=Password_DEE;i<Password_DEE+MaxCaracteresRED&&EEPROM.read(i)!=255&&EEPROM.read(i)!=0;i++){password+=(char)EEPROM.read(i);} //inicializa password
  for (int i=SSID_DEE    ;i<SSID_DEE+MaxCaracteresRED&&EEPROM.read(i)!=255&&EEPROM.read(i)!=0    ;i++){ssid+=    (char)EEPROM.read(i);}    //inicializa SSID
   Serial.print("SSID guardada ");    Serial.println(ssid); status_router=1;
}
void write_EEPROMred()
{
   EEPROM.write(BanderaConfig_RED,1);  status_router=1;
   for (int i=Password_DEE;i<Password_DEE+MaxCaracteresRED;i++){if(password.charAt(i-Password_DEE)!=0) EEPROM.write(i,0);EEPROM.write(i,password.charAt(i-Password_DEE));}
   for (int i=SSID_DEE;    i<SSID_DEE+MaxCaracteresRED;i++)    {if(ssid.charAt(i-SSID_DEE)    !=0)     EEPROM.write(i,0);EEPROM.write(i,ssid.charAt(i-SSID_DEE));    }
   Serial.println("datos de la red guardados en EEPROM");     endwriteEEPROM
}
void print_hora()
{
  Serial.print(hora[0]);Serial.print(':'); Serial.print(hora[1]);Serial.print(':'); Serial.println(hora[2]);Serial.println("");
}
void Obten_hora_post(String line)
{
      //definicion hora Tue, 20 Feb 2018 04:10:31
      int index=line.indexOf("Date:");  //Date: Thu, 25 Jan 2018 18:19:19 GMT---definiciÃƒÂ³n de hora
      if( index >= 0 )
      {     
        unsigned int init_pos=index+23, fin_pos=line.indexOf(':', init_pos);
        for (int i = 0; i < 3; i++)       //configuracion hora
        {
          if(i==0){ hora[i]= hora_GMT_6(line.substring(init_pos,fin_pos).toInt(),true);} //conversion a GMT-6
          else    { hora[i]= line.substring(init_pos,fin_pos).toInt();} 
          init_pos = fin_pos + 1;                                                                                          
          fin_pos = line.indexOf(':', init_pos);
        }
        Serial.println("hora config OK server-""POST""");
        status_hora=Alto; 
      }
}
int hora_GMT_6(unsigned int hora, boolean Status)
{
  //true= 5 & false =6
  int RestHour=0;
  if(Status){ RestHour = 5; }
  else { RestHour = 6; }
  
  int gmtMex=hora - RestHour;
  if(gmtMex<0){gmtMex=gmtMex+24; return gmtMex ;}
  else return hora -  RestHour;
}
boolean segundos_minutos()
{
  if(hora[2]>=60){hora[2]=0;hora[1]++;return 1;}
  else {return 0;}
}
boolean minutos_horas()
{
  if (hora[1]>=60){hora[1]=0;hora[0]++;return 1;}
  else {return 0;}
}
boolean dia()
{
  if(hora[0]>=24){hora[0]=0;return 1;}
  else {return 0;}
}
char CharMayus(char c) // funcion para convertir en mayuscula
{
  if((c>='a') && (c<='z')) return c-32;
  else return c;
}



