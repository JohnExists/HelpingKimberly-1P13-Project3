#include <Arduino.h>
#include<WiFi.h>
#include<WebServer.h>
#include<WebSocketsServer.h>
#include <unordered_map>

#include"website.h"
#include"password.h"

// Initializing constants
#define ROW_DOES_NOT_EXIST -1

#define RIGHT_SENSOR    0
#define MIDDLE_SENSOR   1
#define LEFT_SENSOR     2

#define SAMPLE_RIGHT    0
#define SAMPLE_MIDDLE   1
#define SAMPLE_LEFT     2

#define TRIG_PIN_RIGHT  18
#define TRIG_PIN_MID    5
#define TRIG_PIN_LEFT   17

#define ECHO_PIN_RIGHT  19
#define ECHO_PIN_MID    21
#define ECHO_PIN_LEFT   23

#define START 10
#define CANCEL 11

// #define CALIBRATION_DEBUG
#define DO_LAUNCH_SERVER
// #define DEBUG

IPAddress currentAddress;
WebServer server(80);
WebSocketsServer socket = WebSocketsServer(81);

#define NUMBER_OF_SAMPLES 35
#define SAMPLING_PERIOD 30

int currentSamplingPeriod = 0;
int sampledData[NUMBER_OF_SAMPLES];

unsigned long previousMillis = 0;
int samplePeriod = SAMPLE_RIGHT;
float sensorValueRight = 0, sensorValueMid = 0, sensorValueLeft = 0; 
float lSum = 0, mSum = 0, rSum = 0;

int previousData = -1;

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
  for(int i = 0; i < NUMBER_OF_SAMPLES; ++i) sampledData[i] = 0;
  // Sets up serial port for debugging purposes
  Serial.begin(9600);
  // Launchs the sensor, connects to wifi and launches server
  setupSensor();

  #ifdef DO_LAUNCH_SERVER
  currentAddress = connectWiFi(userSSID, userPASSWORD);
  launchServer();
  #endif
}


void serialPrintClean(std::string prefix, float value)
{
  Serial.println(std::string(prefix).append(std::to_string(value)).c_str());
}


bool objectDetected(float sensorValue) { return sensorValue <= 17.5; }

float normalizedReadSensor(int trig, int echo, int sensor)
{
  float readValue = readSensor(trig, echo);
  switch (sensor)
  {
    case MIDDLE_SENSOR: return readValue - 2;
    case LEFT_SENSOR: return readValue - 4.25;
  }
  return readValue;
}

int getRow(float sensorValue) 
{
  const float R4 = 4.8;
  const float R3 = 6.8;
  const float R2 = 8.8;
  const float R1 = 11.8;
  const float RANGE = 1.15;

  if(R4 - RANGE < sensorValue && sensorValue < R4 + RANGE) return 4;
  if(R3 - RANGE < sensorValue && sensorValue < R3 + RANGE) return 3;
  if(R2 - RANGE < sensorValue && sensorValue < R2 + RANGE) return 2;
  if(R1 - RANGE < sensorValue && sensorValue < R1 + RANGE) return 1;
  return ROW_DOES_NOT_EXIST;
}

int mostFrequent(int nums[], int size) {
  int result = -1;
  int maxCount = 0;

  // Loop through the array to find the most frequent number
  for (int i = 0; i < size; i++) {
      int count = 0;
      
      // Count occurrences of nums[i]
      for (int j = 0; j < size; j++) {
          if (nums[j] == nums[i]) {
              count++;
          }
      }

      // Update the most frequent number if the count of nums[i] is greater
      if (count > maxCount) {
          result = nums[i];
          maxCount = count;
      }
  }

  return result;
}


