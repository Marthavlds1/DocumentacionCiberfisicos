---
layout: default
title: Render y Github
nav_order: 3
parent: Interfaz Web Flask
permalink: /Practicas/Interfaz-Web-Flask/Render-y-Github/
---
# Control Remoto de LED ESP32 con Flask y Despliegue en la Nube (Render + GitHub Pages)

En la ingeniería de sistemas ciberfísicos, la integración de hardware embebido con servicios web accesibles desde internet es un componente esencial del desarrollo IoT moderno. Este reporte detalla la creación de un sistema de control remoto para una tira de LEDs NeoPixel WS2812B conectada a un ESP32, utilizando un backend Flask como intermediario y desplegándolo en la nube mediante Render. Se analiza la evolución desde una comunicación local (LAN) hacia un acceso global a través de internet.



## 1. Objetivos

**General:** Diseñar un sistema IoT completo que permita el control remoto de LEDs físicos desde una interfaz web, eliminando la restricción de red local y permitiendo acceso desde cualquier lugar del mundo.

- Desplegar un backend Flask en Render que exponga una API REST para leer y escribir el estado de los LEDs.
- Crear un frontend web (HTML, CSS, JS) que consuma la API y permita controlar color y cantidad de LEDs activos.
- Programar el ESP32 para hacer polling periódico a la API y reflejar el estado en la tira NeoPixel física.
- Publicar el código fuente en dos repositorios separados de GitHub (backend y frontend).
- Alojar el frontend estático en GitHub Pages para acceso público sin costo.

---

## 2. Tecnologías y Herramientas

### ESP32 y NeoPixel WS2812B
El ESP32 es un microcontrolador con WiFi integrado ampliamente utilizado en proyectos IoT. Su conectividad permite realizar peticiones HTTP hacia servidores remotos. La tira NeoPixel WS2812B permite controlar color RGB y cantidad de LEDs activos de manera individual mediante un solo pin de datos (GPIO 5).

### Flask y API REST
Flask es un microframework de Python para la creación rápida de APIs web. En este proyecto expone dos endpoints:
- `GET /api/state` — Devuelve el estado actual (color y cantidad de LEDs encendidos)
- `POST /api/state` — Actualiza el estado con nuevos valores enviados desde la interfaz

El estado se persiste en un archivo `state.json`, lo que garantiza que el ESP32 siempre consulte el último valor registrado.

### Render (Despliegue en la Nube)
Render es una plataforma de despliegue que permite hospedar aplicaciones web de forma gratuita. A diferencia de correr el servidor localmente, Render asigna una URL pública permanente accesible desde cualquier red. El servicio detecta automáticamente el archivo `Procfile` para iniciar la aplicación con Gunicorn.

### GitHub y GitHub Pages
Se utilizaron dos repositorios independientes:
- **`led-esp32-backend`** — Contiene la API Flask, conectado a Render para su despliegue automático.
- **`led-esp32-frontend`** — Contiene la interfaz web, publicada mediante GitHub Pages como sitio estático.

---

## 3. Componentes Usados

| Componente | Especificación |
|---|---|
| MCU | ESP32 (GPIO 5 como salida para datos NeoPixel) |
| Salidas | Tira NeoPixel WS2812B de 8 LEDs |
| Alimentación | Fuente 5V para la tira / USB para el ESP32 |
| IDE / Lenguaje (firmware) | Arduino IDE (C++) |
| IDE / Lenguaje (backend) | VS Code, Python 3, Flask |
| Librerías Arduino | Adafruit NeoPixel, ArduinoJson, WiFi, HTTPClient |
| Librerías Python | Flask, Flask-CORS, Gunicorn |
| Despliegue backend | Render (Free tier) |
| Despliegue frontend | GitHub Pages |

---

## 4. Desarrollo Técnico

### Etapa 1. Estructura del Proyecto y Repositorios

El proyecto se dividió en dos repositorios para separar responsabilidades y permitir despliegues independientes:

