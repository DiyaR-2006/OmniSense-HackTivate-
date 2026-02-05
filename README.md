## OmniSense: Energy Optimization and Alert Smart System 

OmniSense bridges the gap between traditional energy optimization, security and sustainability. By combining ESP32-based automation with Blynk IoT connectivity, we’ve created a low-cost, zero-invasive solution for energy management and student safety.
Unlike standard IoT prototypes, OmniSense features an Offline Fail-Safe via a local LCD interface, ensuring that critical safety alerts and energy logic persist regardless of Wi-Fi stability. It’s more than a project - something we will be needing and using in the near future.

## Quick Links
- Live Simulation: offline: (https://wokwi.com/projects/453951590301233153)
                   online : (https://wokwi.com/projects/454308317695279105)
- Demo Link (youtube) : https://youtu.be/1yirrnzVDjc?feature=shared

## Key Features
- **Climate Control**: Auto-activates Fan based on DHT22 thresholds.
- **Occupancy Lighting**: PIR-based energy saving.
- **Safety**: Real-time MQ-2 smoke monitoring with audible alarms used as proxy for simulation.
- **Cloud Integration**: Blynk IoT Dashboard for remote telemetry and manual control.

  
## Hardware Setup
- **Microcontroller**: ESP32
- **Sensors**: DHT22, PIR, MQ-2 (Used as proxy for simulation) 
- **Actuators**: Relay (Used as proxy for simulating fan), LED, Active Buzzer
- **Display**: 16x2 I2C LCD (Offline version)

## Software
- C++ 
- Blynk IoT Cloud (Online version)
- Wokwi (Simulation)

## Wiring (ESP32-S3)
- DHT22: GPIO 18
- PIR: GPIO 7
- Smoke: GPIO 4
- Relay: GPIO 12
- LED: GPIO 15
- Buzzer: GPIO 13

## Blynk Datastreams
- V0: Temperature
- V1: Humidity
- V2: AQI/Smoke
- V5: Occupancy
- V7: Light Switch
- V8: Fan Switch

## System Architecture

1. Perception: Sensors poll environmental data and occupancy status.
2. Edge Processing: ESP32 evaluates logic thresholds locally (Offline-ready).
3. Communication: Data syncs to Blynk Cloud via Wi-Fi.
4. Action: Relays toggle power, and LCD/Blynk Dashboards update in real-time.

## Testing Stages & Future Scope
- Current Limitation: Utilizing simulated inputs for RFID and IR sensors within the Wokwi environment.
- Future Scope: Transitioning to MQ135 sensors and implementing ESP-NOW mesh networking for wide scalability.




## **END OF DOCUMENTATION**
