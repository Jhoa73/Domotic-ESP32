#define HDC1080_C 0
#define OUT_C 1
// pine status led
const int led =  2;

//parametros de la red
#define MaxCaracteresRED 10
String ssid="";
String password="";

const char *my_ssid = "ESP32";
const char *my_password = "testpassword";

const char* host = "api.thingspeak.com";
String api_key = "IWEXC266NHIHJ8NO"; 

//IPAddress local_IP(192, 168, 0, 18);
//IPAddress gateway(192, 168, 0, 1);
//IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(8, 8, 8, 8); //optional
//IPAddress secondaryDNS(8, 8, 4, 4); //optional
#define Wifi_disconect WiFi.status() != WL_CONNECTED   
#define Wifi_conect WiFi.status() == WL_CONNECTED  

unsigned int hora[3]={0,0,0};        //variables alacena hora hh,mm,ss

//bandera estados de peticiones
boolean status_router=0;			 //estado del router EEPROM
boolean status_config_router=0;      //badera para indicar que se configuro router correctamente por peticion.
boolean status_config_routerClear=0; //bandera de peticion para borrar datos de router

boolean status_post_thingspeak=0;  //peticion de post a base thingspeak

//buffer de peticion
#define clear_buffer memset(buffer_E,0,size_buffer);
#define size_buffer 400
#define exep_1 size_buffer>k_buffer      //exepcion que reinicia ESP32 rebotin RST(0x0C)
char buffer_E[size_buffer];
unsigned int k_buffer=0;
char c;

boolean currentLineIsBlank=true;
#define cerrar_cliente delay(100); client.stop(); delay(500); Serial.println("client disconnected");

WiFiServer server(80);

ClosedCube_HDC1080 hdc1080;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile int Bandera_time;

//EEPROM direcciones
#define BanderaConfig_RED 0           				//0   Direcciones (#define MaxCaracteresRED 10)
#define BanderaConfig_Frecuencia 1    				//1
#define Frecuencia_DEE 2              				//2  
#define Password_DEE 3                				//3->13
#define SSID_DEE   14                        //14-24

#define EEPROM_SIZE 100
#define endwriteEEPROM EEPROM.commit();

#define Alto 1
#define Bajo 0
#define LED_ALTO digitalWrite(led,HIGH);
#define LED_BAJO digitalWrite(led,LOW);

//---------------------domotica----------------------------
#define JSON_OPCION strstr(buffer_E,"/domotica/")
// pines para salida
const int out1 =  16;    
const int out2 =  17;
const int out3 =  5;

//bandera de configuracion de hora manual
boolean status_hora=0;             // badera para indicar que no ha definido hora
boolean status_config_hora=0;      //bandera para indicar que se configuro hora
boolean status_config_horaJSON=0;
// estatus de temporizador
boolean status_temp=0;              // badera para indicar que no ha definido temporizador
boolean status_config_temp=0;       //bandera para indicar que se configuro temporizador
boolean status_config_tempJSON=0;
boolean status_config_tempClear=0;  //bandera indica peticion para borrar es estado de configuraciones del temp
boolean status_config_tempClearJSON=0;
boolean status_temp_out1=0;		    //banderas para indicar que se configuro n temporizador	
boolean status_temp_out2=0;
boolean status_temp_out3=0;
//control
boolean status_controlOut=0;       //bandera para indicar peticion de vista_out
boolean status_controlOutJSON=0;
boolean Status_Out1;			   //bandera para indicar si esta encendido o apagado outN	
boolean Status_Out2;
boolean Status_Out3;
//varibles donde se guarda hora de apagar y encender
unsigned int hora_off1[3],hora_off2[3],hora_off3[3];   
unsigned int hora_on1[3],hora_on2[3],hora_on3[3];

#define OUT1_ALTO digitalWrite(out1,HIGH);
#define OUT1_BAJO digitalWrite(out1,LOW);
#define OUT2_ALTO digitalWrite(out2,HIGH);
#define OUT2_BAJO digitalWrite(out2,LOW);
#define OUT3_ALTO digitalWrite(out3,HIGH);
#define OUT3_BAJO digitalWrite(out3,LOW);

#define ENCENDER_OUT1 OUT1_BAJO Status_Out1=Alto;
#define ENCENDER_OUT2 OUT2_BAJO Status_Out2=Alto;
#define ENCENDER_OUT3 OUT3_BAJO Status_Out3=Alto;
#define ENCENDER_ALL ENCENDER_OUT1 ENCENDER_OUT2 ENCENDER_OUT3

#define APAGAR_OUT1 OUT1_ALTO Status_Out1=Bajo;
#define APAGAR_OUT2 OUT2_ALTO Status_Out2=Bajo;
#define APAGAR_OUT3 OUT3_ALTO Status_Out3=Bajo;
#define APAGAR_ALL  APAGAR_OUT1 APAGAR_OUT2 APAGAR_OUT3

#define TEMP_APAGA1 if(hora[0]==hora_off1[0] && hora[1]== hora_off1[1]){APAGAR_OUT1}
#define TEMP_APAGA2 if(hora[0]==hora_off2[0] && hora[1]== hora_off2[1]){APAGAR_OUT2}
#define TEMP_APAGA3 if(hora[0]==hora_off3[0] && hora[1]== hora_off3[1]){APAGAR_OUT3}

#define TEMP_ENCIENDE1 if(hora[0]==hora_on1[0] && hora[1]== hora_on1[1]){ENCENDER_OUT1}
#define TEMP_ENCIENDE2 if(hora[0]==hora_on2[0] && hora[1]== hora_on2[1]){ENCENDER_OUT2}
#define TEMP_ENCIENDE3 if(hora[0]==hora_on3[0] && hora[1]== hora_on3[1]){ENCENDER_OUT3}


//-------------------------estacion--------------------------------

#define numero_muestras_Sensor 20

unsigned int Limite_minutosForDia=((23*59)+59);

boolean s_frecuencia=0;            			// indica que se configuro la frecuencia post
boolean s_Configfrecuencia_post=0;  		//peticion de configuracion frecuencia post
boolean s_send_postFrecuencia=0;   			//peticion de post por frecuencia
boolean s_Configfrecuencia_postClear=0;		//indica peticion para borrar frecuencia de envio
 
unsigned int tiempoParaenvio=0;  			// variables para control en frecuencia de envio
unsigned int frecuencia=1;          		//frecuencia en minutos

//variables sensor HDC1080
float Temperatura=0.0;
float Humedad=0.0;
//-----------------------------JSON-------------------------------------------



