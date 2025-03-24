#include <Arduino.h>
#include<WiFi.h>
#include<WebServer.h>
#include<WebSocketsServer.h>

#include"website.h"
#include"password.h"

// Initializing constants
#define ROW_DOES_NOT_EXIST -1

#define SAMPLE_RIGHT  0
#define SAMPLE_MIDDLE 1
#define SAMPLE_LEFT 2

#define TRIG_PIN_RIGHT 18
#define TRIG_PIN_MID 5
#define TRIG_PIN_LEFT 17

#define ECHO_PIN_RIGHT 19
#define ECHO_PIN_MID 21
#define ECHO_PIN_LEFT 23

IPAddress currentAddress;
WebServer server(80);
WebSocketsServer socket = WebSocketsServer(81);

const int INTERVAL = 10;

unsigned long previousMillis = 0;
int samplePeriod = SAMPLE_RIGHT;
float sensorValueRight = 0, sensorValueMid = 0, sensorValueLeft = 0; 

void setupSensor()
{
    // Sets up the pins for outputing to TRIG on sensor
    // will be used to turn on data gathering for sensor
    pinMode(TRIG_PIN_RIGHT, OUTPUT);
    pinMode(TRIG_PIN_MID, OUTPUT);
    pinMode(TRIG_PIN_LEFT, OUTPUT);
    // Sets up the pins for inputting to ECHO on sensor
    // Binary data will be received in form of 3.3V
    pinMode(ECHO_PIN_RIGHT, INPUT);
    pinMode(ECHO_PIN_MID, INPUT);
    pinMode(ECHO_PIN_LEFT, INPUT);
}

float readSensor(int trig, int echo)
{
    // Initializing the sensor
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    // Used to trigger the sensor for data gathering
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
  
    // Calculations for converting sensor data to centimeters
    float duration = pulseIn(echo, HIGH);
    return duration / 58.2;
}

IPAddress connectWiFi(const char* SSID, const char* PASSWORD)
{
  // Outputs important data to the user through serial port
  Serial.println("I am not connected so I am sad :(");
  Serial.print("Connecting To ");
  Serial.print(SSID);
  // Turns on STATION MODE for ESP32 to be used as a web server
  WiFi.mode(WIFI_STA);
  // Starts connecting to WiFi using predefined SSID and PASSWORD (in password.h)
  WiFi.begin(SSID, PASSWORD);
  Serial.println("\nConnecting");

  // Continuously tries to connect until it is successful
  // No timeout was set
  while(WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(100);
  }

  // Once the user connects to the wifi
  Serial.println("\nI am connected so I am happy :)");
  Serial.println("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  return WiFi.localIP();
}

void launchServer()
{
  // Initializes all aspects of the web server
  server.begin();
  Serial.println("Loaded Server!");

  // Loading the HTML page for the web server so that
  // it can be viewed by the user (html page is in website.h)
  server.on("/", [] () {
    server.send(200, (std::string("text").append("/").append("html")).c_str(), INDEX_HTML);
  });

  // Starts running both the socket and web server
  server.begin();
  socket.begin();
}

void setup() 
{
  // Sets up serial port for debugging purposes
  Serial.begin(9600);
  // Launchs the sensor, connects to wifi and launches server
  setupSensor();
  // currentAddress = connectWiFi(userSSID, userPASSWORD);
  // launchServer();
}


void serialPrintClean(std::string prefix, float value)
{
  Serial.println(std::string(prefix).append(std::to_string(value)).c_str());
}


bool objectDetected(float sensorValue) { return sensorValue <= 30.0; }
int getRow(float sensorValue) 
{
  const float R3 = 6.5;
  const float R2 = 8.0;
  const float R1 = 10.0;
  const float RANGE = 1.15;

  if(R3 - RANGE < sensorValue && sensorValue < R3 + RANGE) return 3;
  if(R2 - RANGE < sensorValue && sensorValue < R2 + RANGE) return 2;
  if(R1 - RANGE < sensorValue && sensorValue < R1 + RANGE) return 1;
  return ROW_DOES_NOT_EXIST;
}

void getButtonHovered()
{
  // Values for which sensors detect a finger
  bool rightDet = objectDetected(sensorValueRight),
    midDet = objectDetected(sensorValueMid),
    leftDet = objectDetected(sensorValueLeft);
    
  std::string txt = "";
  // Hovering over middle buttons
  if((rightDet && midDet && leftDet) || (!rightDet && midDet && !leftDet))
  {
    int row = getRow(sensorValueMid);
    if(row == 1) txt = "2";
    if(row == 2) txt = "5";
    if(row == 3) txt = "8";
  // Hovering over right buttons
  } else if ((rightDet && midDet) || (rightDet && !midDet && !leftDet)) 
  {
    int row = getRow(sensorValueRight);
    if(row == 1) txt = "3";
    if(row == 2) txt = "6";
    if(row == 3) txt = "9";
    // Hovering over left buttons
  } else if ((leftDet && midDet) || (!rightDet && !midDet && leftDet))
  {
    int row = getRow(sensorValueLeft);
    if(row == 1) txt = "1";
    if(row == 2) txt = "4";
    if(row == 3) txt = "7";
    
  // No hovering is detected / Invalid hovering
  } else {
    Serial.println("NONE");
  }

  Serial.println(txt.c_str());

}


void loop()
{
  // Runs the update cycle for the server and web socket
  // server.handleClient();
  // socket.loop();

  // If a certain time has passed (500 ms) it will
  // sample sensor data and broadcast it to the user
  unsigned long now = millis();
  if(now - previousMillis > INTERVAL)
  {
    switch (samplePeriod)
    {
    case SAMPLE_RIGHT:
      sensorValueRight = readSensor(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
      #ifdef DEBUG
      if(objectDetected(sensorValueRight)) serialPrintClean("RIGHT: ", sensorValueRight);
      #endif
      samplePeriod = SAMPLE_MIDDLE;
      break;

    case SAMPLE_MIDDLE:
      sensorValueMid = readSensor(TRIG_PIN_MID, ECHO_PIN_MID);
      // #ifdef DEBUG
      if(objectDetected(sensorValueMid)) serialPrintClean("MIDDLE: ", sensorValueMid);
      // #endif
      samplePeriod = SAMPLE_LEFT;
      break;

    case SAMPLE_LEFT:
      sensorValueLeft = readSensor(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
      #ifdef DEBUG
      if(objectDetected(sensorValueLeft)) serialPrintClean("LEFT: ", sensorValueLeft);
      #endif
      samplePeriod = SAMPLE_RIGHT;

      // Done Sampling Sequence (Prints, Then Interprets Current Data)
      getButtonHovered();
      Serial.println("---------------------DONE SAMPLING---------------------");
      break;
    }

    // if(sensorValue < 100) socket.broadcastTXT(std::to_string(sensorValue).c_str());
    previousMillis = now;
  }
}

