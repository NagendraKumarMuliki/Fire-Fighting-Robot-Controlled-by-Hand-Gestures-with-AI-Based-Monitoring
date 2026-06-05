# 🔥🤖 Fire-Fighting Robot Controlled by Hand Gestures with AI-Based Monitoring  
![Project Status](https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge)
![AI Vision](https://img.shields.io/badge/AI-Computer_Vision-blueviolet?style=for-the-badge&logo=ai)
![Robotics](https://img.shields.io/badge/Robotics-Autonomous-orange?style=for-the-badge&logo=robot)
![IoT](https://img.shields.io/badge/IoT-MQTT_Communication-00bfa5?style=for-the-badge&logo=iot)

This project presents a complete **AI‑powered firefighting robot** capable of detecting fire, identifying humans, streaming live video, and responding to **hand gestures** in real time.  
It integrates **computer vision, embedded systems, IoT communication, gesture recognition, and robotics control** using:

🖥️ Laptop  
🍓 Raspberry Pi  
🟦 Arduino UNO  
🧠 YOLOv8  
✋ MediaPipe  
📡 MQTT  
📷 Pi Camera  
📏 Ultrasonic Sensor  

The robot is designed for **safe, intuitive, and semi‑autonomous fire‑response operations**.

---

## 🚀 Project Overview  
![Overview](https://img.shields.io/badge/Architecture-3_Layers-blue?style=for-the-badge&logo=layers)

The robot operates using a **three‑layer architecture**:

### 🖥️ Laptop Layer  
- Performs **gesture recognition** using MediaPipe  
- Runs **YOLOv8** on live video stream from Raspberry Pi  
- Sends commands to Raspberry Pi using **MQTT**  

### 🍓 Raspberry Pi Layer  
- Streams video to laptop  
- Receives gesture/YOLO commands  
- Forwards commands to Arduino via USB  

### 🟦 Arduino Layer  
- Controls motors  
- Activates pump  
- Tracks flame using servo  
- Uses ultrasonic sensor for staircase protection  

The system supports **automatic fire response** and **gesture‑based manual control**.

---

## 🔥 Key Features  
![Features](https://img.shields.io/badge/Features-Advanced-orange?style=for-the-badge&logo=star)

### ✋ Gesture-Based Robot Control  
Control the robot using hand gestures:

➡️ Forward  
⬅️ Left  
➡️ Right  
⬇️ Backward  
⏹️ Stop  
💧 Pump ON  
💧 Pump OFF  

### 🔥 YOLOv8 Fire & Human Detection  
- Detects fire in real time  
- Detects humans in the environment  
- Automatically activates pump for **2 seconds** when fire is detected  
- Servo motor **auto-tracks flame direction**  

### 🛡️ Safety Automation  
- Ultrasonic sensor prevents falling from stairs  
- Pump auto-shutoff for safety  
- Gesture override for manual control  

---

## ⚙️ How the System Works  
![System](https://img.shields.io/badge/System_Workflow-Explained-blueviolet?style=for-the-badge&logo=workflow)

1. Laptop and Raspberry Pi connect to the same mobile hotspot  
2. Laptop webcam performs **gesture recognition**  
3. Raspberry Pi camera streams video to laptop  
4. Laptop runs YOLOv8 to detect fire/humans  
5. Laptop sends commands to Raspberry Pi via **MQTT**  
6. Raspberry Pi forwards commands to Arduino via USB  
7. Arduino executes actions:  
   - Motor control  
   - Pump activation  
   - Servo flame tracking  
   - Ultrasonic staircase protection  

---

## 🧩 Hardware Components  
![Hardware](https://img.shields.io/badge/Hardware-Robot_Core-green?style=for-the-badge&logo=hardware)

- 💻 Laptop  
- 🍓 Raspberry Pi 4  
- 🟦 Arduino UNO  
- 🔋 Motor Driver  
- 🔧 Servo Motor  
- 💧 Water Pump  
- 📏 Ultrasonic Sensor  
- 📷 Raspberry Pi Camera  
- 🔌 Battery Pack  

---

## 🖥️ Software Stack  
![Software](https://img.shields.io/badge/Software-Python%20%7C%20YOLOv8%20%7C%20MQTT-blue?style=for-the-badge&logo=python)

### Laptop  
- Python  
- YOLOv8  
- MediaPipe  
- OpenCV  
- MQTT  

### Raspberry Pi  
- Python  
- Camera streaming  
- MQTT communication  
- USB serial communication  

### Arduino  
- Arduino C++  
- Servo + Ultrasonic libraries  

---

## 📁 Project Structure  
![Structure](https://img.shields.io/badge/Structure-Organized-lightgrey?style=for-the-badge&logo=files)

- Python scripts for gesture recognition, YOLO detection, MQTT, and video streaming  
- Arduino folder with robot control code  
- Models folder with YOLOv8 model  
- README with full documentation  

---

## 🛠️ Installation & Setup  
![Setup](https://img.shields.io/badge/Setup-Simple-success?style=for-the-badge&logo=install)

1. Clone the repository  
2. Install Python dependencies  
3. Upload Arduino code  
4. Connect laptop + Raspberry Pi to same hotspot  
5. Connect Raspberry Pi to Arduino via USB  
6. Start gesture + YOLO scripts on laptop  
7. Start camera streaming on Raspberry Pi  

---

## 🎮 Usage Instructions  
![Usage](https://img.shields.io/badge/Usage-Gesture_Control-blue?style=for-the-badge&logo=joystick)

- Use gestures to control movement  
- Pump auto-activates for 2 seconds when fire is detected  
- Servo tracks flame direction  
- Ultrasonic sensor prevents falling  
- Pi camera streams live video to laptop  

---

## 🧠 Skills Demonstrated  
![Skills](https://img.shields.io/badge/Skills-AI%20%7C%20Robotics%20%7C%20IoT-purple?style=for-the-badge&logo=brain)

- AI + ML model integration  
- Gesture recognition  
- Embedded systems programming  
- IoT communication (MQTT)  
- Real-time video processing  
- Robotics system design  
- Hardware-software integration  
- Safety automation  

---

## 🚀 Future Improvements  
![Future](https://img.shields.io/badge/Future-Autonomous_Navigation-yellow?style=for-the-badge&logo=rocket)

- Autonomous navigation  
- Thermal camera integration  
- SLAM mapping  
- Mobile app control  
- Higher pump pressure  

---

## 🙏 Acknowledgements  
![Thanks](https://img.shields.io/badge/Thanks-Dr.%20Don%20Wilcher-blue?style=for-the-badge&logo=heart)

Special thanks to **Dr. Don Wilcher** for continuous guidance and mentorship.

---

## ⚠️ Disclaimer  
![Disclaimer](https://img.shields.io/badge/Disclaimer-Important-red?style=for-the-badge&logo=alert)

This project is for **educational and experimental use only**.  
It is **not** a certified firefighting or rescue system.  
Use responsibly and **at your own risk**.

---

## 📄 MIT License  
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge&logo=open-source-initiative)

MIT License  

Copyright (c) 2026 **Chinna**

Permission is hereby granted, free of charge, to any person obtaining a copy  
of this software and associated documentation files (the “Software”), to deal  
in the Software without restriction, including without limitation the rights  
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell  
copies of the Software, and to permit persons to whom the Software is  
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in  
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER  
DEALINGS IN THE SOFTWARE.
