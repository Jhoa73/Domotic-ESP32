//--------------api--------------------
void IRAM_ATTR onTimer() ;

void setup_api();
void loop_api();
void init_device();
void init_EEPROM();
//--servicios--
int config_WIFI_password(char *buffer_N);

int test_conexion();
void info_device_red();
void interfaz_LED_conect();

void servidor();
void peticiones(char *buffer_E);
char read_buffer_wifi(char * buffer_E, WiFiClient client);
void JSON_respuesta(WiFiClient client);

void init_EEPROMred();
void write_EEPROMred();

void print_hora();
void Obten_hora_post(String line);
int hora_GMT_6(unsigned int hora, boolean Status);

boolean segundos_minutos();
boolean minutos_horas();
boolean dia();

char CharMayus(char c);
//-------------domotica-----------------
void loop_out();
void init_out();
void temporizador_out();
//--servicios--
int def_control_Wifi_out(char * buffer_E);
int clear_status_temporizador_out(char * buffer_E);
int controlSatus_temporizador_out(char * buffer_E);
int def_hora_Wifi(char * buffer_E);

void HTML_respuesta_out(WiFiClient client);

char split_hora(char * buffer_E, int initial_position, char *StoI);
unsigned int horaStoI(char * StoI, unsigned int * hora);
//------------------estacion---------------------
void loop_weatherStation();
void init_sensor();

int POST_servicio();
int view_answer_post(WiFiClient client);
void interfaz_LED_POST(int estado);
//--servicios--
int POST_thingspeak();
int ConfigFrecuecniaPost(char *buffer_N);

unsigned int horasTominutos(unsigned int hora); 
void controlPost_frecuencia();
void init_EEPROMfrecuenicia();
void write_EEPROMfrecuencia();

void read_HDC1080();
float promedio_read_sensor(char *sensor, int muestra);
void printSerialNumber();

//---------------JSON_RESPUESTA--------------------
void JSON_infoESP32(WiFiClient client);

void JSONconfig_WIFI_password(WiFiClient client);
void JSONclearRED(WiFiClient client);

void JSONdef_control_Wifi_out(WiFiClient client);
void JSONclear_status_temporizador_out(WiFiClient client);
void JSONcontrolSatus_temporizador_out(WiFiClient client);
void JSOdef_hora_Wifi(WiFiClient client);

void JSONPOST_thingspeak(WiFiClient client);
void JSONConfigFrecuecniaPost(WiFiClient client);
void JSONclearFrecuencia(WiFiClient client);