void getButtonHovered()
{
  // Values for which sensors detect a finger
  bool rightDet = objectDetected(sensorValueRight),
    midDet = objectDetected(sensorValueMid),
    leftDet = objectDetected(sensorValueLeft);
    
  int result = -1;
  // Hovering over middle buttons
  if((rightDet && midDet && leftDet) || (!rightDet && midDet && !leftDet))
  {
    int row = getRow(sensorValueMid);
    if(row == 1) result = 2;
    if(row == 2) result = 5;
    if(row == 3) result = 8;
    if(row == 4) result = 0;
  // Hovering over right buttons
  } else if ((!leftDet && midDet && rightDet) || (rightDet && !midDet && !leftDet)) 
  {
    int row = getRow(sensorValueRight);
    if(row == 1) result = 3;
    if(row == 2) result = 6;
    if(row == 3) result = 9;
    if(row == 4) result = START;
    // Hovering over left buttons
  } else if ((leftDet && midDet && !rightDet) || (!rightDet && !midDet && leftDet))
  {
    int row = getRow(sensorValueLeft);
    if(row == 1) result = 1;
    if(row == 2) result = 4;
    if(row == 3) result = 7;
    if(row == 4) result = CANCEL;
  } 

  sampledData[currentSamplingPeriod] = result;

}

void loop()
{
  // Runs the update cycle for the server and web socket
  #ifdef DO_LAUNCH_SERVER
  server.handleClient();
  socket.loop();
  #endif

  // If a certain time has passed (500 ms) it will
  // sample sensor data and broadcast it to the user
  unsigned long now = millis();

  // Called 3 times per sampling period
  // First samples right sensor, pauses, 
  // then samples middle, pauses
  // then samples left,
  // Then interprets the data from the 3 sensors
  // Then Repeats For # Of Samples
  // Using the data from each sampling period,
  // the most frequent data is displayed on the website
  if(now - previousMillis > (SAMPLING_PERIOD / 3))
  {
    switch (samplePeriod)
    {
    case SAMPLE_RIGHT:
      sensorValueRight = normalizedReadSensor(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT, RIGHT_SENSOR);
      rSum = sensorValueRight;
      #ifdef DEBUG
      if(objectDetected(sensorValueRight)) serialPrintClean("RIGHT: ", sensorValueRight);
      #endif
      samplePeriod = SAMPLE_MIDDLE;
      break;

    case SAMPLE_MIDDLE:
      sensorValueMid = normalizedReadSensor(TRIG_PIN_MID, ECHO_PIN_MID, MIDDLE_SENSOR);
      mSum = sensorValueMid;
      #ifdef DEBUG
      if(objectDetected(sensorValueMid)) serialPrintClean("MIDDLE: ", sensorValueMid);
      #endif
      samplePeriod = SAMPLE_LEFT;
      break;

    case SAMPLE_LEFT:
      sensorValueLeft = normalizedReadSensor(TRIG_PIN_LEFT, ECHO_PIN_LEFT, LEFT_SENSOR);
      lSum += sensorValueLeft;
      #ifdef DEBUG
      if(objectDetected(sensorValueLeft)) serialPrintClean("LEFT: ", sensorValueLeft);
      #endif
      samplePeriod = SAMPLE_RIGHT;

      // Done Sampling Sequence (Prints, Then Interprets Current Data)
      getButtonHovered();
      ++currentSamplingPeriod;
      if(currentSamplingPeriod == NUMBER_OF_SAMPLES) 
      {
        #ifdef CALIBRATION_DEBUG
        Serial.println(("Left: " + std::to_string(lSum / NUMBER_OF_SAMPLES)).c_str());
        Serial.println(("Middle: " + std::to_string(mSum / NUMBER_OF_SAMPLES)).c_str());
        Serial.println(("Right: " + std::to_string(rSum / NUMBER_OF_SAMPLES)).c_str());
        lSum = 0;
        mSum = 0;
        rSum = 0;
        #endif
        int data = mostFrequent(sampledData, NUMBER_OF_SAMPLES);
        if(data != -1 && data != previousData) socket.broadcastTXT(std::to_string(data).c_str());
        Serial.println(data);
        previousData = data;
        currentSamplingPeriod = 0;
      }
      break;
    }

    previousMillis = now;
  }
}

