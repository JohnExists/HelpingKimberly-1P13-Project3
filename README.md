
![1p13_Project_3Helping_Kimberly](https://github.com/user-attachments/assets/6c8d75ba-75a1-473c-81db-4769a56fba0d)

## Introduction
<img src="https://github.com/user-attachments/assets/820154f3-181b-4242-8179-805ade935658" alt="Kimberely" width="300"/>


This is Kimberly! She is a joyful, independent, and resilient person. Unfortunately, Kimberly lives with the following disabilities:
* 90% hearing loss, which constitutes profound deafness.
* Retinitis pigmentosa affects her peripheral vision, diminishes her capability to see colours and in dim light.

Currently, Kimberly uses Signia Hearing Aids, a white cane, and many apps on her phone for support in her daily life.
Despite these assistive technologies, Kimberly still faces many challenges to living independently within her daily environment.

In this project, me and my 3 other group members will assist with one challenge she faces, Microwave usage. 

To use her microwave, Kimberely's needs to use her iPhone camera to zoom into each button. This process is time consuming and inconvenient for her.

## Our Solution
To allow Kimberely to easily use her microwave, we designed a compact system that plugs into a wall outlet and where the sensors are placed under the buttons of a microwave. Here are the features:
* Takes input data from Kimberely's finger using three ultrasonic sensors about which button she is hovering over.
* Sends real-time audio feedback to Kimberly via her iPhone, compatible with any Bluetooth-enabled hearing aids.
  * To achieve this Kimberely's connects to a website hosted locally on her network   
* Achieved an accuracy of 90-95% with a response time of less than 1000 ms.
* Considered 3 quantitative objectives: Non-Intrusive, Easy Manufacturing, Environmentally Friendly
* Considered 2 qualitative objectives: Easy-to-Use and Comfortable
* Stayed under the budget constraint of $35

## Approach
Here is how we achieved this solution.
* The main microntroller is an ESP32 used for performing the following operations:
  * Hosts a WebSocketServer on the local network so Kimberely can connect to the website using her iPhone
  * The microcontroller reads and interprets the ultrasonice sensor data to understand which button Kimberely is hovering over
  * The button that Kimberely is hovering over is stored as an integer and is broadcasted to the client using WebSockets
  * The value is received by the client and is output in real-time using the html text-to-speech API
 

Why was an ESP32 used?
 * An ESP32 was used since it supported high-speed networking capabilities with the internet, is low cost and has enough computing power for our applications.
 * ESP32s have a very quick prototyping process which is required for our limited timeline
 * This device was also used since it had the largest community behind it and a lot of support through external libraries.

Why was a WebSocketServer used?
 * The WebSockets protocol was the preferred choice since it allowed for real-time, reliable and frequent data communication between the client and the server which was ideal for our applications.
 * This protocol was also preferred since it was common in industry and had the most support behind it.

## Challenges
Since the ESP32 operates at 3.3V and the Ultrasonic Sensors operated at 5.0V this posed a challenge that needed to be overcome.

## Images 

## Circuit Diagrams

![circuit (4)](https://github.com/user-attachments/assets/8b4ab522-4606-4db7-8aaf-e876f33a464a)

## Note Before Flashing Code To ESP32
#### To ensure that the code compiles create a new "password.h" file in the "src" folder with the following code (replace the placeholders in the string text)
```C++
const char* userSSID = "YOUR NETWORK NAME HERE";
const char* userPASSWORD = "YOUR NETWORK PASSWORD HERE";
```
