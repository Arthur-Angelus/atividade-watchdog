#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "Jujutsu kaisen";
const char* password = "sonochinosadame";

const char* serverName = "https://jsonplaceholder.typicode.com/todos/1";

int ledVermelho = 19;
int ledVerde = 18;

void setup(){
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  Serial.begin(115200);
  Serial.print("conectando a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20){
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status () == WL_CONNECTED){
    Serial.println("\nconectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI (sinal): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\nFalha na conexão!");
  }
}

void loop(){
  if (WiFi.status() == WL_CONNECTED){
    WiFiClientSecure *client = new WiFiClientSecure;
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);
  
  if(client){
    client->setInsecure();

    HTTPClient https;

    Serial.print("[HTTPS] Iniciando requisição...\n");

      if(https.begin(*client, serverName)){

        int httpCode = https.GET();

        if(httpCode > 0){
          Serial.printf("[https] Código de resposta: %d\n", httpCode);

          if(httpCode == HTTP_CODE_OK){
            String payload = https.getString();
            Serial.println("Conteudo recebido:");
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTPS] Falha na requisição, erro: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      } else {
        Serial.printf("[HTTPS] não foi possivel conectar ao servidor\n");
      }

      delete client;
    }
  }
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Reconectando...");
    WiFi.reconnect();
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
    delay(1000);
    digitalWrite(ledVermelho, LOW);
  }
  delay(10000);
}