---
layout: default
title: Interface Gráfica de Usuario
nav_order: 3
parent: Practicas
permalink: /Practicas/Interface-Gráfica-de-Usuario/
---
# Interfaces Gráficas (GUI) y Comunicación Inalámbrica (BLE)


En la ingeniería de sistemas ciberfísicos, la integración de hardware con **Interfaces Hombre-Máquina (HMI)** es fundamental. Este reporte detalla la creación de un sistema de control bidireccional utilizando el **ESP32-S3** y una interfaz desarrollada en **Python (PyQt6)**. Se analiza la evolución desde una comunicación cableada (UART) hacia una inalámbrica mediante **Bluetooth Low Energy (BLE)**.



---

## 2. Objetivos
* **General:** Diseñar un sistema de control que integre hardware y software, validando la integridad de datos en comunicación serie e inalámbrica.
    * Estructurar una GUI multihilo para evitar el bloqueo del renderizado.
    * Implementar un protocolo de mensajería para el control de GPIOs.
    * Configurar un servidor GATT en el ESP32-S3 para conexión BLE.

---

## 3. Tecnologías y Herramientas

### Arquitectura del XIAO ESP32-S3
El módulo Seeed Studio XIAO ESP32S3 destaca por su motor de aceleración de IA y su gestión avanzada de energía. Para este laboratorio, su relevancia radica en el soporte nativo de USB-Serial y su pila BLE 5.0, permitiendo pruebas de comunicación de alta velocidad.

### UART y el flujo de datos asíncrono
La comunicación UART (Universal Asynchronous Receiver-Transmitter) se basa en la sincronización por reloj interno entre dos nodos. En este proyecto, la UART permite una depuración directa y un flujo de datos bidireccional simple pero efectivo para comandos AT-Style.

### Bluetooth Low Energy (BLE) y Perfil GATT
A diferencia del Bluetooth clásico, BLE utiliza una jerarquía de datos:
* Servicios: Agrupaciones lógicas de funciones (identificadas por UUID).
* Características: Los puntos de datos finales donde se lee, escribe o se notifican cambios. El uso de Notificaciones es crítico aquí, ya que permite al servidor (ESP32) enviar datos al cliente (PC) sin que este los solicite explícitamente.

### 4. Componentes Usados 

| Componente | Especificación |
| :--- | :--- |
| MCU | XIAO ESP32-S3 (GPIO 2 como entrada, GPIO 3 como salida) |
| Entradas | Pulsador táctil con configuración INPUT_PULLUP |
| Salidas | LED difuso con resistencia limitadora de corriente de 220Ohms |
| IDE/Lenguaje | Arduino IDE 2.3 (C++) y VS Code (Python 3.11) |
| Librerías GUI | PyQt6 para la gestión de señales y slots |
| Librerías Comm | pyserial para UART y BLEDevice.h para el stack inalámbrico |


