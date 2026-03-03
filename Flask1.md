---
layout: default
title: Flask Local 
nav_order: 1
parent: Interfaz Web Flask
permalink: /Practicas/Interfaz-Web-Flask/Flask-Local/
---
# Practica: Interfaz Web Flask + Servidor Front para Control de LED en ESP32 Fisica

## Descripcion General

Esta practica documenta el proceso de creacion de una interfaz web para controlar una tira de LEDs NeoPixel WS2812B conectada a un ESP32 fisico. Se implemento una arquitectura de dos servidores separados: un backend Flask que expone una API REST, y un frontend estatico HTML/JS/CSS que consume dicha API. La ESP32 consulta el estado del servidor cada 500 ms y aplica el color y cantidad de LEDs indicados.

---

## 1. Conceptos Fundamentales

| Concepto | Descripcion |
|----------|-------------|
| **Flask** | Microframework de Python para crear servidores web y APIs REST |
| **CORS** | Politica del navegador que permite o bloquea peticiones entre distintos origenes |
| **API REST** | Interfaz de comunicacion basada en HTTP con metodos GET y POST |
| **Polling** | Tecnica en la que el cliente consulta periodicamente al servidor para detectar cambios |
| **NeoPixel WS2812B** | LEDs RGB direccionables individualmente mediante un solo pin de datos |
| **IP Local** | Direccion de red dentro de una LAN, usada para comunicar dispositivos sin internet |
| **Servidor Front** | Servidor que sirve archivos estaticos (HTML, CSS, JS) al navegador |

---

## 2. Arquitectura del Sistema

```
[Navegador / iPad / Celular]
        |
        | HTTP POST /api/state
        v
[Servidor Front :5500]     [Backend Flask :5000]
  index.html          <-->  GET  /api/state
  app.js                    POST /api/state
  styles.css
                                  ^
                                  | HTTP GET cada 500 ms
                                  |
                           [ESP32 Fisica]
                           Tira WS2812B (8 LEDs)
```

> _[Insertar diagrama o foto del sistema fisico aqui]_

---

## 3. Tecnologias y Herramientas

| Componente | Especificacion |
|------------|----------------|
| Lenguaje Backend | Python 3 + Flask |
| Lenguaje Frontend | HTML5 + CSS3 + JavaScript (vanilla) |
| Libreria CORS | flask-cors |
| Microcontrolador | ESP32 |
| Tira LED | WS2812B - 8 LEDs (NeoPixel) |
| Libreria Arduino | Adafruit NeoPixel + ArduinoJson |
| Puerto Backend | 5000 |
| Puerto Frontend | 5500 |
| Protocolo | HTTP / TCP-IP |

---

## 4. Estructura del Proyecto

```
Proyecto .zip: assets/files/FlaskLocal.zip
led-esp32-split/
├── backend/
│   ├── app.py              <- API Flask (solo backend)
│   ├── requirements.txt
│   ├── Procfile
│   └── data/
│       └── state.json      <- Estado persistente del LED
├── frontend/
│   ├── index.html          <- Interfaz de control
│   ├── app.js              <- Logica del frontend
│   └── styles.css          <- Estilos
└── esp32-led-fisico.ino    <- Codigo Arduino
```

---

## 5. Desarrollo Tecnico

### Etapa 1 - Configuracion del Backend Flask

El archivo `app.py` expone dos endpoints principales sin servir ningun archivo estatico. CORS esta abierto para permitir peticiones desde cualquier origen.

**Endpoints:**

| Metodo | Ruta | Descripcion |
|--------|------|-------------|
| GET | `/api/state` | Devuelve el estado actual `{color, count, rev}` |
| POST | `/api/state` | Recibe `{color, count}` y actualiza el estado |
| GET | `/api/health` | Verificacion de que el servidor esta activo |

**Codigo `app.py`:**

```python
# [Pegar aqui el contenido de app.py]
```

> _[Insertar captura de pantalla del servidor corriendo en terminal]_

---

### Etapa 2 - Configuracion del Frontend

El frontend es completamente independiente del backend. Se comunica con el mediante `fetch()` apuntando a la IP y puerto del servidor Flask.

**Configuracion de la URL del backend en `app.js`:**

```javascript
const DEFAULT_API = "http://TU_IP:5000";
```

**Codigo `index.html`:**

```html
<!-- [Pegar aqui el contenido de index.html] -->
```

**Codigo `app.js`:**

```javascript
// [Pegar aqui el contenido de app.js]
```

> _[Insertar captura de la interfaz web en el navegador]_

---

### Etapa 3 - Codigo Arduino (ESP32)

