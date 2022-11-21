#include <DHTesp.h>
#include <WiFi.h>

DHTesp dht;

#define TEMP_PIN 13
uint32_t lastTime = 0;
uint32_t timerDelay = 5000;

double temp = 0;
String header;

const char* ssid = "esp";
const char* pass = "12345678";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  dht.setup(TEMP_PIN, DHTesp::DHT11);
  WiFi.softAP(ssid, pass);
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    TempAndHumidity data = dht.getTempAndHumidity();
    temp = data.temperature;
    Serial.println("Temperatura = ");
    Serial.print(temp);
    Serial.println("");
    Serial.println(WiFi.localIP());
    lastTime = millis();
    
    /*
    int networks = WiFi.scanNetworks();
    if (networks == 0) {
      Serial.println("No Networks");
    } else {
      Serial.print(networks);
      Serial.println(" networks found.");
      for (int i = 0; i < networks; i++) {
        Serial.println("SSID: ");
        Serial.print(WiFi.SSID(i));
        Serial.println("");
      }
    }
    */
    WiFiClient client = server.available();
    if (client) {
      Serial.print("New Client");
      String currentLine = "";
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          Serial.write(c);
          header += c;
          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              client.println("Temperature: ");
              client.println(temp);
              break;
            } else {
              currentLine = "";
            }
          } else if (c != '\r') {
            currentLine += c;
          }
        }
      }
      client.stop();
      Serial.println("Client Disconnected");
      Serial.println();
    }
  }

}
