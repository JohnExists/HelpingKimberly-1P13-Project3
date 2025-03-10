#include <Arduino.h>
#include<WiFi.h>
#include<WebServer.h>
#include<WebSocketsServer.h>

#include"website.h"
#include"password.h"


#define TEMP_READ 32

IPAddress currentAddress;
WebServer server(80);
WebSocketsServer socket = WebSocketsServer(81);

const int INTERVAL = 2000;
unsigned long previousMillis = 0;

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
  pinMode(TEMP_READ, INPUT);

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
    socket.broadcastTXT(std::to_string(analogRead(32)).c_str());
    previousMillis = now;
  }
}

