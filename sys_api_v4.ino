                                                                                                                                                                                                                                                       #include <dummy.h>
/**
 * Autor: Nelson Jhoann Concha Canto
 * Version :4
 * fecha: 15/03/18
 * 
 * Sytem Esp32-32bits-IoT-Core Arduino
 * API:
 *    -administra los modulos
 *    -EEPROM
 *    -config Router
 *    - configuracion de hora (implementado  GMT-6 with/withOut horario verano)
 *    -server
 * Modulo de domotica
 *      -control de salidas
 *     -control de temporizadores
 *  Modulo  weatherStation
 *     -control de frecuencia envio
 *     -post al servicio de almacen
 *     -parametros del sensor HDC0180
 *     
 *Status: En prueba.     
 */

#include <Arduino.h>
#include <WiFi.h>
#include <string.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
#include "EEPROM.h"

#include "cabecera.h"
#include "globales.h"

#include "JSON_respuestas.h"
#include "domotica.h"
#include "weatherStation.h"
#include "api.h"

void setup() 
{
 	setup_api();
}

void loop()
{
 	loop_api();    
}



