---
layout: default
title: Sistemas Embebidos
nav_order: 2
parent: Practicas
permalink: /Practicas/Sistemas-Embebidos/
---
# Reporte de Práctica: Programación de Microcontroladores

## 1. ¿Qué son los sistemas embebidos?
Los **sistemas embebidos** son sistemas de cómputo diseñados para ejecutar una función específica dentro de un sistema más amplio. Se integran directamente a un dispositivo físico y están optimizados para tareas como:
* **Control y Monitoreo**
* **Adquisición de datos**
* **Comunicación con otros sistemas (Ciberfísicos)**



[Image of an embedded system block diagram]


---

## 2. Plataformas Utilizadas
Durante la práctica se compararon diversas arquitecturas para evaluar sus capacidades y entornos de programación.

| Plataforma | Microcontrolador | Arquitectura | Núcleos | Conectividad | Documentación |
| :--- | :--- | :--- | :---: | :---: | :--- |
| **Arduino Uno / Nano** | ATmega328P | AVR 8-bit | 1 | No | [Link](https://marthavlds1.github.io/DocumentacionCiberfisicos/Practicas/Sistemas-Embebidos/#arduino-uno) |
| **XIAO RP2040** | RP2040 | ARM Cortex-M0+ | 2 | No | [Link](https://marthavlds1.github.io/DocumentacionCiberfisicos/Practicas/Sistemas-Embebidos/#rp2040) |
| **ESP32 DevKit V1** | ESP32-WROOM-32 | Xtensa LX6 | 2 | Wi-Fi / BT | [Link](https://marthavlds1.github.io/DocumentacionCiberfisicos/Practicas/Sistemas-Embebidos/#esp32-devkit-v1-esp32wroom32) |
| **XIAO ESP32S3 Sense** | ESP32-S3 | Xtensa LX7 | 2 | Wi-Fi / BT | [Link](https://marthavlds1.github.io/DocumentacionCiberfisicos/Practicas/Sistemas-Embebidos/#xiao-esp32s3-sense) |

---

## 3. Entornos de Desarrollo
1. **Arduino IDE:** Programación en **C/C++** para acceso directo al hardware.
2. **MicroPython (Thonny):** Lenguaje de alto nivel para prototipado rápido y legible.

---

## 4. Resultados: Prueba de Blink
Se verificó el funcionamiento cargando el programa de parpadeo de LED en cada placa.

### 4.1 Arduino Uno / Nano (C++)
| Código Fuente | Evidencia Visual |
| :--- | :--- |
| ```cpp void setup() { pinMode(LED_BUILTIN, OUTPUT); } void loop() { digitalWrite(LED_BUILTIN, HIGH); delay(500); digitalWrite(LED_BUILTIN, LOW); delay(500); } ``` | <video src="assets/videos/Arduino1.mp4" width="300" controls></video> |

### 4.2 XIAO RP2040 (MicroPython)
| Código Fuente | Evidencia Visual |
| :--- | :--- |
| ```python from machine import Pin import time LED = Pin(17, Pin.OUT) while True: LED.value(0) # ON time.sleep(0.5) LED.value(1) # OFF time.sleep(0.5) ``` | <video src="assets/videos/20401.mp4" width="300" controls></video> |

### 4.3 ESP32 DevKit V1 (MicroPython)
| Código Fuente | Evidencia Visual |
| :--- | :--- |
| ```python from machine import Pin import time LED = Pin(2, Pin.OUT) while True: LED.value(1) time.sleep(0.5) LED.value(0) time.sleep(0.5) ``` | <video src="assets/videos/ESP321.mp4" width="300" controls></video> |

### 4.4 XIAO ESP32S3 Sense (MicroPython)
| Código Fuente | Evidencia Visual |
| :--- | :--- |
| ```python from machine import Pin import time LED = Pin(21, Pin.OUT) while True: LED.value(0) # ON time.sleep(0.5) LED.value(1) # OFF time.sleep(0.5) ``` | <video src="assets/videos/S31.mp4" width="300" controls></video> |










