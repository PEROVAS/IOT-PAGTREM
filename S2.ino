

#include <WiFi.h>
#include <PubSubClient.h>

/* ============================
   CONFIGURAÇÕES DE REDE WI-FI
   ============================ */
const char* ssid = "SEU_WIFI";          // nome da rede WiFi
const char* password = "SENHA_WIFI";    // senha da rede WiFi

/* ============================
   CONFIGURAÇÕES MQTT (HIVEMQ)
   Broker público HiveMQ:
   broker.hivemq.com | porta 1883
   ============================ */
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Criação dos objetos
WiFiClient espClient;
PubSubClient client(espClient);

/* ============================
   PINOS DO SENSOR ULTRASSONICO
   ============================ */
#define TRIG_PIN 5
#define ECHO_PIN 18

/* ============================
   PINO DO LED DE ILUMINAÇÃO
   (controlado pelo tópico MQTT)
   ============================ */
#define LED_PIN 2   // LED azul do seu diagrama

/* ============================
   FUNÇÃO: Conectar ao WiFi
   ============================ */
void setup_wifi() {
  delay(10);
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

/* ==========================================================
   FUNÇÃO CALLBACK MQTT
   - Executada sempre que chega mensagem em um tópico inscrito
   - Aqui controlamos a ILUMINAÇÃO (LED)
   ========================================================== */
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  Serial.println("Conteúdo: " + msg);

  // Controle de iluminação
  if (String(topic) == "S1_llum") {
    if (msg == "1") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Luz LIGADA por MQTT");
    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println("Luz DESLIGADA por MQTT");
    }
  }
}
