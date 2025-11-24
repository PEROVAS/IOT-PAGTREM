/***********************************************************
 *  Código do Nó S4 (Trem)
 *  Criado por ChatGPT a pedido do usuário
 *
 *  Funções:
 *   - Conexão WiFi + HiveMQ MQTT
 *   - Controle da ponte-H (motor do trem)
 *   - Controle do LED RGB status
 *   - Inscrição nos tópicos:
 *        Trem_Motor
 *        Trem_StatusRGB
 ***********************************************************/

#include <WiFi.h>
#include <PubSubClient.h>

/* =============================
   CONFIGURAÇÃO WIFI
   ============================= */
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";

/* =============================
   CONFIGURAÇÃO MQTT (HIVEMQ)
   ============================= */
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

/* =============================
   PINOS DA PONTE-H (motor trem)
   ============================= */
#define IN1 26     // controle direção A
#define IN2 27     // controle direção B
#define ENA 14     // controle velocidade (PWM)

/* =============================
   PINOS DO LED RGB STATUS
   ============================= */
#define LED_R 4
#define LED_G 16
#define LED_B 17

/* =============================
   FUNÇÃO DE CONEXÃO WIFI
   ============================= */
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

/* =====================================================
   FUNÇÃO CALLBACK MQTT
   -> Executa quando chega mensagem dos tópicos inscritos
   ===================================================== */
void callback(char* topic, byte* msg, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) message += (char)msg[i];

  Serial.print("Mensagem recebida em: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  Serial.println(message);

  /* ----------------------------------------
      CONTROLE DO MOTOR (Trem_Motor)
      comandos possíveis:
       STOP
       FORWARD
       BACKWARD
       SPEED:XXX (0 a 255)
     ---------------------------------------- */
  if (String(topic) == "Trem_Motor") {

    if (message == "STOP") {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 0);
      Serial.println("TREM PARADO");
    }

    if (message == "FORWARD") {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 200);  // velocidade padrão
      Serial.println("TREM EM FRENTE");
    }

    if (message == "BACKWARD") {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, 200);
      Serial.println("TREM EM RÉ");
    }

    if (message.startsWith("SPEED:")) {
      int vel = message.substring(6).toInt();
      analogWrite(ENA, vel);
      Serial.print("Velocidade ajustada para: ");
      Serial.println(vel);
    }
  }

  /* ----------------------------------------
      CONTROLE DO LED RGB (Trem_StatusRGB)
      Exemplo:
        RED
        GREEN
        BLUE
        OFF
     ---------------------------------------- */
  if (String(topic) == "Trem_StatusRGB") {

    if (message == "RED") {
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, LOW);
    }

    if (message == "GREEN") {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_B, LOW);
    }

    if (message == "BLUE") {
      digitalWrite(LED_R, LOW_