La ESP32 se conecta al WiFi, y en el `loop()` consulta el endpoint `/api/state` cada 500 ms. Si el campo `rev` cambio respecto al ultimo valor conocido, aplica el nuevo color y cantidad a la tira NeoPixel.

**Parametros configurados en el `.ino`:**

```cpp
const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_PASSWORD";
const char* STATE_URL = "http://TU_IP:5000/api/state";
```

**Codigo completo `esp32-led-fisico.ino`:**

```cpp
// [Pegar aqui el contenido del .ino]
```

**Diagrama de conexion fisica (wiring):**

| ESP32 | Tira WS2812B |
|-------|--------------|
| GPIO 5 | DIN |
| GND | GND |
| Fuente 5V externa | VCC |

> _[Insertar foto del circuito fisico / wiring real]_

---

### Etapa 4 - Arranque de los Servidores

Se requieren **dos terminales abiertas simultaneamente**.

**Terminal 1 - Backend:**

```bash
cd backend
pip install flask flask-cors
python app.py
```

Salida esperada:
```
API corriendo en      http://0.0.0.0:5000
ESP32 apunta a        http://TU_IP:5000/api/state
```

> _[Insertar captura de la terminal del backend]_

**Terminal 2 - Frontend:**

```bash
cd frontend
python -m http.server 5500 --bind 0.0.0.0
```

Salida esperada:
```
Serving HTTP on 0.0.0.0 port 5500 ...
```

> _[Insertar captura de las dos terminales corriendo al mismo tiempo]_

---

### Etapa 5 - Flasheo de la ESP32

1. Abrir `esp32-led-fisico.ino` en Arduino IDE
2. Seleccionar placa: `Tools > Board > ESP32 Dev Module`
3. Seleccionar puerto COM correcto
4. Presionar **Upload**
5. Abrir Monitor Serial a **115200 baud**

**Salida esperada en Monitor Serial:**

```
[WiFi] Conectando.....
[WiFi] Conectado - IP: 192.168.x.x
[LED] rev=2  color=#ff0000  count=8
```

> _[Insertar captura del Monitor Serial]_

---

## 6. Resultados y Evidencia

### Interfaz web funcionando en PC

> _[Insertar captura de la interfaz en el navegador de la PC con badge "Conectado"]_

### Interfaz web funcionando desde otro dispositivo

Acceso desde dispositivo externo en la misma red WiFi:

```
http://TU_IP:5500/index.html
```

> _[Insertar captura de la interfaz desde el iPad o celular]_

### ESP32 respondiendo a los cambios

> _[Insertar foto de la tira LED encendida con el color seleccionado desde la web]_

### Logs del backend mostrando el polling de la ESP32

> _[Insertar captura de la terminal con los GET /api/state cada 500 ms]_

---

## 7. Nota Tecnica - Firewall

Para que otros dispositivos en la red puedan acceder a los servidores, fue necesario abrir los puertos en el Firewall de Windows ejecutando los siguientes comandos en **PowerShell como administrador**:

```powershell
netsh advfirewall firewall add rule name="Flask 5000" dir=in action=allow protocol=TCP localport=5000
netsh advfirewall firewall add rule name="Front 5500" dir=in action=allow protocol=TCP localport=5500
```

---

## 8. Analisis y Discusion

### Diferencia entre servidor local y servidor remoto

El servidor Flask local esta disponible unicamente dentro de la red WiFi donde corre la PC. A diferencia de un servicio en la nube (como Render o Railway), no requiere internet externo, lo que lo hace ideal para laboratorio y prototipado con hardware fisico como la ESP32.

### Integracion con Sistemas Ciberfisicos

Esta arquitectura demuestra un patron clasico de sistemas ciberfisicos: un actuador fisico (tira LED) es controlado en tiempo real a traves de una interfaz web, con la logica de estado centralizada en un servidor intermedio. La ESP32 actua como cliente HTTP que sincroniza su estado con el servidor.

### IP dinamica como limitacion

Durante la practica se observo que la IP local de la PC cambia al reiniciar o cambiar de red. Para un sistema mas robusto seria conveniente asignar una IP estatica a la PC dentro del router, o usar un servicio de DNS local.

### Latencia del sistema

Con un intervalo de polling de 500 ms, el tiempo maximo de respuesta desde que el usuario mueve un slider hasta que el LED cambia es de aproximadamente **500-700 ms**, lo cual es aceptable para esta aplicacion.

---

## 9. Conclusiones

> _[Escribir conclusiones personales aqui]_

---

## 10. Referencias

- Documentacion oficial de Flask: https://flask.palletsprojects.com
- Libreria Adafruit NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel
- ArduinoJson: https://arduinojson.org
- _[Agregar mas referencias segun sea necesario]_