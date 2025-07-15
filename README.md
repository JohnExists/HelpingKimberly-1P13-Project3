![1p13_Project_3Helping_Kimberly](https://github.com/user-attachments/assets/6c8d75ba-75a1-473c-81db-4769a56fba0d)

## Introduction
<img src="https://github.com/user-attachments/assets/820154f3-181b-4242-8179-805ade935658" alt="Kimberely" width="300"/>


This is Kimberly! She is a joyful, independent, and resilient person. Unfortunately, Kimberly lives with the following disabilities:
* 90% hearing loss, which constitutes profound deafness.
* Retinitis pigmentosa affects her peripheral vision, diminishes her capability to see colours and in dim light.

Currently, Kimberly uses Signia Hearing Aids, a white cane, and many apps on her phone for support in her daily life.
Despite these assistive technologies, Kimberly still faces many challenges to living independently within her daily environment.

In this project, me and my 3 other group members will assist with one challenge she faces, Microwave usage. 

To use her microwave, Kimberely's needs to use her iPhone camera to zoom into each button too see what she is pressing. This process is time consuming and inconvenient for her.

## Our Solution
To allow Kimberely to easily use her microwave, we designed a compact system that plugs into a wall outlet and where the sensors are placed under the buttons of a microwave. Here are the features:
* Takes input data from Kimberely's finger using three ultrasonic sensors about which button she is hovering over.
* Sends real-time audio feedback to Kimberly via her iPhone, compatible with any Bluetooth-enabled hearing aids.
  * To achieve this Kimberely's connects to a website hosted locally on her network   
* Achieved an accuracy of 90-95% with a response time of less than 2500 ms.
* Considered 3 quantitative objectives: Non-Intrusive, Easy Manufacturing, Environmentally Friendly
* Considered 2 qualitative objectives: Easy-to-Use and Comfortable
* Stayed under the budget constraint of $35


![03ded4d2-a017-41ba-85c5-f5ff64413738](https://github.com/user-attachments/assets/0d9fe456-917c-4269-bdec-0e39acf6a753)


## Approach
Here is how we achieved this solution.
* The main microntroller is an ESP32 used for performing the following operations:
  * Hosts a WebSocketServer on the local network so Kimberely can connect to the website using her iPhone
  * The microcontroller reads and interprets the data from the three ultrasonice sensors (HC-SR04, one for each column) to understand which button Kimberely is hovering over
    * First determines which column the finger is hovering over; then it determines the row based on the distance of the finger from the sensor.
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

#### Circuit Diagrams

![circuit (4)](https://github.com/user-attachments/assets/8b4ab522-4606-4db7-8aaf-e876f33a464a)


## Challenges
#### The ESP32 and Ultrasonic Sensor Have Different Operating Voltages.
 * The solution is split into two parts:
   * providing a high enough voltage for optimal sensor functionality
     * Using a power distribution board to provide a stable 5V to the sensors
   * Limiting the voltage received from the sensors to avoid damaging the ESP32 pins
     * Ultrasonic sensor output 5V through the echo pin and the ESP32 pins can only accept 3.3V
     * Limited the voltage received from the output signal using a voltage divider
<img width="354" height="189" alt="image" src="https://github.com/user-attachments/assets/7670ca60-d829-45ab-8225-e16df5134618" />

#### Interpreting The Data From The Sensor Accurately.
   * Since the HC-SR04 is a cheap sensor, it's output is very noisy.
     * The three sensors are sampled 35 times with a 30 ms delay between each sample, afterwards the most frequent value is sent to the client and it loops through this process again
   * The sensors detect the users finger even if it's in another column, the solution was checking multiple columns
     * If the left and middle sensors detect an object, that means it's hovering over the left column
     * If the left, middle and right sensors detect an object, that means it's hovering over the middle column
     * If the middle and right sensors detect an object, that means it's hovering over the right column
  

## Previous Prototypes
<img width="1980" height="1102" alt="image (1)" src="https://github.com/user-attachments/assets/c5b5cf6a-3671-4be4-8bd8-2484a0aed27e" />
<img width="1986" height="988" alt="image (2)" src="https://github.com/user-attachments/assets/f8de0949-9fba-4590-b51e-9ff02ca4a5dc" />

## Testing Procedure
<img width="1992" height="950" alt="image (4)" src="https://github.com/user-attachments/assets/4fb5d266-f382-4f63-9e07-b694d5488542" />
<img width="1980" height="970" alt="image (3)" src="https://github.com/user-attachments/assets/e2db945e-a551-4e60-9ed2-909db8eda44d" />


## Note Before Flashing Code To ESP32
#### To ensure that the code compiles create a new "password.h" file in the "src" folder with the following code (replace the placeholders in the string text)
```C++
const char* userSSID = "YOUR NETWORK NAME HERE";
const char* userPASSWORD = "YOUR NETWORK PASSWORD HERE";
```
