---
layout: default
title: Protocolo Comunicacion
nav_order: 3
parent: Practicas
permalink: /Practicas/Protocolo-Comunicacion/
---

# Protocolos de Comunicación

## 1. ¿Qué es un Sistema Embebido?
A diferencia de una computadora personal, un **sistema embebido** es un sistema de computación diseñado para realizar funciones específicas o dedicadas, generalmente dentro de un sistema mecánico o eléctrico mayor.


**Características principales:**
* **Especialización:** Diseñados para una tarea concreta.
* **Eficiencia:** Optimizados para bajo consumo de energía y costo reducido.
* **Limitaciones técnicas:** Memoria y capacidad de procesamiento ajustadas a la necesidad del proyecto.
* **Interacción física:** Capacidad de leer sensores y controlar actuadores en el mundo real.



---

## 2. Protocolos de Comunicación Analizados
Para que los sistemas embebidos "hablen" entre sí o con otros periféricos, utilizan protocolos estándar. En esta práctica exploramos tres de los más importantes:

### A. UART (Universal Asynchronous Receiver-Transmitter)
Es un protocolo de comunicación serie **asíncrona**, lo que significa que no necesita una señal de reloj compartida.
* **Conexión:** Utiliza dos líneas principales, **TX** (transmisión) y **RX** (recepción).
* **Modo:** Full-duplex (puede enviar y recibir datos al mismo tiempo).
* **Uso común:** Consolas de depuración, módulos GPS y Bluetooth.

### B. I2C (Inter-Integrated Circuit)
Protocolo de tipo **síncrono** que utiliza una arquitectura Maestro-Esclavo.
* **Conexión:** Solo requiere dos hilos: **SDA** (datos) y **SCL** (reloj).
* **Ventaja:** Permite conectar múltiples dispositivos (hasta 127) usando el mismo par de cables mediante direcciones únicas.
* **Uso común:** Sensores de temperatura, acelerómetros y pantallas OLED.

### C. SPI (Serial Peripheral Interface)
Es el protocolo más rápido de los tres, operando de forma **síncrona** a altas velocidades.
* **Conexión:** Utiliza cuatro hilos (**MOSI, MISO, SCK, SS**).
* **Ventaja:** Transferencia de datos a muy alta velocidad sin la complejidad de direccionamiento de I2C.
* **Uso común:** Tarjetas SD, pantallas LCD de alta resolución y memorias Flash.

---

## 3. Plataformas de Prueba
En este reporte se comparó el rendimiento de tres arquitecturas distintas:
1. **ATMEGA328 (Arduino Nano/Uno):** Arquitectura de 8 bits, ideal para tareas sencillas.
2. **ESP32:** Potente procesador de 32 bits con conectividad Wi-Fi y Bluetooth integrada.
3. **RP2040 (Raspberry Pi Pico):** Procesador de doble núcleo con alta flexibilidad en sus pines de entrada/salida.

---

## 4. Resultados de la Práctica: Latencia y Baudrate
*Aquí es donde debes insertar la información específica de tus experimentos.*

### Comparativa de Latencia
> **Instrucciones:** Identifica el baudrate máximo de cada plataforma y mide el tiempo que tarda un mensaje en llegar de un dispositivo a otro tras 1000 envíos.

#### Gráficas de Latencia obtenidas:
* **Prueba UART (Plataforma A vs B):**
  *(Inserta aquí tu imagen o descripción de resultados)*

* **Prueba I2C (Plataforma B vs C):**
  *(Inserta aquí tu imagen o descripción de resultados)*

* **Prueba SPI (Plataforma A vs C):**
  *(Inserta aquí tu imagen o descripción de resultados)*

### Conclusiones de la medición
- **Plataforma más rápida:** [Escribir aquí]
- **Protocolo con menor latencia:** [Escribir aquí]
- **Observaciones:** [Escribir aquí sobre la desviación estándar o errores encontrados]