
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
  * The value is received by the client and is output in real-time using the html text-to-speech API (SpeechSynthesis)
* To distribute power to the ESP32 and the ultrasonic sensors, an STM32 Nucleo microcontroller was used


Why was a website used?
 * Compatibility accross all devices
 * Doesn't require an external downloads making it the most convenient to use
 * Large support through external APIs and libraries

Why was an ESP32 used?
 * An ESP32 was used since it supported high-speed networking capabilities with the internet, is low cost and has enough computing power for our applications.
 * ESP32s have a very quick prototyping process which is required for our limited timeline
 * This device was also used since it had the largest community behind it and a lot of support through external libraries.

Why was a WebSocketServer used?
 * The WebSockets protocol was the preferred choice since it allowed for real-time, reliable and frequent data communication between the client and the server which was ideal for our applications.
 * This protocol was also preferred since it was common in industry and had the most support behind it.

Decision Matrix Used For This Product:
<br>
<img width="528" height="233" alt="image" src="https://github.com/user-attachments/assets/35a062e3-0772-4b6c-80d4-e178a69834ac" />

## Challenges
#### The ESP32 operates at 3.3V and the Ultrasonic Sensors operates at 5.0V.
 * The solution is split into two parts:
   * providing a high enough voltage for optimal sensor functionality
     * Using a power distribution board to provide a stable 5V to the sensors
   * Limiting the voltage received from the sensors to avoid damaging the ESP32 pins
     * Ultrasonic sensor output 5V through the echo pin and the ESP32 pins can only accept 3.3V
     * Limited the voltage received from the output signal using a voltage divider
<img width="531" height="284" alt="image" src="https://github.com/user-attachments/assets/7670ca60-d829-45ab-8225-e16df5134618" />

## Images 

## Circuit Diagrams

![circuit (4)](https://github.com/user-attachments/assets/8b4ab522-4606-4db7-8aaf-e876f33a464a)

## Note Before Flashing Code To ESP32
#### To ensure that the code compiles create a new "password.h" file in the "src" folder with the following code (replace the placeholders in the string text)
```C++
const char* userSSID = "YOUR NETWORK NAME HERE";
const char* userPASSWORD = "YOUR NETWORK PASSWORD HERE";
```
