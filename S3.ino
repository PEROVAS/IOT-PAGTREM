
#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

/* 
   configurações de rede
    */
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";

/* 
   configuração mqtt (hive mqtt)
   */
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

/* 
   pinos do led e servos
*/
#define LED_PIN 2
#define SERVO1_PIN 15   // GPIO recomendado para servo

Servo servo1;

/* 
   função: Conectar ao WiFi
    */
void setup_wifi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

/* 
   callback mqtt
   - Executada quando chega mensagem nos tópicos inscritos
    */
void callback(char* topic, byte* message, unsigned int length) {
  String msg = "";

  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }

  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  Serial.println(msg);

  /* 
     controle de iluminação (Tópico: S1_llum)
      */
  if (String(topic) == "S1_llum") {
    if (msg == "1") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED LIGADO via MQTT");
    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED DESLIGADO via MQTT");
    }
  }

  /* 
     controle do servo 1  (Tópico: S3_Presence_1)
      */
  if (String(topic) == "S3_Presence_1") {
    int angulo = msg.toInt();  // converte texto para número

    if (angulo >= 0 && angulo <= 180) {
      servo1.write(angulo);
      Serial.print("Servo1 movido para: ");
      Serial.println(angulo);
    } else {
      Serial.println("Valor inválido para servo!");
    }
  }
}


