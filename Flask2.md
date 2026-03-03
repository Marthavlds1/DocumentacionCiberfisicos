---
layout: default
title: Backend y Fronted remoto
nav_order: 2
parent: Interfaz Web Flask
permalink: /Practicas/Interfaz-Web-Flask/Backend-y-Fronted-remoto/
---
# Práctica: Control de Tira LED con ESP32 vía Red Local (Frontend/Backend en PCs distintas)

**Materia:** Sistemas Ciberfísicos  
**Institución:** Universidad Iberoamericana A.C.  
**Alumno:** _[Tu nombre]_  
**Fecha:** _[Fecha de entrega]_

---

## 1. Objetivo

Diseñar e implementar un sistema de control de una tira LED WS2812B conectada a una ESP32, donde la interfaz gráfica (frontend) y el servidor de control (backend) corren en **computadoras físicamente distintas** dentro de la misma red local, demostrando la separación de responsabilidades en arquitecturas cliente-servidor aplicadas a sistemas ciberfísicos.

### Objetivos específicos

- Desplegar un servidor Flask (API REST) en una PC dedicada al backend.
- Servir la interfaz web desde una segunda PC con una IP diferente.
- Conectar la ESP32 a la misma red para hacer polling al backend y controlar la tira LED en tiempo real.
- Validar la comunicación entre los tres nodos: Frontend → Backend ← ESP32.

---

## 2. Arquitectura del Sistema

```
[PC Frontend : 172.16.0.243:3000]
        │
        │  HTTP POST /api/state  (color, count)
        ▼
[PC Backend : 172.16.0.167:5000]   ←── HTTP GET /api/state (polling 500ms) ── [ESP32]
        │                                                                            │
        │  state.json (persistencia)                                                 ▼
        └────────────────────────────────────────────────────────────────► Tira LED WS2812B
```

| Nodo | IP | Puerto | Tecnología |
|---|---|---|---|
| Frontend | 172.16.0.243 | 3000 | HTML + CSS + JS (http.server) |
| Backend | 172.16.0.167 | 5000 | Python Flask + Flask-CORS |
| ESP32 | DHCP (WiFi) | — | Arduino C++ |

---

## 3. Componentes y Materiales

| Componente | Especificación |
|---|---|
| Microcontrolador | ESP32 Dev Module |
| Tira LED | WS2812B (8 LEDs) |
| Backend | Python 3.x + Flask 2.2.5 + Flask-CORS |
| Frontend | HTML5 / CSS3 / JavaScript vanilla |
| Red | WiFi LAN compartida entre ambas PCs y ESP32 |
| IDE Arduino | Arduino IDE 2.x con soporte ESP32 by Espressif |

---

## 4. Estructura del Proyecto

```
led-esp32-split/
├── backend/
│   ├── app.py              # Servidor Flask (API REST)
│   ├── requirements.txt    # Dependencias Python
│   └── data/
│       └── state.json      # Estado persistente del LED
├── frontend/
│   ├── index.html          # Interfaz de usuario
│   ├── app.js              # Lógica de comunicación con el backend
│   └── styles.css          # Estilos visuales
└── esp32-led-fisico.ino    # Firmware de la ESP32
```

---

## 5. Desarrollo

### 5.1 Backend — Servidor Flask

El backend expone tres endpoints REST:

| Método | Endpoint | Descripción |
|---|---|---|
| GET | `/api/state` | Devuelve el estado actual (color + cantidad de LEDs) |
| POST | `/api/state` | Actualiza el estado desde el frontend |
| GET | `/api/health` | Health check del servidor |

El servidor usa `CORS(app, resources={r"/api/*": {"origins": "*"}})` para permitir peticiones desde cualquier origen, lo que es fundamental cuando frontend y backend están en IPs distintas.

#### Código del backend (`app.py`)

```python
# [PEGAR AQUÍ EL CONTENIDO DE app.py]
```

#### Captura del backend corriendo

> 📸 _[Insertar screenshot de la terminal con Flask corriendo y la IP visible]_

---

### 5.2 Frontend — Interfaz Web

La interfaz permite:
- Cambiar el color de los LEDs con un color picker.
- Ajustar la cantidad de LEDs encendidos (0–8) con un slider.
- Ver el estado de conexión en tiempo real (✅ Conectado / ❌ Sin conexión).
- Cambiar la URL del backend desde la propia interfaz sin editar código.

La línea clave en `app.js` que apunta al backend es:

```javascript
const DEFAULT_API = "http://172.16.0.167:5000";
```

El frontend se sirve desde la segunda PC con:

```bash
cd frontend
python -m http.server 3000
```

Y se accede desde el navegador en:

```
http://172.16.0.243:3000
```

#### Código del frontend (`app.js`)

```javascript
// [PEGAR AQUÍ EL CONTENIDO DE app.js]
```

#### Captura de la interfaz en el navegador

> 📸 _[Insertar screenshot de la interfaz web con "✅ Conectado" visible]_

