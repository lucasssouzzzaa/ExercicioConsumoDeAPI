// #include <Arduino.h>
// #include <WiFi.h>
// #include <HTTPClient.h>

// // Prototipo das funcoes
// void conexaoWifi();
// void reconexaoWifi();

// const char *SSID = "SENAI EDUCACIONAL";
// const char *SENHA = "info@134";

// const unsigned long tempoConexao = 10000;
// const unsigned long tempoReconexao = 5000;

// const String enderecoApi = "https://wttr.in/Sao+Caetano+do+Sul?lang=pt&format="

// float teperatura, sensacaoTermica;
// int umidade, pressao;
// String condica, nascerSol

// void obterDadosApi() {
//   if (WiFi.status() == WL_CONNECTED) {
//     HTTPClient http;
//     http.begin(enderecoApi);
//     int httpResponseCode = http.GET();

//     if (httpResponseCode > 0) {
//       String payload = http.getString();
//       Serial.println(payload);
//       // Aqui você pode processar o payload para extrair os dados desejados
//     } else {
//       Serial.printf("Erro na requisição: %d\n", httpResponseCode);
//     }
//     http.end();
//   } else {
//     Serial.println("WiFi não conectado");
//   }
// }

// void setup() {
//   Serial.begin(115200);

//   // Conexao WIfi
//   conexaoWifi();

// }

// void loop() {
//   reconexaoWifi();
// }

// void conexaoWifi(){
//   Serial.printf("Conectando ao wifi: %s", SSID);

//   WiFi.begin(SSID, SENHA);

//   unsigned long tempoWifi = millis();

//   while (WiFi.status() != WL_CONNECTED && millis() - tempoWifi < tempoConexao)
//   {
//     Serial.print(".");
//     delay(500);
//   }

//   if (WiFi.status() == WL_CONNECTED)
//   {
//     Serial.println("Wifi conectado com sucesso!");
//     Serial.print("Endereço IP: ");
//     Serial.println(WiFi.localIP());
//   }
//   else
//   {
//     Serial.println("Falha ao se conectar");
//   }
// }

// void reconexaoWifi(){
//   unsigned long tempoAtual = millis();
//   unsigned long tempoUltimaConexao = 0;

//   if (tempoAtual - tempoUltimaConexao > tempoReconexao)
//   {
//     if (WiFi.status() != WL_CONNECTED)
//     {
//       Serial.println("Conexao Wifi perdida");
//       conexaoWifi();
//     }
//     tempoUltimaConexao = tempoAtual;
//   }
// }

// String consultaApi(String codigo) {
//   HTTPClient http;
//   int codigoHttp = http.GET();
//   String resposta = "";

//   if (codigoHttp == 200) {
//     resposta = http.getString();
//   } else {
//     Serial.printf("Erro na requisição: %d\n", codigoHttp);
//     }

//     http.end();
//     return resposta;
// }


// Exercicio
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ezTime.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "SENAI EDUCACIONAL";
const char* password = "info@134";
const char* endpoint = "https://wttr.in/Sao+Caetano+do+Sul?lang=pt&format=";

LiquidCrystal_I2C lcd(0x27, 16, 2);
Timezone myTZ;

unsigned long ultimaLeitura = 0;
const long intervalo = 10000;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  waitForSync();
  myTZ.setLocation("America/Sao_Paulo");
}

void loop() {
  unsigned long agora = millis();

  if (agora - ultimaLeitura >= intervalo) {
    ultimaLeitura = agora;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(endpoint);
      int httpCode = http.GET();

      if (httpCode > 0) {
        String payload = http.getString();
        
        StaticJsonDocument<1024> doc;
        deserializeJson(doc, payload);

        const char* pressao = doc["pressao"];
        const char* condicao = doc["condicao"];
        const char* nascerSol = doc["nascerSol"];

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Pres: ");
        lcd.print(pressao);

        lcd.setCursor(0, 1);
        lcd.print(condicao);

        Serial.println("Pressao: " + String(pressao));
        Serial.println("Condicao: " + String(condicao));
        Serial.println("Nascer Sol: " + String(nascerSol));
        Serial.println("--------------------");
      }
      http.end();
    }
  }
}