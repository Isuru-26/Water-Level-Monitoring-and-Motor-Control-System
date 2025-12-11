# 💧 ESP32 Water Level Monitoring & Motor Control System

A smart IoT-based water tank monitoring solution built using **ESP32**, **HC-SR04 Ultrasonic Sensor**, **OLED Display (SSD1306)**, **Buzzer**, **LED indicators**, **Motor/Relay**, and a **real‑time Web Dashboard** for monitoring tank water levels and controlling motor operation automatically.

---

## 📌 Features

* Measures **real-time water level** using an HC-SR04 ultrasonic sensor
* Displays **percentage & water height** on the OLED screen
* Automatic **motor ON/OFF control** based on water level:

  * **≤ 20% → Motor ON**
  * **100% → Motor OFF**
* Beautiful **Web Dashboard** with:

  * Real-time % water level
  * Water height (cm)
  * Distance to water surface
  * Animated tank visualization
  * Status indication (Low / Medium / High)
* LED-based level indication:

  * 🔴 Low (≤ 30%)
  * 🟡 Medium (30–70%)
  * 🟢 High (≥ 70%)
* Buzzer alert when water level is **too low**
* Fully wireless monitoring via browser (WiFi)

---

## 🔧 Components Used

* **ESP32 Development Board**
* **HC-SR04 Ultrasonic Sensor**
* **0.96" SSD1306 OLED Display**
* **5V Relay Module** (Active HIGH or LOW configurable)
* **Buzzer**
* **LED Indicators** (Low / Mid / High)
* **Water Tank Setup**
* **Dot Board / Wiring / Soldering**

---

## 🛠 How It Works

* ESP32 continuously reads distance from the ultrasonic sensor
* Calculates **actual water height** using tank & sensor dimensions
* Converts height → percentage
* Updates:

  * OLED Display
  * LED indicators
  * Motor relay
  * Buzzer
  * Web dashboard via `/data` API

Motor remains ON until the tank reaches 100%
Motor turns ON again only when water level drops below 20%

---

## 🌐 Web Dashboard Preview

A fully responsive HTML dashboard featuring:

* Live statistics
* Animated water-tank visualization
* Status indicators
* Auto-refresh every 2 seconds
* Works on phone + laptop

---

## 🚀 Applications

* Smart home water tank monitoring
* Automatic motor control systems
* Apartment water management
* Industrial tank level monitoring
* Anywhere real-time water level tracking is needed

---

## 🔮 Future Improvements

* Add manual motor control via dashboard
* Add data logging + graph history
* Add notification alerts (SMS / WhatsApp / Firebase)
* Convert prototype to a **custom PCB**
* Integrate with home automation (Home Assistant, MQTT, etc.)

---

