//respuesta de peticiones

#define head_JSON_RESPUESTA client.println("HTTP/1.1 200 OK\nContent-Type: application/json; charset=utf-8"); client.println("");
void JSON_infoESP32(WiFiClient client)
{   read_HDC1080();
    head_JSON_RESPUESTA       
    client.print("{");
    client.print("\"IP red\":\"");  client.print(WiFi.localIP());client.print("\"");
    client.print(",\"IP local\":\"");client.print(WiFi.softAPIP());client.print("\"");   
    if(status_hora){client.print(",\"Hora\":\"");client.print(hora[0]);client.print(":");client.print(hora[1]);client.print(":");client.print(hora[2]);client.print("\"");} 
    client.print(",\"Status_router\":"); if(Wifi_disconect){client.print("\"desconectado\"");} else {client.print("\"Conectado\"");} 
    if(EEPROM.read(BanderaConfig_RED)        ==1){client.print(",\"SSID\":\"");client.print(ssid); client.print("\"");}
    if(EEPROM.read(BanderaConfig_Frecuencia) ==1){client.print(",\"Frecuencia\":\"");client.print(frecuencia); client.print(" minutos\"");} 
    client.print(",\"temp\":"); client.print(Temperatura);
    client.print(",\"hum\":"); client.print(Humedad);
    client.print(",\"PETICION_ESP32\":\"INFO\"");
    client.println("}");
  }
//del sistema
void JSONconfig_WIFI_password(WiFiClient client)
{
	head_JSON_RESPUESTA   
    client.print("{");
    client.print("\"IP red\":\"");  client.print(WiFi.localIP());client.print("\"");
    client.print(",\"IP local\":\"");client.print(WiFi.softAPIP());client.print("\"");
    if(status_hora){client.print(",\"Hora\":\"");client.print(hora[0]);client.print(":");client.print(hora[1]);client.print(":");client.print(hora[2]);client.print("\"");} 
    client.print(",\"Status_router\":"); if(Wifi_disconect){client.print("\"desconectado\"");} else {client.print("\"Conectado\"");} 
    client.print(",\"SSID\":\"");client.print(ssid); client.print("\""); 
    client.print(",\"PETICION_ESP32\":\"CONFIG_RED\"");
    client.println("}");
}
void JSONclearRED(WiFiClient client)
{
	head_JSON_RESPUESTA 
    client.print("{"); 
    client.print("\"PETICION_ESP32\":\"CLEAR_RED\"");
    client.println("}");  
}
//estacion
void JSONPOST_thingspeak(WiFiClient client)
{
    head_JSON_RESPUESTA 
    client.print("{");
    client.print("\"temp\":"); client.print(Temperatura);
    client.print(",\"hum\":"); client.print(Humedad);
    if(status_hora){client.print(",\"Hora\":\"");client.print(hora[0]);client.print(":");client.print(hora[1]);client.print(":");client.print(hora[2]);client.print("\"");} 
    client.print(",\"Status_router\":"); if(Wifi_disconect){client.print("\"desconectado\"");} else {client.print("\"Conectado\"");} 
    client.print(",\"PETICION_ESP32\":\"POST_THINGSPEAK\"");
    client.println("}");
}
void JSONConfigFrecuecniaPost(WiFiClient client)
{
    head_JSON_RESPUESTA  
    client.print("{");
    client.print("\"Frecuencia\":"); client.print(frecuencia);
    if(status_hora){client.print(",\"Hora\":\"");client.print(hora[0]);client.print(":");client.print(hora[1]);client.print(":");client.print(hora[2]);client.print("\"");} 
    client.print(",\"Status_router\":"); if(Wifi_disconect){client.print("\"desconectado\"");} else {client.print("\"Conectado\"");} 
    client.print(",\"PETICION_ESP32\":\"CONFIG_FRECUENCIA_POST\"");
    client.println("}");   
}
void JSONclearFrecuencia(WiFiClient client)
{
  	head_JSON_RESPUESTA   
    client.print("{");
    client.print("\"PETICION_ESP32\":\"CLEAR_FRECUENCIA\"");
    if(status_hora){client.print(",\"Hora\":\"");client.print(hora[0]);client.print(":");client.print(hora[1]);client.print(":");client.print(hora[2]);client.print("\"");} 
    client.print(",\"Status_router\":"); if(Wifi_disconect){client.print("\"desconectado\"");} else {client.print("\"Conectado\"");} 
    client.println("}"); 
}
//domotica
void JSONdef_control_Wifi_out(WiFiClient client)
{
    head_JSON_RESPUESTA      
    client.print("{");
    client.print("\"OUT1\":");client.print(Status_Out1);
    client.print(",\"OUT2\":");client.print(Status_Out2);
    client.print(",\"OUT3\":");client.print(Status_Out3);  
    client.print(",\"PETICION_ESP32\":\"CONTROL_OUT\"");
    client.println("}");
}
void JSONclear_status_temporizador_out(WiFiClient client)
{
    head_JSON_RESPUESTA      
    client.print("{");
    client.print("\"TEMP_OUT1\":");client.print(status_temp_out1);
    client.print(",\"TEMP_OUT2\":");client.print(status_temp_out2);
   client.print(",\"TEMP_OUT3\":");client.print(status_temp_out3);
    client.print(",\"PETICION_ESP32\":\"CLEAR_TEMPORIZADOR\"");
    client.println("}");
}
void JSONcontrolSatus_temporizador_out(WiFiClient client)
{
    head_JSON_RESPUESTA      
    client.print("{");
    if(status_temp_out1){
            client.print("\"TEMP_OUT1_ON\":\"");
            client.print(hora_on1[0]);client.print(":");client.print(hora_on1[1]);client.print(":");client.print(hora_on1[2]);client.print("\""); 
            client.print(",\"TEMP_OUT1_OFF\":\"");
            client.print(hora_off1[0]);client.print(":");client.print(hora_off1[1]);client.print(":");client.print(hora_off1[2]);client.print("\"");
            if(status_temp_out2||status_temp_out3){client.print(",");}
        }
    if(status_temp_out2){
            client.print("\"TEMP_OUT2_ON\":\"");
            client.print(hora_on2[0]);client.print(":");client.print(hora_on2[1]);client.print(":");client.print(hora_on2[2]);client.print("\""); 
            client.print(",\"TEMP_OUT2_OFF\":\"");
            client.print(hora_off2[0]);client.print(":");client.print(hora_off2[1]);client.print(":");client.print(hora_off2[2]);client.print("\"");
            if(status_temp_out3){client.print(",");}
        }
    if(status_temp_out3){
            client.print("\"TEMP_OUT3_ON\":\"");
            client.print(hora_on3[0]);client.print(":");client.print(hora_on3[1]);client.print(":");client.print(hora_on3[2]);client.print("\""); 
            client.print(",\"TEMP_OUT3_OFF\":\"");
            client.print(hora_off3[0]);client.print(":");client.print(hora_off3[1]);client.print(":");client.print(hora_off3[2]);client.print("\"");
        }
    client.print(",\"PETICION_ESP32\":\"CONFIG_STATUS_TEMPORIZADOR\"");
    client.println("}");

}
void JSOdef_hora_Wifi(WiFiClient client)
{
	head_JSON_RESPUESTA
    client.print("{\"Hora\":\""); client.print(hora[0]);client.print(":");client.print(hora[1]);client.print(":");client.print(hora[2]); client.print("\"");
    client.print(",\"PETICION_ESP32\":\"CONFIG_HORA_MANUAL\"");
    client.println("}");
}


