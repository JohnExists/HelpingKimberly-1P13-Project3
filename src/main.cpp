#include <Arduino.h>
#include<WiFi.h>
#include<WebServer.h>
#include<WebSocketsServer.h>

#include"website.h"
#include"password.h"


#define TRIG_PIN 18
#define ECHO_PIN 19

IPAddress currentAddress;
WebServer server(80);
WebSocketsServer socket = WebSocketsServer(81);

const int INTERVAL = 1500;
unsigned long previousMillis = 0;

void setupSensor()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

float readSensor()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  
    float duration = pulseIn(ECHO_PIN, HIGH);
    return duration / 58.2;
}

IPAddress connectWiFi(const char* SSID, const char* PASSWORD)
{
  Serial.println("I am not connected so I am sad :(");
  Serial.print("Connecting To ");
  Serial.print(SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(100);
  }

  Serial.println("I am connected so I am happy :)");
  Serial.println("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  return WiFi.localIP();
}

void launchServer()
{
  server.begin();
  Serial.println("Loaded Server!");
  server.on("/", [] () {
    server.send(200, (std::string("text").append("/").append("html")).c_str(), INDEX_HTML);
  });
  server.begin();
  socket.begin();
}

void setup() 
{
  Serial.begin(9600);
  setupSensor();
  currentAddress = connectWiFi(userSSID, userPASSWORD);
  launchServer();

}

void loop()
{
  server.handleClient();
  socket.loop();

  unsigned long now = millis();
  if(now - previousMillis > INTERVAL)
  {
    int sensorValue = static_cast<int>(readSensor());
    if(sensorValue < 100) socket.broadcastTXT(std::to_string(sensorValue).c_str());
    previousMillis = now;
  }
}

