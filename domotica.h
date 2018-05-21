//-------------domotica-----------------
void loop_out()
{
	temporizador_out(); 
}
void init_out()
{
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  OUT1_ALTO OUT2_ALTO OUT3_ALTO
  Status_Out1=Bajo; Status_Out2=Bajo; Status_Out3=Bajo;
}
int def_control_Wifi_out(char * buffer_E) // ip/outn_on o ip/outn_off o ip/vista_out
{ 
  if(strstr(buffer_E,"out1_on")){        /*Serial.println("\r\n--pone en alto OUT1 --");*/ ENCENDER_OUT1 return 1;}
  else if(strstr(buffer_E,"out1_off")){  /*Serial.println("\r\n--pone en bajo OUT1--");*/  APAGAR_OUT1   return 1;}
        
  if(strstr(buffer_E,"out2_on")){        /*Serial.println("\r\n--pone en alto OUT2 --");*/ ENCENDER_OUT2 return 1;}
  else if(strstr(buffer_E,"out2_off")){  /*Serial.println("\r\n--pone en bajo OUT2--");*/  APAGAR_OUT2   return 1;}
        
  if(strstr(buffer_E,"out3_on")){        /*Serial.println("\r\n--pone en alto OUT3 --");*/ ENCENDER_OUT3 return 1;}
  else if(strstr(buffer_E,"out3_off")){  /*Serial.println("\r\n--pone en bajo OUT3--");*/  APAGAR_OUT3   return 1;}
        
  if(strstr(buffer_E,"outall_on")){        /*Serial.println("\r\n--pone en alto OUT1 --");*/ ENCENDER_ALL return 1;}
  else if(strstr(buffer_E,"outall_off")){  /*Serial.println("\r\n--pone en bajo OUT1--");*/  APAGAR_ALL   return 1;}     

  if(strstr(buffer_E,"/vista_out")){return 1;}
  return 0;
}
void temporizador_out()
{ 
  if(status_temp_out1) 
  {
    TEMP_APAGA1 TEMP_ENCIENDE1   
   }
  if(status_temp_out2)
  {
    TEMP_APAGA2 TEMP_ENCIENDE2
  }
  if(status_temp_out3) 
  {
    TEMP_APAGA3 TEMP_ENCIENDE3
  }
}
int clear_status_temporizador_out(char * buffer_E) // ip/temporizador_clear
{
   if(strstr(buffer_E,"/temporizador_clear")){status_temp_out1=Bajo; status_temp_out2=Bajo; status_temp_out3=Bajo; status_temp=0; return 1;}
   return 0;
}
int controlSatus_temporizador_out(char * buffer_E) // ip/temporizador_out1:hh:mm:ss&hh:mm:ss primero hora de encender y luego apagar.
{
   if(JSON_OPCION){status_config_tempClearJSON=clear_status_temporizador_out(buffer_E);}
   else{status_config_tempClear=clear_status_temporizador_out(buffer_E);}
    if(status_config_tempClear||status_config_tempClearJSON){return 0;}
              
    String sub_buffer_N="";  for (int i=0;i<size_buffer;i++){sub_buffer_N+=buffer_E[i];}  sub_buffer_N+=0; //convertir char array to String
    unsigned int initPosOn=sub_buffer_N.indexOf(":"),initPosOff=sub_buffer_N.indexOf("&");
   // TEMPORIZADOR_OUT1:hora de encender&hora de apagar
    
   if(sub_buffer_N.indexOf("temporizador_out1:")>0)
   {     
    for(int i=0;i<3;i++){
     hora_on1[i]=sub_buffer_N.substring(initPosOn+1,initPosOn+3).toInt();
     hora_off1[i]=sub_buffer_N.substring(initPosOff+1,initPosOff+3).toInt();  
     initPosOn=sub_buffer_N.indexOf(":",initPosOn+1);
     initPosOff=sub_buffer_N.indexOf(":",initPosOff+1); 
    }
     status_temp_out1=1; status_temp=Alto;
     return 1;
   }
   else if(sub_buffer_N.indexOf("temporizador_out2:")>0)
   {
     for(int i=0;i<3;i++){
     hora_on2[i]=sub_buffer_N.substring(initPosOn+1,initPosOn+3).toInt();
     hora_off2[i]=sub_buffer_N.substring(initPosOff+1,initPosOff+3).toInt();  
     initPosOn=sub_buffer_N.indexOf(":",initPosOn+1);
     initPosOff=sub_buffer_N.indexOf(":",initPosOff+1); 
    }
     status_temp_out2=1;status_temp=Alto;
     return 1;
   }
   else if(sub_buffer_N.indexOf("temporizador_out3:")>0)
   {
      for(int i=0;i<3;i++){
     hora_on3[i]=sub_buffer_N.substring(initPosOn+1,initPosOn+3).toInt();
     hora_off3[i]=sub_buffer_N.substring(initPosOff+1,initPosOff+3).toInt();  
     initPosOn=sub_buffer_N.indexOf(":",initPosOn+1);
     initPosOff=sub_buffer_N.indexOf(":",initPosOff+1); 
    }  
     status_temp_out3=1;status_temp=Alto;
      return 1;
   } 
   return 0; 
}
int def_hora_Wifi(char * buffer_E) // ip/hora:hh:mm:ss
{
  char StoI[6]; unsigned int initial_position=4; 
  if(strstr(buffer_E,"hora:"))
  { 
     //format /hora:hh:mm:ss  24H
     Serial.println("configuracion de hora manual");    
     *StoI  =split_hora(buffer_E,initial_position,StoI);
     *hora =horaStoI(StoI,hora);  //conversion de del arreglo de char a horas            
     status_hora=Alto;  
     return Alto ;
   }
  else return Bajo;
}
char split_hora(char * buffer_E, int initial_position, char *StoI)
{
 int i_StoI=0;
  
 while(buffer_E[initial_position] != ':' && initial_position<size_buffer){initial_position++;}
 for(int i=initial_position; i<size_buffer && i_StoI<6 ;i++)
 {
   if(buffer_E[i] !=':'){StoI[i_StoI]=buffer_E[i]; i_StoI++;}  
 }
 return *StoI;
}
unsigned int horaStoI(char * StoI, unsigned int * hora)
{
  hora[0]=10*(StoI[0]-48)+(StoI[1]-48);  hora[1]=10*(StoI[2]-48)+(StoI[3]-48);  hora[2]=10*(StoI[4]-48)+(StoI[5]-48);
  return *hora;
}
void HTML_respuesta_out(WiFiClient client) //cualquier configuracion de salidas 
{
   client.flush();
   client.println("HTTP/1.1 200 OK");
   client.println("Content-Type: text/html");
   
   client.println("");
   client.println("<!DOCTYPE HTML>");
   
   client.println("<html>");
   client.println("<head>");
   client.println("<meta charset=""UTF-8"">"); 
   client.println("</head>");       
   client.println("<body style=""background-color:#084B8A"">");     
   client.println("<title> Dormitorio ESP32 </title>");
 
   client.println("<h1 align=""center"" style=""color:#FFFFFF""> Control de luces - dormitorio </h1>" );
       
   client.println("<div style=""background-color:#0B0B61"" align=""center"">");
   client.println("<h3 style=""color:#FFFFFF"">Estado de las luces</h3>");
   
   client.println("<table width=""50%"" border=""2"" align=""center"" cellspacing=""0"" bordercolor=""#000000"" bgcolor=""#084B8A"" style=""color:#FFFFFF"">" );
   client.println("<tr> <td align=""center"">Luz</td><td align=""center"">Estado</td> </tr>");   
   //------control-----
   switch (Status_Out1)
   {
      case Alto: client.println("<tr> <td align=""center"">Terraza</td> <td align=""center"" bgcolor=""#AEB404"">Encendido</td> </tr>"); break; 
      case Bajo: client.println("<tr> <td align=""center"">Terraza</td> <td align=""center"" bgcolor=""#04B404"">Apagado</td> </tr>"); break;
      default:   break;
    }
    switch (Status_Out2)
    {
      case Alto: client.println("<tr> <td align=""center"">Cuarto</td> <td align=""center"" bgcolor=""#AEB404"">Encendido</td> </tr>"); break; 
      case Bajo: client.println("<tr> <td align=""center"">Cuarto</td> <td align=""center"" bgcolor=""#04B404"">Apagado</td> </tr>"); break;
      default:   break;
     }

     switch (Status_Out3)
     {
       case Alto: client.println("<tr> <td align=""center"">Pasillo</td> <td align=""center"" bgcolor=""#AEB404"">Encendido</td> </tr>"); break; 
       case Bajo: client.println("<tr> <td align=""center"">Pasillo</td> <td align=""center"" bgcolor=""#04B404"">Apagado</td> </tr>"); break;
       default:   break; 
     }
        
     client.println("</table>");
     client.println("<br/><br/><br/>");
     client.println("</div>");
     
     //------- interfaz de confirmacion de la configuracion de hora  ----------
     client.println("<H4 align=""center"">");
     if (status_config_hora){ client.println("Hora configurada <br/>");}
     else if(!status_hora){client.println("No has definido la hora ! <br/>");}
     client.println("</H4>");
     
    //----- interfaz de confirmacion de la configuracion de temporizador-----------
     client.println("<H4 align=""center"">");
     if (status_config_temp)
     { 
        client.println("Temporizador configurada <br/>");
     }
     else if(!status_temp){client.println("No has definido ningun temporizador ! <br/>");}
     client.println("</H4>");

     client.println("<div style=""background-color:#0B0B61"" align=""center"">");
     
     client.println("<h3 style=""color:#FFFFFF"">Estado de los temporizadores</h3>"); 
     client.println("<table width=""50%"" border=""2"" align=""center"" cellspacing=""0"" bordercolor=""#000000"" bgcolor=""#084B8A"" style=""color:#FFFFFF"">");
     client.println("<tr><td align=""center"" > Hora actual</td> <td align=""center""> Info temporizadores</td></tr>");
     
     client.println("<tr>");
     client.println("<td rowspan=""3"" align=""center""><b>");   // hora actual
     if(hora[0]<10){client.println("0");}
     client.println(hora[0]);
     client.println(":");
     if(hora[1]<10){client.println("0");}
     client.println(hora[1]);
     client.println(":");
     if(hora[2]<10){client.println("0");}
     client.println(hora[2]);
     client.println("<b/></td>");
      
     client.println("<td align=""center""><b>Temporizador 1</b> <br/>");
     if (status_temp_out1)      //horas para temporizador 1
     { 
        client.println("Hora de apagarse:<br/>");
        if(hora_off1[0]<10){client.println("0");}
        client.println(hora_off1[0]);
        client.println(":");
        if(hora_off1[1]<10){client.println("0");}
        client.println(hora_off1[1]);
        client.println(":");
        if(hora_off1[2]<10){client.println("0");}
        client.println(hora_off1[2]);
        client.println("<br/>");

        client.println("Hora de Encenderse:<br/>");
        if(hora_on1[0]<10){client.println("0");}
        client.println(hora_on1[0]);
        client.println(":");
        if(hora_on1[1]<10){client.println("0");}
        client.println(hora_on1[1]);
        client.println(":");
        if(hora_on1[2]<10){client.println("0");}
        client.println(hora_on1[2]);
        
       }
     else { client.println("no definido!<br/>");}
     client.println("</td>");
     client.println("</tr>");

     client.println("<tr>");
     client.println("<td align=""center""><b>Temporizador 2</b><br/>");
     if (status_temp_out2)  //horas para temporizador 2
     {
        client.println("Hora de apagarse:<br/>");
        if(hora_off2[0]<10){client.println("0");}
        client.println(hora_off2[0]);
        client.println(":");
        if(hora_off2[1]<10){client.println("0");}
        client.println(hora_off2[1]);
        client.println(":");
        if(hora_off2[2]<10){client.println("0");}
        client.println(hora_off2[2]);
        client.println("<br/>");
        
        client.println("Hora de Encenderse:<br/>");
        if(hora_on2[0]<10){client.println("0");}
        client.println(hora_on2[0]);
        client.println(":");
        if(hora_on2[1]<10){client.println("0");}
        client.println(hora_on2[1]);
        client.println(":");
        if(hora_on2[2]<10){client.println("0");}
        client.println(hora_on2[2]);
     }
     else { client.println("no definido!<br/>");}
     client.println("</td>");
     client.println("</tr>");

     client.println("<tr>");
     client.println("<td align=""center""><b>Temporizador 3</b> <br/>");
     
     if (status_temp_out3)    //horas para temporizador 3
     {
        client.println("Hora de apagarse:<br/>");
        if(hora_off3[0]<10){client.println("0");}
        client.println(hora_off3[0]);
        client.println(":");
        if(hora_off3[1]<10){client.println("0");}
        client.println(hora_off3[1]);
        client.println(":");
        if(hora_off3[2]<10){client.println("0");}
        client.println(hora_off3[2]);
        client.println("<br/>");
        
        client.println("Hora de Encenderse:<br/>");
        if(hora_on3[0]<10){client.println("0");}
        client.println(hora_on3[0]);
        client.println(":");
        if(hora_on2[1]<10){client.println("0");}
        client.println(hora_on3[1]);
        client.println(":");
        if(hora_on3[2]<10){client.println("0");}
        client.println(hora_on3[2]);
     } 
     else { client.println("no definido!<br/>");}
     client.println("</td>");
     client.println("</tr>");
     client.println("</table>");
     client.println("<br/>");
     client.println("</div>");    
     
     client.println("<div style=""background-color:#0B0B61"">");
     client.println("<h4 style=""color:#FFFFFF""> Control help </h4>" );
     client.println("<p style=""color:#FFFFFF"" >");
     client.print(WiFi.localIP());
     client.println("/Outall_off = apaga todas las luces <br/>");
     client.print(WiFi.localIP());
     client.println("/Outall_on = prende todas las luces  <br/>");
     client.print(WiFi.localIP());
     client.println("/Out1_off = apaga la luz de la terraza  <br/>");
     client.print(WiFi.localIP());
     client.println("/Out2_on = prende la luz del Cuarto  <br/>");
     client.print(WiFi.localIP());
     client.println("/Out3_off = apaga la luz del pasillo <br/>");     
     client.println("</p> <br/><br/>");
     client.println("</div>");
          
     client.println("<div style=""background-color:#0B0B61"">");
     client.println("<h4 style=""color:#FFFFFF""> Config hora help </h4>" );
     client.println("<p style=""color:#FFFFFF"" >");
     client.print(WiFi.localIP());
     client.println("/Hora:hh:mm:ss <br/>");
     client.println(" formato de hora:<br/>");
     client.println(" hh = hora <br/>");
     client.println(" mm = minutos <br/>");
     client.println(" ss = segundos <br/>");
     client.println("</p> <br/><br/>");
     client.println("</div>");     

     client.println("<div style=""background-color:#0B0B61"">");
     client.println("<h4 style=""color:#FFFFFF""> Config temporizador help </h4>" );
     client.println("<p style=""color:#FFFFFF"" >");
     client.print(WiFi.localIP());
     client.println("/Temporizador_out1:hora de encender_out1 :hora de apagar_out1<br/>");
     client.print(WiFi.localIP());
     client.println("/Temporizador_clear= borra todos los temporizadores<br/>");
     client.println("<br/> Ejemplo:<br/>");
     client.println(" Temporizador_out1:06:12:12 :12:12:12<br/>");
     client.println(" Esto activa temporizador para OUT1(terraza) para encenderse a las 6:12:12 y apagarse a las 12:12:12 <br/>");
     client.println("</p> <br/><br/>");
     client.println("</div>");

     client.println("<div style=""background-color:#0B0B61"">");
     client.println("<h4 style=""color:#FFFFFF""> help </h4>" );
     client.println("<p style=""color:#FFFFFF"" >");
     client.print(WiFi.localIP());
     client.println("/vista_out");
     client.println("</p> <br/><br/>");
     client.println("</div>");

     client.println("<p style=""color:#FFFFFF""><b> Copyright Ã‚Â© 2018 by Nelson Jhoann concha Canto <br/> All rights reserved</b> </p>");
      
     client.println("</body>");
     client.println("</html>");   
}