```
led-esp32-backend/
├── app.py              ← Servidor Flask con la API REST
├── Procfile            ← Instrucción de inicio para Render
├── requirements.txt    ← Dependencias Python (flask, flask-cors, gunicorn)
└── data/
    └── state.json      ← Persistencia del estado de los LEDs

led-esp32-frontend/
├── index.html          ← Interfaz de usuario
├── styles.css          ← Estilos visuales
└── app.js              ← Lógica de consumo de la API
```

> 📸 **[Agrega aquí una captura de tus dos repositorios en GitHub]**

---

### Etapa 2. Backend Flask — API REST

El servidor Flask expone un recurso `/api/state` que permite leer y escribir el estado de los LEDs. Se habilitó CORS para permitir peticiones desde el frontend alojado en GitHub Pages.

> 📋 **[Pega aquí el contenido completo de `app.py`]**

```python
# Ejemplo de estructura del endpoint
@app.route('/api/state', methods=['GET', 'POST'])
def state():
    # GET: leer state.json y retornar JSON
    # POST: recibir color y count, guardar en state.json
    pass
```

El archivo `requirements.txt` incluyó las dependencias necesarias:

```
flask
flask-cors
gunicorn
```

El `Procfile` indica a Render cómo iniciar el servidor:

```
web: gunicorn app:app
```

---

### Etapa 3. Despliegue en Render

El repositorio `led-esp32-backend` se conectó directamente a Render. Cada `push` a la rama `main` dispara un redeploy automático.

**Configuración usada en Render:**

| Campo | Valor |
|---|---|
| Runtime | Python 3 |
| Build Command | `pip install -r requirements.txt` |
| Start Command | `gunicorn app:app` |
| Instance Type | Free |
| URL asignada | `https://led-esp32-backend.onrender.com` |

> 📸 **[Agrega aquí una captura de Render mostrando el estado "Live"]**

> 📸 **[Agrega aquí una captura del resultado al visitar `/api/state` en el navegador]**

---

### Etapa 4. Frontend Web — Interfaz de Control

La interfaz fue diseñada con HTML, CSS y JavaScript puro, sin frameworks adicionales. Se conecta a la API mediante `fetch()` y actualiza la vista en tiempo real.

**Wireframe del Sistema:**
- **Header:** Indicador de estado de conexión con el backend (`✅ Conectado` / `❌ Sin conexión`)
- **Body:** Selector de color, control de cantidad de LEDs y previsualización visual de la tira
- **Footer:** Botones de acción rápida (Apagar todo, Encender todo, Guardar)

> 📸 **[Agrega aquí una captura de la interfaz web funcionando]**

La URL del backend se configura desde la propia interfaz y se persiste en `localStorage`:

```javascript
const DEFAULT_API = "https://led-esp32-backend.onrender.com";
let API_BASE = localStorage.getItem("led_api_base") || DEFAULT_API;
```

> 📋 **[Pega aquí el contenido completo de `app.js` si deseas mostrarlo]**

---

### Etapa 5. Firmware ESP32 — Polling a la API

El ESP32 se conecta a WiFi y consulta el endpoint `/api/state` cada 500 ms. Solo actualiza la tira física cuando detecta un cambio en el campo `rev` (número de revisión), evitando actualizaciones innecesarias.

```cpp
// Fragmento principal del loop
void loop() {
  ensureWiFi();
  if (millis() - lastPoll >= POLL_MS) {
    lastPoll = millis();
    fetchAndUpdate();
  }
}
```

**Datos clave de configuración:**

| Parámetro | Valor |
|---|---|
| `WIFI_SSID` | Nombre de tu red WiFi |
| `WIFI_PASS` | Contraseña de tu red WiFi |
| `STATE_URL` | `https://led-esp32-backend.onrender.com/api/state` |
| `LED_PIN` | GPIO 5 |
| `LED_COUNT` | 8 |
| `POLL_MS` | 500 ms |

> 📋 **[Pega aquí el contenido completo del archivo `esp32-led-fisico.ino`]**

> 📸 **[Agrega aquí una fotografía del circuito físico armado]**

---

### Etapa 6. Diagrama de Flujo del Sistema

