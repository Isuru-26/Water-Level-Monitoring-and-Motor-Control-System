# 💧 Smart Water Level Monitoring System  
🌐 ESP32-Based Ultrasonic Water Level Monitoring & Automatic Motor Control  

---

## 🎯 Project Overview

The **Smart Water Level Monitoring System** is an ESP32-based IoT solution that continuously monitors the water level of a tank using an **ultrasonic sensor** and automatically controls a **water pump (motor)** to maintain safe water levels.

The system provides **real-time visual feedback** through an **OLED display**, **LED indicators**, and **buzzer alerts**, while also offering a **WiFi-enabled web dashboard** for remote monitoring. This project is ideal for **home water tanks, apartments, irrigation systems, and industrial water management**.

---

## ✨ Key Features & Benefits

### 📏 Real-Time Water Level Measurement  
Accurately measures water level using an ultrasonic sensor mounted above the tank.

### 🖥️ OLED Display Visualization  
Displays water percentage, water height (cm), and tank fill status on a 128×64 OLED screen.

### 🌐 Web-Based Dashboard  
Live monitoring through a browser with animated tank visualization, percentage display, and status indicators.

### ⚙️ Automatic Motor Control  
- **≤ 20% → Motor ON (Tank Low)**  
- **100% → Motor OFF (Tank Full)**  
Prevents overflow and dry running.

### 🚨 Alert & Indicator System  
- Buzzer alert when water level is critically low  
- LED indicators for Low / Medium / High water levels

### ⚡ ESP32 Powered IoT System  
Fast processing, built-in WiFi, and expandable for cloud integration.

---

## ⚙️ How It Works

### 1️⃣ Water Level Sensing
- Ultrasonic sensor sends a trigger pulse  
- Echo time is measured and converted to distance (cm)

### 2️⃣ Data Processing
- ESP32 calculates actual water height based on tank dimensions  
- Converts height into percentage value

### 3️⃣ Motor Automation
- Motor turns **ON** when water level drops below 20%  
- Motor turns **OFF** automatically when tank becomes full

### 4️⃣ Real-Time Monitoring
- Live data displayed on OLED  
- Sensor data served via ESP32 web server  
- Web dashboard updates every 2 seconds

---

## 🛠️ Technology Stack

| Category | Technologies |
|--------|-------------|
| Microcontroller | ESP32 |
| Sensor | Ultrasonic Sensor (HC-SR04) |
| Display | OLED 128×64 (SSD1306) |
| Output Devices | Relay, Motor, Buzzer, LEDs |
| Connectivity | WiFi |
| Firmware | Arduino (C/C++) |
| Libraries | Adafruit_GFX, Adafruit_SSD1306, ArduinoJson |

---

## 🚀 Getting Started

### 🔑 Prerequisites
- Arduino IDE  
- ESP32 Board Package installed  
- USB cable  
- Required libraries:
  - Adafruit GFX Library  
  - Adafruit SSD1306  
  - ArduinoJson  

---

## 🔌 Hardware Connections

| Component | ESP32 Pin |
|---------|-----------|
| Ultrasonic TRIG | GPIO 5 |
| Ultrasonic ECHO | GPIO 18 |
| Buzzer | GPIO 23 |
| Relay (Motor) | GPIO 12 |
| LED (Low) | GPIO 25 |
| LED (Medium) | GPIO 26 |
| LED (High) | GPIO 27 |
| OLED SDA | Default ESP32 SDA |
| OLED SCL | Default ESP32 SCL |

---

## ▶️ Upload & Run

1. Select the correct **ESP32 board** and **COM port**  
2. Upload the code to the ESP32  
3. Open **Serial Monitor** (Baud rate: **115200**)  
4. Connect to the displayed IP address using a web browser  
5. Monitor live water level and motor status

---

## 💡 Use Cases

- 🏠 Home Water Tank Automation  
- 🚰 Automatic Pump Control Systems  
- 🌾 Irrigation & Agriculture Water Monitoring  
- 🏭 Industrial Tank Level Monitoring  
- 🧪 IoT & Embedded Systems Projects  

---

## 🧪 System Behavior Summary

| Water Level | Motor | Buzzer | LEDs |
|------------|-------|--------|------|
| ≤ 20% | ON | ON | 🔴 Low |
| 30–70% | Last State | OFF | 🟡 Medium |
| ≥ 70% | OFF | OFF | 🟢 High |
| 100% | OFF | OFF | 🟢 Full |

---

## 🔧 Configuration

Modify tank dimensions in the code as needed:

```cpp
#define TANK_HEIGHT 10     // Tank water depth (cm)
#define SENSOR_HEIGHT 13   // Sensor height from bottom (cm)
