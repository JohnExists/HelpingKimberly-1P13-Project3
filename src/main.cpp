#include <Arduino.h>
#include<WiFi.h>
#include<WebServer.h>
#include<WebSocketsServer.h>

#include"website.h"
#include"password.h"


#define TEMP_READ 32

String http;
bool ledOn = false;

IPAddress currentAddress;
WebServer server(80);
WebSocketsServer socket = WebSocketsServer(81);

WiFiClient client;

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
    server.send(200, (std::string("text").append("/").append("html")).c_str(), 
      (std::string(INDEX_HTML_OPEN) + std::string(INDEX_HTML_CLOSE)).c_str());
  });
  server.begin();
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
}

