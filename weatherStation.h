//------------------estacion---------------------
void loop_weatherStation()
{
  if(s_frecuencia&& status_router)                                                        //envio por frecuencia
  { 
    if(Wifi_disconect){if(test_conexion()){controlPost_frecuencia();}}  // verifica si esta esta desconectado
    else controlPost_frecuencia(); 
  } 
}
void init_sensor()
{
  //HDC1080
  Serial.println("Datos del Sensor HDC1080: ");
  hdc1080.begin(0x40);
  Serial.print("Manufacturer ID=0x");
  Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
  Serial.print("Device ID=0x");
  Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device
  printSerialNumber();
  delay(100);
}
int POST_servicio() //test_conexion(), interfaz_led(n)->api returna cero si hay algun error en envio
{
  WiFiClient client; const int httpPort = 80;
  int status_POST=0;
  if(Wifi_disconect && status_router){if(!test_conexion()){return 0;}} // verifica si esta esta desconectado y si no verifica si se puede conectar
  interfaz_LED_POST(1);
  Serial.print("conectando a host:");   Serial.print(host);Serial.println(" ...\n"); delay(500);
  if (!client.connect(host, httpPort)) {Serial.println("connection failed");return 0;}
  else
  {
    //read sensor
    read_HDC1080();
    String data_to_send = api_key;
    data_to_send += "&field1=";data_to_send += String(Temperatura);
    data_to_send += "&field2=";data_to_send += String(Humedad);
    
    client.flush();
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    status_POST=view_answer_post(client);
  }
  interfaz_LED_POST(2);
  client.stop(); 
  return 1; 
}
int view_answer_post(WiFiClient client)
{
   String line="";unsigned long timeout = millis(); 
   while (!client.available()){if (millis() - timeout > 5000) {Serial.println(">>Client Timeout-POST!");client.stop();return 0;}} //time limite answer    
   while(client.available()) { line += client.readStringUntil('\r');}  //read peticion
   Serial.println("Respuesta del servidor:"); Serial.println(line); Serial.println("\nclosing connection POST");
   if(line.indexOf("200 OK") >= 0 )
   {  
        Serial.println("POST Servicio OK!");
        Obten_hora_post(line);   print_hora();
        return 1;
   }
   else{ Serial.println("POST Servicio BAD!"); return 0;}
}
void interfaz_LED_POST(int estado)
{  
  if(estado==1){LED_ALTO delay(300);LED_BAJO delay(300);LED_ALTO}
  if(estado==2){LED_BAJO}  
}
int POST_thingspeak() // servicio de envio a thigspeak
{
  if(strstr(buffer_E,"/post_thingspeak"))
    {   //visualizar en JSON     
      read_HDC1080();
      return 1;
    }
  return 0; 
}
int ConfigFrecuecniaPost(char *buffer_N) //formato ip/frecuencia_post/horas=hh&minutos=mm  o ip/frecuencia_post/clear
{  
  if(strstr(buffer_N,"/frecuencia_post/clear")) //elimina la frecuencia de envio
  {
    s_Configfrecuencia_postClear=1;  s_frecuencia=0; 
    EEPROM.write(BanderaConfig_Frecuencia,0); endwriteEEPROM  return 0;
  } 
  s_Configfrecuencia_postClear=0;
  if(strstr(buffer_N,"/frecuencia_post/"))
  {
    String sub_buffer_N="";  for (int i=0;i<size_buffer;i++){sub_buffer_N+=buffer_N[i];}  sub_buffer_N+=0; //convertir char array to String
    boolean Sf_hora=strstr(buffer_N,"horas="),Sf_minutos=strstr(buffer_N,"minutos="); int pos_inicial=sub_buffer_N.indexOf("frecuencia_post/");
    if(Sf_hora&&!Sf_minutos)
    {
        frecuencia=horasTominutos(sub_buffer_N.substring(sub_buffer_N.indexOf("horas=")+6,sub_buffer_N.indexOf(' ',pos_inicial)).toInt()); 
        if(frecuencia==0){frecuencia=1; return 0;}
    }     
    if(!Sf_hora&&Sf_minutos)
    {
        frecuencia=sub_buffer_N.substring(sub_buffer_N.indexOf("minutos=")+8,sub_buffer_N.indexOf(' ',pos_inicial)).toInt(); 
        if(frecuencia==0){frecuencia=1; return 0;}
    }     
    if(Sf_hora&&Sf_minutos)
    {
        frecuencia=horasTominutos(sub_buffer_N.substring(sub_buffer_N.indexOf("horas=")+6,sub_buffer_N.indexOf('&,pos_inicial')).toInt()); 
        frecuencia=sub_buffer_N.substring(sub_buffer_N.indexOf("minutos=")+8,sub_buffer_N.indexOf(' ',pos_inicial)).toInt()+frecuencia;
        if(frecuencia==0){frecuencia=1; return 0;}     
    }    
    if (Sf_hora || Sf_minutos)
    { 
    	Serial.print("\n->frecuencia de envio configurada cada ");Serial.print(frecuencia);Serial.println(" minutos");
        write_EEPROMfrecuencia(); s_frecuencia=1; return 1;
    }
     return 0; 
  }
  else return 0;
}
unsigned int horasTominutos(unsigned int horas){return horas*60;} 
void controlPost_frecuencia()  //->funcion ejecutada en el loop
{
  if(s_send_postFrecuencia) 
  {
      tiempoParaenvio=horasTominutos(hora[0])+hora[1]+frecuencia;   //tiempo actual+frecuencia de envio-<
      if(tiempoParaenvio>=Limite_minutosForDia){ tiempoParaenvio=tiempoParaenvio-Limite_minutosForDia; }
      s_send_postFrecuencia=0;
      Serial.println("configuracion de tiempo para envio por frecuencia");
  }
  else 
  { 
    if((horasTominutos(hora[0])+hora[1])== tiempoParaenvio)
    {
      int StatusPost=0,intentos=0;
      Serial.print("Envio por Frecuencia!!!\npasaron ");Serial.print(frecuencia); Serial.println(" minutos");
      while(!StatusPost&& intentos<3){
          StatusPost=POST_servicio();
          //verificar con un GET https://api.thingspeak.com/channels/361821/feeds.json?results=2 si last_entry_id aumento. *PROPUESTA.  
          intentos++;
        }
       if(StatusPost){s_send_postFrecuencia=1;}
       else { ESP.restart();}
    }
  }
}
float promedio_read_sensor(char *sensor, int muestra) //funciones HDC1080
{
  float promedio=0.0;

  if(strstr(sensor,"temperatura"))
  {
    for(int i=0;i<muestra;i++){promedio+=hdc1080.readTemperature();} return promedio/muestra;
  }
  if(strstr(sensor,"humedad"))
  {
    for(int i=0;i<muestra;i++) {promedio+=hdc1080.readHumidity();}return promedio/muestra;
  }
  Serial.println("Bung en la seleccion del sensor- metodo:""promedio_read_sensor""");
  return 0;
}
void read_HDC1080()
{  
  hdc1080.begin(0x40);
  Temperatura=promedio_read_sensor("temperatura",numero_muestras_Sensor); 
  Humedad=promedio_read_sensor("humedad",numero_muestras_Sensor);
}
void init_EEPROMfrecuenicia()
{
 frecuencia=(int)EEPROM.read(Frecuencia_DEE); 
 Serial.print("Frecuencia de envio cada ");Serial.print(frecuencia); Serial.println(" minutos");
  s_frecuencia=1;
}
void write_EEPROMfrecuencia()
{
   EEPROM.write(BanderaConfig_Frecuencia,1);  
   EEPROM.write(Frecuencia_DEE,frecuencia);
   Serial.println("datos de frecuencia guardados en EEPROM");
   endwriteEEPROM
}
void printSerialNumber() 
{
  Serial.print("Device Serial Number=");
  HDC1080_SerialNumber sernum = hdc1080.readSerialNumber();
  char format[12];
  sprintf(format, "%02X-%04X-%04X", sernum.serialFirst, sernum.serialMid, sernum.serialLast);
  Serial.println(format);
  Serial.println("");
}


