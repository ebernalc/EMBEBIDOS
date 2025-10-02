//Bernal Esteban, Jiménez Juliana, Meriño Eduardo, Mendez Brian y Mora Zharick
//Hay que hacer cambios a la board al momento de subirlo, quitarle el espacio asignado al OTA para darselo al programa
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const char* ssid = "ZonaEBC" ;
const char* password = "123456789";


String apiKey = "af10085fc6ecef5902568a0d04813467";
String city = "Barranquilla";
String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric&lang=es";

void setup() {
  Serial.begin(115200);
  delay(1000);

  SerialBT.begin("ESP32_LED_Control");
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado al WiFi!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());
  Serial.println("El ESP32 está listo. Conéctate por Bluetooth.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverPath);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      
      
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float temperatura = doc["main"]["temp"];
        int humedad = doc["main"]["humidity"];
        const char* clima = doc["weather"][0]["description"];

        if (SerialBT.available()) {
          String comando = SerialBT.readStringUntil('\n');
          comando.trim();
          Serial.print("Comando recibido: ");
          Serial.println(comando);
          
          if (comando == "GET_CLIMA") {
            Serial.printf("Ciudad: %s\n", city);
            Serial.printf("Temperatura: %.1f °C\n", temperatura);
            Serial.printf("Humedad: %d %%\n", humedad);
            Serial.printf("Descripción: %s\n", clima);
          } 
          else if (comando == "GET_TEMPERATURA") {
            Serial.printf("Temperatura: %.1f °C\n", temperatura);
          } 
          else if (comando == "GET_HUMEDAD") {
            Serial.printf("Humedad: %d %%\n", humedad);
          } 
          else if (comando == "GET_DES") {
            Serial.printf("Descripción: %s\n", clima);
          } 
          else {
            Serial.println("Comando no válido");
          }
        }

    } else {
      Serial.print("Error en la petición HTTP: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi desconectado");
  }

  delay(1000);
  }
}
