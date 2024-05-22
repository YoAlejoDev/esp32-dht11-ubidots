/******************************************
 *
 * Codigo de UBIDOTS
 *
 * Developed by Alejandro Hurtado (https://github.com/YoAlejoDev)
 *
 * ****************************************/

/****************************************
 * Librerias incluidas
 ****************************************/
#include "UbidotsEsp32Mqtt.h"
#include <DHT.h>

/****************************************
 * Constantes importantes para Ubidots
 ****************************************/
const char *UBIDOTS_TOKEN = "BBUS-TvT1flj8M1m5gHE7T7v2T5Dt9mJ8Pl";
const char *WIFI_SSID = "Alejo's Phone";      // Nombre del WIFI
const char *WIFI_PASS = "1234567890..";      // Clave del WIFI
const char *DEVICE_LABEL = "Esp32Test";   // Nombre del dispositivo en Ubidots
const char *VARIABLE_LABEL = "Esp32 Temperatura"; // Nombre de la variable en Ubidots
const char *VARIABLE_LABEL_2 = "Esp32 Humedad"; // Nombre de la variable en Ubidots

#define DHTPIN 13 // Pin del sensor
#define DHTTYPE DHT11 // Tipo de sensor

unsigned long timer; // Timer para la lectura de datos

Ubidots ubidots(UBIDOTS_TOKEN); // Instancia de la clase Ubidots
DHT dht(DHTPIN, DHTTYPE); // Instancia de la clase DHT

// Global variables

double h;
double t;

/****************************************
 * Funciones principales
 ****************************************/

void setup()
{
  // Envio de datos a Ubidots
  Serial.begin(9600);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setup();
  ubidots.reconnect();

  timer = millis();
  dht.begin();
}

void loop()
{
  // Lectura de datos
  h = dht.readHumidity();
  t = dht.readTemperature();

  // Verificacion de la conexion
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  // Verificacion de la lectura de datos
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read data"));
  }

  const double temp = t*10;

  ubidots.add(VARIABLE_LABEL, temp); // Enviar temperatura
  ubidots.add(VARIABLE_LABEL_2, h); // Enviar humedad
  ubidots.publish(DEVICE_LABEL); // Publicar datos
  timer = millis();
  ubidots.loop(); // Mantener la conexion

  // Mostrar datos en el monitor serial
  Serial.println(F("Humedad: "));
  Serial.println(h);
  Serial.println(F("Temperature: "));
  Serial.println(temp);
  Serial.println("");

  // Esperar 2 segundos para la siguiente lectura
  delay(2000);
}