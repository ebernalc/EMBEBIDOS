#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "ZonaEBC";
const char* password = "123456789";


String apiKey = "af10085fc6ecef5902568a0d04813467";
String city = "Barranquilla";
String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric&lang=es";

void setup() {
  Serial.begin(115200);
  delay(1000);


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
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    Serial.println(" Conectando a OpenWeather...");
    http.begin(serverPath);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("Respuesta del servidor:");
      Serial.println(payload);

      // Usamos ArduinoJson para parsear
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float temperatura = doc["main"]["temp"];
        int humedad = doc["main"]["humidity"];
        const char* clima = doc["weather"][0]["description"];

        Serial.println("===== CLIMA =====");
        Serial.print("Ciudad: ");
        Serial.println(city);
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" °C");
        Serial.print("Humedad: ");
        Serial.print(humedad);
        Serial.println(" %");
        Serial.print("Descripción: ");
        Serial.println(clima);
        Serial.println("====================");
      } else {
        Serial.println("Error parseando JSON");
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
