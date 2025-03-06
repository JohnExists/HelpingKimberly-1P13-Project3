#include <Arduino.h>
#include<WiFi.h>
#include<WebServer.h>

#include"website.h"
#include"password.h"

#define BLUE_LED 23

String http;
bool ledOn = false;

IPAddress currentAddress;
WiFiServer server(80);
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
}

void setup() 
{
  pinMode(BLUE_LED, OUTPUT);

  Serial.begin(9600);
  currentAddress = connectWiFi(userSSID, userPASSWORD);
  launchServer();
}

void sendResponse()
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
}

void updateLED()
{

}

void updateWebpage()
{
  client.println(INDEX_HTML_OPEN);
  client.println(INDEX_HTML_CLOSE);
}

void loop() 
{
  client = server.available();
  if(client)
  {
    Serial.println("New client logging in...");
    String clientData;
    while(client.connected())
    {
      if(client.available())
      {
        char read = client.read();
        http += read;
        Serial.print(read);
        if(read == '\n')
        {
          if(clientData.length() == 0)
          {
            sendResponse();
            updateLED();
            updateWebpage();
          } else {
            clientData = "";
          }
        } else if(read != '\r') {
          clientData += read;
        }
      }
    }
    http = "";
    client.stop();
    Serial.println("Client Disconnected!");
    Serial.println();
  }
  
}
