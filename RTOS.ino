// Esteban Bernal, Juliana Jiménez, Brian Mendez, Eduardo Meriño y Zharick Mora.
#include <WiFi.h>
#include <HTTPClient.h>


const char* ssid = "ZonaEBC";
const char* password = "123456789";


String apiKey = "BR4UBPMLRJO7QXTT";
const char* server = "http://api.thingspeak.com/update";


#define MQ135_PIN 34
#define LED_PIN 2


int lastADC = 0;
float lastVoltage = 0.0;


void TaskBlink(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  while (true) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1 s
  }
}


void TaskReadMQ135(void *pvParameters) {
  analogReadResolution(12);
  while (true) {
    int adcValue = analogRead(MQ135_PIN);
    float voltage = adcValue * (3.3 / 4095.0);

    lastADC = adcValue;
    lastVoltage = voltage;

    char airQuality[20];
    if (adcValue < 1000) strcpy(airQuality, "Aire BUENO");
    else if (adcValue < 2000) strcpy(airQuality, "Aire REGULAR");
    else strcpy(airQuality, "Aire MALO");

    Serial.printf("MQ135 -> ADC: %d | Voltaje: %.2f V | Estado: %s\n",
                  adcValue, voltage, airQuality);

    vTaskDelay(2000 / portTICK_PERIOD_MS);  // 2 s
  }
}


void TaskSendThingSpeak(void *pvParameters) {
  while (true) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String(server) + "?api_key=" + apiKey +
                   "&field1=" + String(lastADC) +
                   "&field2=" + String(lastVoltage, 2);

      http.begin(url);
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("ThingSpeak HTTP: %d\n", httpCode);
      } else {
        Serial.printf("Error al enviar: %s\n",
                      http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.println("WiFi desconectado, intentando reconectar...");
      WiFi.reconnect();
    }

    vTaskDelay(5000 / portTICK_PERIOD_MS);  // 5 s
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println("\n=== ESP32 + MQ135 + LED + ThingSpeak (RTOS) ===");
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  
  xTaskCreatePinnedToCore(TaskBlink, "Blink", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskReadMQ135, "MQ135", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskSendThingSpeak, "ThingSpeak", 4096, NULL, 1, NULL, 1);
}

void loop() {
}
