# 🧼 IR-Based Battery Powered Soap Dispenser (Low Power & Cost Effective)

## 📌 Overview
This project is a **battery-powered automatic soap dispenser** using a custom-built **IR proximity sensing circuit**. It detects the presence of a hand and activates a motor to dispense soap.

The system is designed with two main goals:
- ⚡ **Ultra-low power consumption**
- 💰 **Cost-effective hardware design**

It combines **analog signal processing (LM393)** with **digital control (ATTiny85)** for efficient and reliable operation.

---

## ⚙️ Key Features
- ✋ Touchless operation using IR reflection
- 🔋 6V battery-powered system
- ⚡ Ultra low power consumption using sleep modes
- 💰 Cost-effective design using discrete components
- 🧠 Hybrid system (Comparator + Microcontroller)
- 🎯 Duty-cycled IR sensing (not always ON)
- ⏱️ Controlled dispensing duration (~3 seconds)
- 💤 Deep sleep using Watchdog Timer (WDT)

---

## 🧰 Hardware Components
- **Microcontroller:** ATTiny85  
- **Comparator:** LM393  
- **IR LED + Photodiode**  
- **MOSFET:** IRLZ44N (motor driver)  
- **Motor/Pump:** 6V DC Motor  
- **Battery:** 6V  
- **Potentiometer (10K):** Sensitivity adjustment  
- **Resistors & Passive Components**

---

## 🔌 Working Principle

### 1. IR Sensing (Duty-Cycled)
- IR LED is powered **only during sampling**
- Reduces continuous power consumption

### 2. Detection Stage
- Reflected IR is detected by photodiode
- Signal is compared with threshold using LM393
- Threshold adjusted via potentiometer

### 3. Control Logic (ATTiny85)
- Reads comparator output
- Validates detection
- Controls ON time of motor (~3 sec)
- Adds cooldown (~1 sec) to prevent retriggering

### 4. Motor Driver
- MOSFET (IRLZ44N) switches motor
- Ensures efficient current handling

---

## 🧠 Firmware Strategy (Power Optimization)

### 🔹 Deep Sleep Mode
- Uses `SLEEP_MODE_PWR_DOWN`
- MCU sleeps most of the time

### 🔹 Watchdog Timer (WDT)
- Wakes MCU periodically (every 500ms)

### 🔹 Duty Cycling (Key Feature)
- IR sensor powered ON for ~15ms only
- Sampling interval: 500ms

### 🔹 Peripheral Shutdown
- ADC disabled  
- Analog comparator disabled  
- Timers disabled  
- USI disabled  

### 🔹 Brown-Out Detector Disabled
- Reduces sleep current further

### 🔹 Smart Timing Logic
- Active state: ~3 seconds
- Cooldown: ~1 second

---

## 🔋 Power Efficiency

This project achieves excellent real-world power performance:

- 🔻 Voltage drop: **~0.01V per day**
- 📊 Condition: **Under frequent usage**
- 💤 Extremely low standby current

This ensures **long battery life** and practical usability.

---

## 💰 Cost Optimization

The circuit is designed to be **low-cost and production-friendly**.

### 🔹 Design Choices
- Discrete **IR LED + photodiode** instead of modules
- **LM393** used instead of complex signal ICs
- **ATTiny85** for low-cost control
- Minimal component count

### 🔹 Benefits
- Lower BOM cost
- Easy component availability
- Suitable for mass production
- Reliable performance without expensive parts

---

## 📊 Performance Summary

| Parameter            | Value                          |
|---------------------|--------------------------------|
| Power Source        | 6V Battery                     |
| Voltage Drop        | ~0.01V/day (frequent usage)    |
| Sampling Interval   | 500 ms                         |
| IR ON Time          | ~15 ms                         |
| Active Time         | ~3 seconds                     |
| Cooldown Time       | ~1 second                      |
| Control System      | ATTiny85 + LM393               |
| Output              | 6V Motor (via MOSFET)          |

---

## 🛠️ Circuit Architecture

### 🔹 IR Detection Stage
- IR LED + Photodiode
- Controlled via MCU (power gating)

### 🔹 Comparator Stage
- LM393 for signal conditioning
- Adjustable threshold

### 🔹 Control Stage
- ATTiny85 handles logic and timing

### 🔹 Driver Stage
- MOSFET switches motor efficiently

---

## 🚧 Challenges Faced
- Reducing idle power consumption
- Avoiding false triggers from ambient light
- Stable IR detection
- Efficient duty cycling implementation

---
## 📷 Circuit Diagram
<img width="1031" height="637" alt="image" src="https://github.com/user-attachments/assets/4b921a59-2c3c-405a-875d-edbec008ff3a" />

---
