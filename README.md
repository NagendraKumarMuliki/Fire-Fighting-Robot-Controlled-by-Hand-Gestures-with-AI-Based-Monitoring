# Fire-Fighting-Robot-Controlled-by-Hand-Gestures-with-AI-Based-Monitoring

This project presents a complete AI‑powered firefighting robot capable of detecting fire, identifying humans, streaming live video, and responding to hand gestures in real time. The system integrates computer vision, embedded systems, IoT communication, gesture recognition, and robotics control using a laptop, Raspberry Pi, Arduino UNO, YOLOv8, MediaPipe, and ultrasonic sensing. The robot is designed for safe, intuitive, and semi‑autonomous fire‑response operations.

## Project Overview

The robot operates using a three‑layer architecture involving a laptop, Raspberry Pi, and Arduino. The laptop performs gesture recognition using its webcam and runs YOLOv8 on the live video stream coming from the Raspberry Pi camera. The Raspberry Pi streams video to the laptop, receives gesture commands from the laptop through a shared hotspot connection, and forwards these commands to the Arduino via USB. The Arduino controls the motors, pump, servo motors, and ultrasonic sensor for staircase protection. The system supports both automatic and gesture‑based control of movement and pump activation.

## Key Features

The robot supports full gesture‑based control using the laptop webcam. The user can control Forward, Backward, Left, Right, Stop, Pump ON, and Pump OFF using hand gestures. YOLOv8 detects fire and humans in the live video stream from the Raspberry Pi camera. When fire is detected, the pump automatically activates for two seconds for safety and then turns off. The pump can also be manually controlled through gestures. The servo motor automatically tracks the flame direction based on YOLO detection. The downward‑facing ultrasonic sensor provides staircase protection by preventing the robot from falling off edges.

## How the System Works

The laptop and Raspberry Pi are connected to the same mobile hotspot. The laptop uses its webcam to recognize hand gestures through MediaPipe. It also receives the live video stream from the Raspberry Pi camera and runs YOLOv8 to detect fire and humans. Based on gesture recognition or YOLO detection, the laptop sends commands to the Raspberry Pi using MQTT. The Raspberry Pi receives these commands and forwards them to the Arduino through a USB cable. The Arduino executes the commands by driving the motors, activating the pump, rotating the servo motor, and monitoring the ultrasonic sensor for staircase protection. The Raspberry Pi camera continuously streams live video to the laptop for monitoring and YOLO inference.

## Hardware Components

The system uses a laptop for gesture recognition and YOLO inference, a Raspberry Pi 4 for video streaming and command relay, and an Arduino UNO for motor control, pump activation, servo flame tracking, and ultrasonic staircase protection. Additional components include a motor driver, servo motor, water pump, ultrasonic sensor, Pi camera, and battery pack.

## Software Stack

The laptop runs Python, YOLOv8, MediaPipe, OpenCV, and MQTT. The Raspberry Pi runs Python for camera streaming, MQTT communication, and USB serial communication. The Arduino uses Arduino C++ with servo and ultrasonic libraries.

## Project Structure

The repository contains Python scripts for gesture recognition, YOLO detection, MQTT communication, and video streaming. The Arduino folder contains the robot control code. The models folder contains the YOLOv8 model file. The README provides documentation for setup and usage.

## Installation and Setup

Clone the repository. Install all Python dependencies on the laptop. Upload the Arduino code to the Arduino UNO. Connect the laptop and Raspberry Pi to the same hotspot. Connect the Raspberry Pi to the Arduino using a USB cable. Start the gesture recognition and YOLO detection scripts on the laptop. Start the camera streaming script on the Raspberry Pi.

## Usage Instructions

Use hand gestures to control the robot. The robot supports Forward, Backward, Left, Right, Stop, Pump ON, and Pump OFF. When YOLO detects fire, the pump automatically activates for two seconds and then turns off for safety. The servo motor tracks the flame direction. The ultrasonic sensor prevents the robot from falling off stairs or edges. The Raspberry Pi camera streams live video to the laptop for monitoring and YOLO inference.

## Skills Demonstrated

This project demonstrates AI and machine learning model integration, gesture recognition, embedded systems programming, IoT communication using MQTT, real‑time video processing, robotics system design, hardware‑software integration, and safety automation.

## Future Improvements

Potential enhancements include autonomous navigation, thermal camera integration, SLAM mapping, mobile app control, and improved pump pressure.

## Acknowledgements

Special thanks to Dr. Don Wilcher for continuous guidance and mentorship throughout this project.