El sistema opera bajo una arquitectura cliente-servidor distribuida en tres capas:

```
[Interfaz Web] ──POST/GET──► [Backend Flask en Render] ◄──GET── [ESP32 + NeoPixel]
(GitHub Pages)                  (API REST / state.json)            (polling cada 500ms)
```

**Flujo de una actualización:**
1. El usuario cambia el color o cantidad en la interfaz web.
2. El frontend hace un `POST /api/state` con los nuevos valores.
3. Flask guarda el nuevo estado en `state.json` e incrementa `rev`.
4. El ESP32 detecta el cambio de `rev` en su siguiente consulta.
5. La tira NeoPixel actualiza su color y cantidad de LEDs encendidos.

> 📸 **[Agrega aquí un diagrama de flujo o arquitectura del sistema]**

---

## 5. Resultados y Evidencia

### Análisis de Operación

Durante las pruebas se evaluó la latencia del sistema en distintas condiciones de red:

| Condición | Latencia aproximada |
|---|---|
| Red local con Render activo | ~200–500 ms |
| Primera petición (Render dormido) | ~30–50 segundos (spin-down del plan Free) |
| Red móvil (4G) | ~500–800 ms |

El principal factor de latencia en el plan gratuito de Render es el **spin-down por inactividad**: si no hay peticiones durante varios minutos, el servidor se "duerme" y la primera petición tarda hasta 50 segundos en responder. Una vez activo, la latencia regresa a valores normales.

> 📸 **[Agrega aquí una captura del monitor serial de Arduino mostrando los logs de polling]**

> 📸 **[Agrega aquí un video o GIF del sistema funcionando — LEDs cambiando desde la web]**

### URLs del Proyecto Desplegado

| Recurso | URL |
|---|---|
| Frontend (GitHub Pages) | `https://marthavlds1.github.io/led-esp32-fronted/` |
| Backend (Render) | `https://led-esp32-backend.onrender.com` |
| API State | `https://led-esp32-backend.onrender.com/api/state` |

---

## 6. Análisis y Discusión

**Acceso Global vs. Red Local**
La principal diferencia con prácticas anteriores es la eliminación de la restricción de red local. Al desplegar el backend en Render, cualquier dispositivo con acceso a internet puede controlar los LEDs, independientemente de la red a la que esté conectado el ESP32 o el usuario.

**Separación de Repositorios**
Dividir el proyecto en dos repositorios (backend y frontend) sigue el principio de separación de responsabilidades. Esto permite actualizar la interfaz visual sin afectar el servidor, y viceversa. Render detecta automáticamente cambios en el repo del backend y redeploy sin intervención manual.

**Limitaciones del Plan Gratuito de Render**
El plan Free de Render suspende el servicio tras períodos de inactividad, lo que genera una latencia inicial elevada. Para proyectos en producción, se recomienda usar un plan pagado o implementar un servicio de ping periódico (como UptimeRobot) para mantener el servidor activo.

**Persistencia con Archivo JSON**
El estado se guarda en un archivo `state.json` en el sistema de archivos de Render. Dado que el plan gratuito no garantiza disco persistente entre deploys, los datos pueden perderse al hacer un nuevo despliegue. Para mayor robustez, se recomienda migrar a una base de datos como SQLite o PostgreSQL.

**Polling vs. WebSockets**
El ESP32 utiliza polling (consulta periódica) en lugar de WebSockets. Esto simplifica el código pero introduce una latencia máxima igual al intervalo de polling (500 ms). Una mejora futura sería implementar Server-Sent Events o WebSockets para que el ESP32 reciba notificaciones en tiempo real sin necesidad de consultar constantemente.

---

## 7. Conclusiones

> ✏️ **[Escribe aquí tus conclusiones personales sobre la práctica]**

Sugerencia de puntos a cubrir:
- ¿Qué aprendiste sobre el despliegue de APIs en la nube?
- ¿Qué diferencias notaste entre controlar el hardware localmente vs. desde internet?
- ¿Qué mejorarías del sistema si tuvieras más tiempo?