---

### 5.3 Firmware ESP32 (`esp32-led-fisico.ino`)

La ESP32 realiza polling cada 500 ms al endpoint `/api/state` del backend. Al recibir la respuesta JSON, interpreta el color hexadecimal y la cantidad de LEDs y actualiza la tira WS2812B en consecuencia.

Parámetros configurados en el `.ino`:

```cpp
const char* ssid      = "NOMBRE_DE_RED";
const char* password  = "CONTRASEÑA";
const char* serverUrl = "http://172.16.0.167:5000/api/state";
```

#### Código del firmware

```cpp
// [PEGAR AQUÍ EL CONTENIDO DE esp32-led-fisico.ino]
```

#### Captura del Monitor Serie

> 📸 _[Insertar screenshot del Monitor Serie de Arduino IDE mostrando la conexión WiFi y el polling]_

---

## 6. Pasos de Instalación y Ejecución

### PC Backend

```bash
# 1. Navegar a la carpeta backend
cd "ruta/al/proyecto/backend"

# 2. Instalar dependencias
pip install flask==2.2.5 flask-cors

# 3. Correr el servidor
python app.py
```

### PC Frontend

```bash
# 1. Navegar a la carpeta frontend
cd "ruta/al/proyecto/frontend"

# 2. Servir los archivos estáticos
python -m http.server 3000
```

Abrir en el navegador: `http://<IP_PC_FRONTEND>:3000`

### ESP32

1. Abrir `esp32-led-fisico.ino` en Arduino IDE.
2. Instalar soporte ESP32: **Archivo → Preferencias → URLs adicionales:**
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Instalar placa: **Herramientas → Gestor de placas → esp32 by Espressif**
4. Cambiar `ssid`, `password` y `serverUrl` con los datos de la red y la IP del backend.
5. Seleccionar placa **ESP32 Dev Module** y el puerto correcto.
6. Subir el código.

---

## 7. Resultados

### Logs del backend durante la prueba

```
172.16.0.243 - - [03/Mar/2026 08:47:20] "OPTIONS /api/state HTTP/1.1" 200 -
172.16.0.243 - - [03/Mar/2026 08:47:20] "POST /api/state HTTP/1.1" 200 -
172.16.0.243 - - [03/Mar/2026 08:47:31] "GET /api/state HTTP/1.1" 200 -
```

Los logs confirman que:
- El frontend (`172.16.0.243`) envía `POST` al mover los controles.
- La ESP32 hace `GET` periódicos para leer el estado.

### Evidencia fotográfica

> 📸 _[Insertar foto/video de la tira LED respondiendo a los cambios en la interfaz]_

> 📸 _[Insertar screenshot del estado "✅ Conectado" en la interfaz]_

---

## 8. Análisis y Discusión

### Separación de responsabilidades

La arquitectura implementada replica un patrón real de sistemas distribuidos: el frontend solo se preocupa por la presentación y la captura de input del usuario, mientras que el backend gestiona el estado y la persistencia. La ESP32 es un cliente más de esa API, igual que lo es el navegador.

### CORS

Al correr frontend y backend en IPs distintas, el navegador aplica la política de mismo origen (Same-Origin Policy). Flask-CORS resuelve esto añadiendo los headers `Access-Control-Allow-Origin: *` a todas las respuestas de la API, lo que se evidencia en las peticiones `OPTIONS` (preflight) que aparecen en los logs antes de cada `POST`.

### Polling vs WebSockets

El firmware usa polling cada 500 ms, lo que introduce una latencia máxima de 500 ms entre que el usuario mueve un slider y los LEDs responden. Para aplicaciones de tiempo real más exigentes, una mejora sería usar WebSockets o Server-Sent Events (SSE) para que el backend notifique a la ESP32 en lugar de esperar a ser consultado.

### Persistencia con `state.json`

El estado se persiste en un archivo JSON local, lo que permite que la ESP32 recupere el último estado conocido si se reconecta después de una desconexión. La escritura se hace de forma atómica (archivo temporal + rename) para evitar corrupción.

---

## 9. Conclusiones

- Se logró separar correctamente frontend y backend en dos PCs con IPs distintas (`172.16.0.243` y `172.16.0.167`), demostrando que la arquitectura es independiente del host.
- La ESP32 se integró como un tercer cliente de la misma API REST, validando la interoperabilidad del sistema.
- CORS es un componente crítico en cualquier arquitectura web donde el cliente y el servidor tienen orígenes distintos.
- El patrón cliente-servidor con polling es funcional para este caso de uso, pero presenta limitaciones de latencia que se pueden resolver con comunicación por eventos.

---

## 10. Referencias

- [Flask Documentation](https://flask.palletsprojects.com/)
- [Flask-CORS Documentation](https://flask-cors.readthedocs.io/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [WS2812B Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)
- [MDN — Same-Origin Policy](https://developer.mozilla.org/en-US/docs/Web/Security/Same-origin_policy)