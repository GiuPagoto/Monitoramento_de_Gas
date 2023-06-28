#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "PAGOTO";
const char* password = "giogiu26";
const char* mqttServer = "pagoto.duckdns.org";
const int mqttPort = 1883;
const char* mqttUser = "giu";
const char* mqttPassword = "Amendoin14";
const char* mqttTopicgas_natural = "esp32/gas_natural";
const char* mqttTopicgas_petroleo = "esp32/gas_petroleo";
const int sensorPingas_natural = 36; // Potenciômetro do GN
const int sensorPingas_petroleo = 35; // Potenciômetro do GLP

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi com endereço IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Falha na conexão - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  int sensorValuegas_natural = analogRead(sensorPingas_natural);
  int sensorValuegas_petroleo = analogRead(sensorPingas_petroleo);

  float gas_natural=((float)sensorValuegas_natural/4095)*100;
  float gas_petroleo=((float)sensorValuegas_petroleo/4095)*100;

  String gas_naturalPayload = String(gas_natural);
  String gas_petroleoPayload = String(gas_petroleo);

  Serial.print("Enviando dados para o Node-RED - gas_natural: ");
  Serial.println(gas_natural);
  client.publish(mqttTopicgas_natural, gas_naturalPayload.c_str());

  Serial.print("Enviando dados para o Node-RED - gas_petroleo: ");
  Serial.println(gas_petroleo);
  client.publish(mqttTopicgas_petroleo, gas_petroleoPayload.c_str());

  delay(1000);
}
