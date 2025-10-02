//Bernal Esteban, Jiménez Juliana, Meriño Eduardo, Mendez Brian y Mora Zharick
#include "BluetoothSerial.h"

// Objeto para comunicación Bluetooth
BluetoothSerial SerialBT;

// Pines de los LEDs
const int led1 = 2;   // Puedes cambiar el pin
const int led2 = 4;   // según tu conexión
const int led3 = 5;

void setup() {
  // Inicializar comunicación Bluetooth
  Serial.begin(115200);
  SerialBT.begin("ESP32_LED_Control"); // Nombre del dispositivo BT

  // Configurar pines como salida
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Inicialmente apagados
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  Serial.println("El ESP32 está listo. Conéctate por Bluetooth.");
}

void loop() {
  if (SerialBT.available()) {
    char comando = SerialBT.read();  // Leer el comando enviado
    Serial.print("Comando recibido: ");
    Serial.println(comando);

    switch (comando) {
      case '0': // Apagar todos
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        Serial.println("Todos los LEDs apagados");
        break;

      case '1': // Solo LED1
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        Serial.println("LED 1 encendido");
        break;
      
      case '2': // solo LED2
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, LOW);
        Serial.println("LED 2 encendido");
        break;

      case '3': // solo LED3
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        Serial.println("LEDs 3 encendido");
        break;

      case '4': // LED1 y LED2
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, LOW);
        Serial.println("LED 1 y 2 encendidos");
        break;

      case '5': // LED1, LED2 y LED3
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        Serial.println("Los 3 LEDs encendidos");
        break;
    
      default:
        Serial.println("Comando no válido. Usa 0, 1, 2, 3, 4 0 5.");
        break;
    }
  }
}
