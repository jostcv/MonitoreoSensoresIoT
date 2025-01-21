#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configuración del DHT11
#define DHTPIN 4     // Pin donde está conectado el DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configuración del LED
#define LED_PIN 15
#define VENTILADOR_ENA 18
#define VENTILADOR_IN1 19
#define VENTILADOR_IN2 5

// Configuración de la red Wi-Fi
const char* ssid = "iPhone de Josué patricio";       // Reemplaza con el nombre de tu red Wi-Fi
const char* password = "12345678"; // Reemplaza con la contraseña de tu red Wi-Fi

// Configuración de ThingsBoard y MQTT
const char* mqttServer = "172.20.10.3"; // Cambiar si usas un servidor personalizado
const int mqttPort = 1883;
const char* token = "2cxplXI9UuL8SeQC2yne";  // Reemplaza con el token de tu dispositivo en ThingsBoard

WiFiClient espClient;
PubSubClient client(espClient);

void conectarWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conectando a WiFi...");
    WiFi.begin(ssid, password);
    int intentos = 0;
    while (WiFi.status() != WL_CONNECTED && intentos < 20) {
      delay(1000);
      Serial.print(".");
      intentos++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConexión a WiFi exitosa");
      Serial.print("Dirección IP: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nNo se pudo conectar a WiFi. Verifica el SSID y contraseña.");
    }
  }
}

void conectarMQTT() {
  if (!client.connected()) {
    Serial.println("Conectando a ThingsBoard...");
    while (!client.connected()) {
      if (client.connect("ESP32", token, NULL)) {
        Serial.println("Conexión a ThingsBoard exitosa");
      } else {
        Serial.print("Error de conexión MQTT, rc=");
        Serial.print(client.state());
        Serial.println(" Intentando nuevamente en 5 segundos...");
        delay(5000);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Inicializar DHT11
  dht.begin();

  // Inicializar el LED y el ventilador
  pinMode(LED_PIN, OUTPUT);
  pinMode(VENTILADOR_ENA, OUTPUT);
  pinMode(VENTILADOR_IN1, OUTPUT);
  pinMode(VENTILADOR_IN2, OUTPUT);

  // Conexión inicial
  conectarWiFi();
  client.setServer(mqttServer, mqttPort);
  conectarMQTT();
}

void loop() {
  conectarWiFi(); // Reintentar WiFi si es necesario
  conectarMQTT(); // Reintentar MQTT si es necesario

  client.loop(); // Mantener la conexión MQTT

  // Leer datos del sensor DHT11
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error al leer el DHT11, reintentando...");
    delay(2000); // Esperar antes de reintentar
    return;
  }

  // Mostrar los datos en el Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  // Controlar el LED y el ventilador
  if (temperatura > 27) {
    digitalWrite(LED_PIN, HIGH); // Encender LED
    digitalWrite(VENTILADOR_ENA, HIGH); // Activar ventilador
    digitalWrite(VENTILADOR_IN1, HIGH);
    digitalWrite(VENTILADOR_IN2, LOW);
  } else {
    digitalWrite(LED_PIN, LOW); // Apagar LED
    digitalWrite(VENTILADOR_ENA, LOW); // Apagar ventilador
  }

  // Crear un JSON con los datos
  String payload = "{\"temperatura\":";
  payload += String(temperatura);
  payload += ",\"humedad\":";
  payload += String(humedad);
  payload += "}";

  // Publicar los datos en ThingsBoard
  if (client.connected()) {
    client.publish("v1/devices/me/telemetry", payload.c_str());
  } else {
    Serial.println("No se pudo publicar en MQTT. Cliente no conectado.");
  }

  // Esperar antes de la próxima lectura
  delay(2000);
}
