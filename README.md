# Guardian Vest

<img src="images/full_view_image.jpg" alt="Wheelchair Overview" width="300"/>

## Project Overview
This project implements a safety vest using Arduino UNO R4. 
The vest is a multifunctional system designed to protect socially vulnerable pedestrians and reduce the risk of accidents.  
It not only detects potential hazards but also responds to them, providing assistance to both pedestrians and drivers.  
The vest can detect approaching vehicles using ultrasonic and radar sensors, and detect falls using an IMU sensor.  
These detections trigger responses such as LEDs, buzzers, vibration motors, airbags, and alert notifications via Blynk.  
Additionally, the vest can be produced at low cost, making it accessible to a wide range of pedestrians.

## Hardware
- Arduino UNO R4 Wifi

Detection - Vehicles
- Ultrasonic sensor (Distance measurement)
- Radar sensor (Speed measurement)

Detection - Fall
- IMU sensor (Fall detection)

Response - Vehicles
- Vibration motors (Tactile response)
- Buzzer (Auditory response)
- LEDs (Visual response)

Response - Fall
- Airbag (Neck protection)
- Alert notification (via Blynk)
  
## Software
- Arduino IDE

## System Architecture
**Arduino:**
- Detect approaching vehicles using ultrasonic and radar sensors
- Respond with vibration motors, buzzer, and LEDs
- Detect falls using the IMU sensor
- Respond with airbag deployment and alert notification via Blynk


## Features
- Real-time vehicle detection and warning for pedestrians
- Fall detection and automatic airbag deployment
- Multi-modal feedback: tactile, auditory, and visual
- Low-cost and easily wearable design
- Smartphone alert notifications via Blynk

## How to Run
1. Connect all sensors and actuators to Arduino UNO R4 Wifi according to the hardware diagram.  
2. Upload the `vest1.ino` sketch to the Arduino using Arduino IDE.  
3. Power on the vest and ensure all sensors initialize correctly.  
4. Monitor the vest’s response to obstacles (vehicles) and simulated falls.  
5. Adjust thresholds for sensors as needed for different environments.  
