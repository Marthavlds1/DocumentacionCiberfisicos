---
layout: default
title: Interface Gráfica de Usuario
nav_order: 3
parent: Practicas
permalink: /Practicas/Interface-Gráfica-de-Usuario/
---
# Práctica: Interfaces Gráficas (GUI) y Comunicación Inalámbrica (BLE)

## 1. Introducción
En la ingeniería de sistemas ciberfísicos, la capacidad de interactuar con el hardware a través de **Interfaces Hombre-Máquina (HMI)** es vital. Esta práctica documenta el desarrollo de una aplicación en Python (PyQt6) para controlar un ESP32-S3, migrando de una comunicación cableada (UART) a una inalámbrica mediante Bluetooth Low Energy (BLE).



---

## 2. Objetivos y Arquitectura
* **Objetivo:** Diseñar una GUI multihilo que permita el control bidireccional sin bloquear el renderizado de la interfaz.
* **Protocolos:** Implementación de mensajería por bus serie y servidor GATT (Generic Attribute Profile).

### Tabla de Tecnologías Utilizadas
| Componente | Herramienta / Protocolo | Función |
| :--- | :--- | :--- |
| **Hardware** | XIAO ESP32-S3 | Procesamiento y servidor BLE |
| **Lenguaje GUI** | Python + PyQt6 | Creación de la interfaz visual |
| **Librería Serial** | PySerial | Comunicación UART inicial |
| **Librería Wireless** | Bleak / Pybluez2 | Gestión de conexión Bluetooth |

---

## 3. Diseño de la Interfaz (Wireframe)
Antes de la programación, se estructuró un esqueleto visual (Wireframe) para definir el flujo de la aplicación:
1. **Conexión:** Selección de puerto COM o búsqueda de dispositivo BLE.
2. **Control:** Botón virtual para encendido/apagado de LED físico.
3. **Monitoreo:** Indicador visual que reacciona al presionar el Push Button físico de la PCB.

> *Espacio para imagen: Wireframe de la interfaz (Figma/Canva)*
> ![Wireframe](tu_ruta_a_imagen)

---

## 4. Desarrollo y Resultados

### 4.1 Implementación de la GUI (Python)
Se utilizó un diseño orientado a objetos para manejar la lógica de la ventana y un **hilo secundario (Thread)** para la escucha de datos, evitando que la interfaz se "congele" al esperar respuesta del microcontrolador.

#### Código Esencial (Estructura de Comunicación)
```python
# Ejemplo de envío de comando desde la GUI
    def enviar_comando(self, comando):
        if self.serial.is_open:
            self.serial.write(comando.encode())
            print(f"Enviado: {comando}")