---
layout: default
title: Protocolos de Comunicación
nav_order: 3
parent: Practicas
permalink: /Practicas/Protocolos-de-Comunicación/
---

# Protocolos de Comunicación

## 1. ¿Qué es un Sistema Embebido?
A diferencia de una computadora personal, un **sistema embebido** es un sistema de computación diseñado para realizar funciones específicas o dedicadas, generalmente dentro de un sistema mecánico o eléctrico mayor.

**Características principales:**
* **Especialización:** Diseñados para una tarea concreta.
* **Eficiencia:** Optimizados para bajo consumo de energía y costo reducido.
* **Limitaciones técnicas:** Memoria y capacidad de procesamiento ajustadas a la necesidad del proyecto.
* **Interacción física:** Capacidad de leer sensores y controlar actuadores en el mundo real.

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