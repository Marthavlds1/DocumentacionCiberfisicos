# Comunicación Industrial Modbus TCP — PLC Allen-Bradley Micro850

Implementación de comunicación Modbus TCP entre dos PLC Allen-Bradley Micro850 y una interfaz gráfica en Python, con programación en lenguaje escalera mediante Connected Components Workbench (CCW).

---

## Tabla de Contenidos

- [Descripción General](#descripción-general)
- [Hardware y Software Requerido](#hardware-y-software-requerido)
- [Arquitectura del Sistema](#arquitectura-del-sistema)
- [Configuración de Red](#configuración-de-red)
- [Configuración del PLC en CCW](#configuración-del-plc-en-ccw)
- [Modbus Mapping](#modbus-mapping)
- [Programación Ladder](#programación-ladder)
- [Interfaz Web con Flask](#interfaz-web-con-flask)
- [Interfaz Gráfica con PySide6](#interfaz-gráfica-con-pyside6)
- [Pruebas de Comunicación](#pruebas-de-comunicación)
- [Resultados](#resultados)
- [Conclusiones](#conclusiones)

---

## Descripción General

El sistema integra tres componentes principales:

1. **PLC 1 (ProjectMod1)** — IP `192.168.3.152` — Servidor Modbus TCP, programado en Ladder para leer y escribir registros con el PLC 2.
2. **PLC 2** — IP `192.168.3.156` — Servidor Modbus TCP con lógica complementaria al PLC 1.
3. **PC con Python** — Cliente Modbus TCP doble. Incluye una interfaz web en Flask para control básico de salidas y una interfaz gráfica con PySide6 para control multi-PLC con LEDs virtuales y lectura de botones físicos.

La comunicación PLC–PLC y PC–PLC fue validada correctamente, demostrando intercambio estable de datos en tiempo real.

---

## Hardware y Software Requerido

### Hardware

| Elemento | Descripción |
|----------|-------------|
| PLC Allen-Bradley Micro850 | Modelo `2080-LC50-48QWB` (×2) |
| Switch Ethernet | Linksys WRT300N |
| Cable Ethernet | Cat5e o superior |
| Cable USB Tipo B | Programación inicial del PLC |
| Laptop / PC | Cliente Modbus y entorno de desarrollo |
| Eliminador (fuente) | Para el switch Linksys |

### Software

| Herramienta | Versión | Propósito |
|-------------|---------|-----------|
| Connected Components Workbench (CCW) | v22 | Programación del PLC |
| Python | 3.12+ | Interfaces y cliente Modbus |
| Visual Studio Code | — | IDE |
| Qt Designer (incluido en PySide6) | — | Diseño de interfaz gráfica |
| Flask | — | Interfaz web |
| pymodbus | 3.9.x | Comunicación Modbus TCP |
| PySide6 | 6.x | Interfaz gráfica de escritorio |

> El framework **.NET 3.5** de Microsoft es requisito para instalar CCW. Verificar en `Panel de Control → Programas → Activar o desactivar características de Windows`.

### Instalación de librerías Python

```bash
pip install flask pymodbus PySide6
```

Verificar instalación:

```bash
pip list
# Debe aparecer: flask, pymodbus, PySide6
```

---

## Arquitectura del Sistema

```
                      Red 192.168.3.0/24

        ┌──────────────┐   MSG_MODBUS2   ┌──────────────┐
        │    PLC 1     │◄───────────────►│    PLC 2     │
        │ 192.168.3.152│  holding regs   │ 192.168.3.156│
        └──────┬───────┘                 └──────┬───────┘
               │                                │
               └───────────────┬────────────────┘
                               │ Ethernet
                        ┌──────▼──────┐
                        │   Switch    │
                        │   Linksys   │
                        └──────┬──────┘
                               │
                    ┌──────────▼──────────┐
                    │    PC / Laptop      │
                    │  Cliente Modbus TCP │
                    │                     │
                    │  ┌───────────────┐  │
                    │  │ Flask :5000   │  │
                    │  └───────────────┘  │
                    │  ┌───────────────┐  │
                    │  │ PySide6 GUI   │  │
                    │  └───────────────┘  │
                    └─────────────────────┘
```

### Flujos de comunicación

| Dirección | Mecanismo | Datos |
|-----------|-----------|-------|
| PLC 1 → PLC 2 | MSG_MODBUS2 (Cmd=3) | Arreglo `entradas_plc[]` |
| PLC 2 → PLC 1 | MSG_MODBUS2 (Cmd=3) | Arreglo `entradas_plc[]` |
| PC → PLC | `write_coil()` | Marcas booleanas |
| PLC → PC | `read_coils()` | Estado de botones físicos |
| PC → PLC | `write_register()` | Registro de control (Flask) |

---

## Configuración de Red

### 1. Asignar IP al Switch

1. Conectar el cable Ethernet del switch a la PC.
2. Abrir el navegador y acceder a `192.168.1.1` (IP de fábrica Linksys).
   - Usuario: `admin` / Contraseña: `admin`
3. En **Network Setup**, cambiar la IP del router a `192.168.3.200`.
4. Guardar y cerrar.

> Si el switch no responde en `192.168.1.1` y se desconoce la IP asignada, realizar reset de fábrica: mantener presionado el botón Reset (costado del puerto 4 Ethernet) por 5 segundos, desconectar y reconectar.

### 2. Conexión Inalámbrica (opcional)

1. Conectarse a la red Wi-Fi `Linksys`.
2. Ir a `Panel de Control → Redes e Internet → Centro de redes → Wi-Fi (Linksys) → Propiedades → TCP/IPv4`.
3. Configurar IP estática en el mismo segmento, por ejemplo `192.168.3.101`.

### 3. Verificar Conectividad

```bash
ping 192.168.3.152
ping 192.168.3.156
```

Ambos deben responder antes de continuar.

---

## Configuración del PLC en CCW

### Agregar el Controlador

1. Conectar el cable USB Tipo B al PLC y a la PC.
2. Abrir CCW → clic en **Discover**.
3. En **Connection Browser**, expandir `USB` y seleccionar el controlador detectado.
4. Asignar nombre y ubicación al proyecto generado.

> Verificar que la versión de firmware del controlador en CCW coincida con la del PLC físico para evitar errores de compatibilidad.

### Asignar Dirección IP al PLC

1. En la página principal del proyecto ir a `Ethernet → Configure IP address and settings`.
2. Asignar la IP correspondiente dentro del segmento `192.168.3.x`:
   - **PLC 1:** `192.168.3.152`
   - **PLC 2:** `192.168.3.156`
3. Máscara de subred: `255.255.255.0`.
4. Clic en **Download** → seleccionar conexión USB → confirmar descarga de configuración Ethernet.

Una vez asignada la IP se puede desconectar el USB y usar Ethernet para futuras descargas.

### Establecer Conexión Ethernet en CCW

1. Conectar el cable Ethernet del PLC al switch.
2. En CCW, clic en el ícono de lápiz junto a la ruta de conexión → **Browse**.
3. En **Connection Browser**, expandir `AB_Ethip-1, Ethernet`.
4. Seleccionar el PLC con la IP configurada → **OK**.
5. Clic en **Connect** o **Download** según se requiera.

---

## Modbus Mapping

El Modbus Mapping asocia variables internas del PLC con direcciones Modbus accesibles externamente. Se configura en `Página principal del proyecto → Modbus Mapping → Add`.

### PLC 1 — ProjectMod1 (192.168.3.152)

| Variable | Tipo | Dirección CCW | Descripción |
|----------|------|---------------|-------------|
| `entradas_plc` | INT[0..9] | 400001–400010 | Holding registers enviados al PLC 2 |
| `valor_leido2` | INT | 000357 | Dato recibido del PLC 2 convertido a INT |
| `marca_m2` | BOOL | 000374 | Marca lógica activada desde botón físico o interfaz |
| `phmarca_m2` | BOOL | 000375 | Estado del botón físico, legible por Python |

### PLC 2 — ModNuevoNuevo (192.168.3.156)

| Variable | Tipo | Dirección CCW | Descripción |
|----------|------|---------------|-------------|
| `phmarca_m1` | BOOL | 000001 | Estado físico del botón `_IO_EM_DI_00` |
| `marca_m1` | BOOL | 000002 | Marca lógica para activar envío de datos |
| `entradas_plc` | INT[0..9] | 000003–000016 | Holding registers enviados al PLC 1 |
| `valor_leido1` | INT | 000163 | Dato recibido del PLC 1 convertido a INT |

> **Offset importante:** La dirección usada en el código Python es siempre **una unidad menor** que la definida en CCW.
> Ejemplo: dirección CCW `000008` → dirección Python `7`.

### Tipos de registro Modbus

| Prefijo | Tipo | Acceso | Uso |
|---------|------|--------|-----|
| `0xxxx` | Coils | R/W | Marcas booleanas y estados de botones |
| `4xxxx` | Holding Registers | R/W | Arreglos `entradas_plc[]` |

---

## Programación Ladder

Los programas Ladder se crean en CCW: clic derecho en **Programas → Agregar → Nuevo LD: Diagrama en escalera**.

### Bloque MSG_MODBUS2

Para comunicación PLC–PLC se usa el bloque `MSG_MODBUS2` en su variante **TCP** (no RTU, que es comunicación serial).

Parámetros del bloque:

| Parámetro | Subparámetro | Valor | Descripción |
|-----------|-------------|-------|-------------|
| `LocalCfg` | Channel | `4` | Canal TCP |
| `LocalCfg` | TriggerType | `20` | Delay en ms |
| `LocalCfg` | Cmd | `3` | Read Holding Registers |
| `LocalCfg` | ElementCnt | `10` | Registros a leer |
| `TargetCfg` | Addr | `1` | Dirección Modbus destino |
| `TargetCfg` | NodeAddress[0..3] | `192.168.3.XXX` | IP del PLC destino |

### Lógica PLC 2 (192.168.3.156)

```
Rung 1 — Bloque MSG_MODBUS2
  Lee entradas_plc[] del PLC 1 (NodeAddress[3] = 152)

Rung 2 — Envío de valor al PLC 1
  IF marca_m1 OR _IO_EM_DI_00 THEN
    MOV 20 → entradas_plc[1]

Rung 3 — Reset cuando no hay activación
  IF NOT marca_m1 AND NOT _IO_EM_DI_00 THEN
    MOV 0 → entradas_plc[1]

Rung 4 — Evaluación del valor recibido
  ANY_TO_INT(ReadAddress1[8]) → valor_leido1
  IF valor_leido1 == 90 THEN activar salida digital

Rung 5 — Botón físico mapea a coil
  _IO_EM_DI_00 → phmarca_m1
```

### Lógica PLC 1 — ProjectMod1 (192.168.3.152)

```
Rung 1 — Bloque MSG_MODBUS2
  Lee entradas_plc[] del PLC 2 (NodeAddress[3] = 156)

Rung 2 — Envío de valor al PLC 2
  IF marca_m2 OR _IO_EM_DI_03_Bto_Stop THEN
    MOV 20 → entradas_plc[2]

Rung 3 — Reset cuando no hay activación
  IF NOT marca_m2 AND NOT _IO_EM_DI_03 THEN
    MOV 0 → entradas_plc[2]

Rung 4 — Evaluación del valor recibido
  ANY_TO_INT(ReadAddress2[x]) → valor_leido2
  IF valor_leido2 == 30 THEN activar _IO_EM_DO_01

Rung 5 — Botón físico mapea a coil
  _IO_EM_DI_03_Bto_Stop → phmarca_m2
```

> El bloque `ANY_TO_INT` es necesario porque `ReadAddress[]` devuelve tipo `WORD`. Sin esta conversión no es posible hacer comparaciones numéricas en la lógica Ladder.

### Descarga del programa al PLC

1. Verificar conexión Ethernet activa en CCW.
2. Clic en **Download** en la barra superior.
3. Seleccionar el driver Ethernet configurado → **Aceptar**.
4. Elegir **Descargar programa al controlador lógico**.

---

## Interfaz Web con Flask

Interfaz ligera para encender y apagar una salida del PLC desde el navegador. Cualquier dispositivo en la misma red puede acceder usando la IP de la PC en el puerto `5000`.

El PLC debe tener mapeado el holding register correspondiente en Modbus Mapping, y el programa Ladder debe evaluar ese registro para activar la salida deseada.

### Estructura del proyecto

```
interfaz_web/
└── plc_control.py
```

### `plc_control.py`

```python
from flask import Flask, render_template_string, request
from pymodbus.client import ModbusTcpClient

# ── Parámetros de conexión ──────────────────────────────────────
IP_PLC    = "192.168.3.152"
PUERTO    = 502
DIRECCION = 0               # Holding register mapeado en CCW

app = Flask(__name__)

# ── Escritura al PLC ────────────────────────────────────────────
def enviar_valor(valor: int):
    cliente = ModbusTcpClient(IP_PLC, port=PUERTO)
    try:
        if not cliente.connect():
            return False, "Sin conexión con el PLC"
        respuesta = cliente.write_register(DIRECCION, valor)
        if respuesta.isError():
            return False, f"Error al escribir: {respuesta}"
        return True, f"Valor {valor} enviado correctamente"
    except Exception as exc:
        return False, f"Excepción: {exc}"
    finally:
        cliente.close()

# ── Página principal ────────────────────────────────────────────
PLANTILLA = """
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="utf-8">
    <title>Control PLC</title>
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body {
            font-family: 'Segoe UI', sans-serif;
            background: #1a1a2e;
            color: #eee;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
        }
        h1 { font-size: 1.8rem; margin-bottom: 2rem; letter-spacing: 1px; }
        .controles { display: flex; gap: 1.5rem; }
        button {
            padding: 1rem 2.5rem;
            font-size: 1rem;
            font-weight: 600;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            transition: opacity 0.2s;
        }
        button:hover { opacity: 0.85; }
        .encender { background: #27ae60; color: #fff; }
        .apagar   { background: #c0392b; color: #fff; }
        .estado   { margin-top: 2rem; font-size: 0.95rem; color: #aaa; }
    </style>
</head>
<body>
    <h1>Panel de Control — PLC</h1>
    <div class="controles">
        <form action="/encender" method="post">
            <button class="encender" type="submit">Encender</button>
        </form>
        <form action="/apagar" method="post">
            <button class="apagar" type="submit">Apagar</button>
        </form>
    </div>
    <p class="estado">{{ estado }}</p>
</body>
</html>
"""

@app.route("/")
def inicio():
    return render_template_string(PLANTILLA, estado="Listo.")

@app.route("/encender", methods=["POST"])
def encender():
    _, msg = enviar_valor(1)
    return render_template_string(PLANTILLA, estado=msg)

@app.route("/apagar", methods=["POST"])
def apagar():
    _, msg = enviar_valor(0)
    return render_template_string(PLANTILLA, estado=msg)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=False)
```

### Ejecución

```bash
python plc_control.py
# Acceder desde el navegador: http://<IP-de-la-PC>:5000
```

---

## Interfaz Gráfica con PySide6

Interfaz de escritorio que permite controlar salidas físicas de ambos PLCs desde botones virtuales y visualizar en tiempo real el estado de los botones físicos mediante LEDs de colores.

### Estructura del proyecto

```
interfaz_gui/
├── main.py          ← Lógica principal
├── interfaz.ui      ← Diseño creado en Qt Designer
└── interfazui.py    ← Generado a partir del .ui
```

### Diseño en Qt Designer

1. Abrir **Qt Designer** (buscar `Designer` en Windows).
2. Seleccionar **Dialog without Buttons → Create**.
3. Insertar los elementos necesarios:

**Push Buttons** (panel izquierdo → Buttons):
- Arrastrar al área de diseño.
- Doble clic para editar el texto visible (ej. `Mod1`, `Mod2`).
- En Object Inspector, asignar un `objectName` descriptivo: `btn_mod1`, `btn_mod2`.

**Labels como LEDs** (Display Widgets → Label):
- Arrastrar al área de diseño.
- En Property Editor → `styleSheet`: escribir `background-color: white;`
- Borrar el texto de la etiqueta.
- Renombrar: `blueled_mod2`, `redled_mod1`.

4. Guardar como `interfaz.ui` en la carpeta del proyecto.

### Conversión `.ui` → `.py`

```bash
pyside6-uic interfaz.ui -o interfazui.py
```

### `main.py`

```python
import sys
import os
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from PySide6.QtWidgets import QApplication, QDialog
from PySide6.QtCore    import QTimer
from pymodbus.client   import ModbusTcpClient
from interfazui        import Ui_Dialog


class VentanaControl(QDialog):
    def __init__(self):
        super().__init__()
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)

        # ── Conexión a PLCs ──────────────────────────────────────
        self.plc1 = ModbusTcpClient("192.168.3.152")
        self.plc2 = ModbusTcpClient("192.168.3.156")

        ok1 = self.plc1.connect()
        ok2 = self.plc2.connect()

        print("--- INICIANDO SISTEMA ---")
        print(f"[{'OK' if ok1 else 'ERROR'}] PLC 1 (152) {'Conectado' if ok1 else 'Sin conexión'}.")
        print(f"[{'OK' if ok2 else 'ERROR'}] PLC 2 (156) {'Conectado' if ok2 else 'Sin conexión'}.")

        # ── Timer para lectura periódica de botones físicos ──────
        self.monitor = QTimer(self)
        self.monitor.timeout.connect(self.actualizar_leds)
        self.monitor.start(200)

        # ── Botones virtuales → coils en los PLCs ────────────────
        # btn_mod1 activa marca_m2 en PLC 1 (CCW 000374 → Python 373)
        self.ui.btn_mod1.pressed.connect(
            lambda: self._escribir_coil(self.plc1, 373, True, "PLC1", "btn_mod1"))
        self.ui.btn_mod1.released.connect(
            lambda: self._escribir_coil(self.plc1, 373, False, "PLC1", "btn_mod1"))

        # btn_mod2 activa marca_m1 en PLC 2 (CCW 000002 → Python 1)
        self.ui.btn_mod2.pressed.connect(
            lambda: self._escribir_coil(self.plc2, 1, True, "PLC2", "btn_mod2"))
        self.ui.btn_mod2.released.connect(
            lambda: self._escribir_coil(self.plc2, 1, False, "PLC2", "btn_mod2"))

    # ── Escritura genérica en coil ───────────────────────────────
    def _escribir_coil(self, cliente, direccion, valor, nombre_plc, nombre_btn):
        accion = "TRUE" if valor else "FALSE"
        print(f"-> [GUI] {nombre_btn} {'Presionado' if valor else 'Soltado'}: "
              f"escribiendo {accion} en {nombre_plc} coil {direccion}")
        try:
            cliente.write_coil(direccion, valor)
        except Exception as e:
            print(f"   Error al escribir coil: {e}")

    # ── Lectura de botones físicos y actualización de LEDs ───────
    def actualizar_leds(self):
        # phmarca_m2 en PLC 1 (CCW 000375 → Python 374)
        self._leer_y_pintar(self.plc1, 374, self.ui.blueled_mod2, "blue", "PLC 1", 374)
        # phmarca_m1 en PLC 2 (CCW 000001 → Python 0)
        self._leer_y_pintar(self.plc2, 0,   self.ui.redled_mod1,  "red",  "PLC 2", 0)

    def _leer_y_pintar(self, cliente, direccion, widget, color, nombre_plc, entrada_log):
        try:
            resp = cliente.read_coils(direccion, count=1)
            if resp.isError():
                return
            activo = resp.bits[0]
            widget.setStyleSheet(
                f"background-color: {'white' if not activo else color};")
            print(f"<- [{nombre_plc}] Entrada {entrada_log} cambió a: {activo}")
        except Exception:
            pass

    # ── Cierre limpio de conexiones ──────────────────────────────
    def closeEvent(self, event):
        self.plc1.close()
        self.plc2.close()
        event.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    ventana = VentanaControl()
    ventana.show()
    sys.exit(app.exec())
```

### Ejecución

```bash
# Estar conectado a la misma red que los PLCs
python main.py
```

---

## Pruebas de Comunicación

### Prueba digital (sin hardware externo)

Permite verificar el intercambio de datos entre PLCs modificando variables manualmente en CCW.

1. Conectar al PLC 1 en CCW → ir a la tabla de variables globales.
2. Modificar manualmente `entradas_plc[1] = 25`.
3. Conectar al PLC 2 y verificar que `ReadAddress[1] = 25`.

La correspondencia entre arreglos es directa:

```
PLC 1: entradas_plc[1] = 25  →  PLC 2: ReadAddress[1] = 25
PLC 2: entradas_plc[5] = 78  →  PLC 1: ReadAddress[5] = 78
```

Si los datos no llegan, revisar:
- `ReadLocal → Cmd = 3`
- `ReadTarget → Addr = 1`
- `ReadTarget → NodeAddress` = IP correcta del PLC destino
- Que ambos PLCs estén en el mismo segmento de red sin conflictos de IP

### Prueba con hardware físico

1. Presionar botones físicos en cada PLC.
2. Verificar que la salida digital del PLC receptor se activa cuando el valor recibido cumple la condición programada.
3. Verificar que los LEDs virtuales en la interfaz Python cambian de color en tiempo real.

---

## Resultados

### Conexión desde Python

```
--- INICIANDO SISTEMA ---
[OK] PLC 1 (152) Conectado.
[OK] PLC 2 (156) Conectado.
```

### Lectura de botones físicos

```
<- [PLC 1] Entrada 374 cambió a: True
<- [PLC 1] Entrada 374 cambió a: False
<- [PLC 2] Entrada 0 cambió a: True
<- [PLC 2] Entrada 0 cambió a: False
```

### Escritura desde botones virtuales

```
-> [GUI] btn_mod1 Presionado: escribiendo TRUE en PLC1 coil 373
-> [GUI] btn_mod1 Soltado: escribiendo FALSE en PLC1 coil 373
-> [GUI] btn_mod2 Presionado: escribiendo TRUE en PLC2 coil 1
-> [GUI] btn_mod2 Soltado: escribiendo FALSE en PLC2 coil 1
```

### Intercambio PLC–PLC

| Condición | Resultado |
|-----------|-----------|
| PLC 2 activa `_IO_EM_DI_00` | PLC 1 recibe `valor_leido2 = 20` |
| PLC 1 activa `_IO_EM_DI_03_Bto_Stop` | PLC 2 recibe `valor_leido1 = 20` |
| `valor_leido2 == 90` en PLC 1 | Se activa `_IO_EM_DO_01` |
| `valor_leido1 == 30` en PLC 2 | Se activa salida configurada |

### Estabilidad

- Sin errores de conexión Modbus durante todas las pruebas.
- Sin bloqueos ni saturación en el cliente Python.
- LEDs virtuales actualizados fluidamente a 200 ms.
- Campos `Error`, `ErrorID` y `SubErrorID` del bloque MSG_MODBUS2 sin códigos de error en ninguna prueba.

---

## Conclusiones

La práctica permitió implementar y validar comunicación industrial **Modbus TCP** entre dos PLC Allen-Bradley Micro850 y dos interfaces en Python. Los puntos clave:

- El **Modbus Mapping** en CCW es el paso fundamental; sin él, ningún dispositivo externo puede acceder a las variables internas del PLC.
- El bloque **MSG_MODBUS2** con `Cmd = 3` es el mecanismo estándar para lectura de holding registers entre controladores Micro850.
- La conversión **`ANY_TO_INT`** es imprescindible en Ladder para operar con los datos recibidos, ya que `ReadAddress[]` devuelve tipo `WORD`.
- El **offset de una unidad** entre las direcciones CCW y las usadas en Python es un detalle crítico: una dirección incorrecta produce lecturas silenciosas de otro registro sin generar error.
- Python con **pymodbus** funciona como cliente Modbus completo: puede leer y escribir tanto coils como holding registers, lo que permite construir desde un panel web básico hasta una interfaz de supervisión más completa.
