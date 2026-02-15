import sys
import serial
import serial.tools.list_ports
import threading

from PyQt6.QtWidgets import (
    QApplication, QWidget, QPushButton, QLabel,
    QVBoxLayout, QHBoxLayout
)
from PyQt6.QtCore import Qt, pyqtSignal, QObject


# ===============================
# Señales para comunicación hilo
# ===============================

class SerialSignals(QObject):
    led_on = pyqtSignal()
    led_off = pyqtSignal()
    disconnected = pyqtSignal()

signals = SerialSignals()


# ===============================
# Ventana principal
# ===============================

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Control UART - ESP32 S3")
        self.setFixedSize(350, 300)

        self.serial_port = None
        self.serial_thread_running = False

        # ===== Widgets =====

        # 1️⃣ Indicador LED virtual
        self.led_indicator = QLabel()
        self.led_indicator.setFixedSize(100, 100)
        self.led_indicator.setStyleSheet(
            "background-color: gray; border-radius: 50px;"
        )

        # 2️⃣ Label estado
        self.status_label = QLabel("Estado: Desconectado")
        self.status_label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        # 3️⃣ Botón Encender
        self.btn_on = QPushButton("Encender LED")
        self.btn_on.clicked.connect(self.send_on)

        # 4️⃣ Botón Apagar
        self.btn_off = QPushButton("Apagar LED")
        self.btn_off.clicked.connect(self.send_off)

        # 5️⃣ Botón Conectar
        self.btn_connect = QPushButton("Conectar")
        self.btn_connect.clicked.connect(self.toggle_connection)

        # ===== Layout =====
        layout = QVBoxLayout()

        layout.addWidget(self.led_indicator, alignment=Qt.AlignmentFlag.AlignCenter)
        layout.addWidget(self.status_label)

        buttons_layout = QHBoxLayout()
        buttons_layout.addWidget(self.btn_on)
        buttons_layout.addWidget(self.btn_off)

        layout.addLayout(buttons_layout)
        layout.addWidget(self.btn_connect)

        self.setLayout(layout)

        # ===== Señales =====
        signals.led_on.connect(self.update_led_on)
        signals.led_off.connect(self.update_led_off)
        signals.disconnected.connect(self.handle_disconnect)

    # ===============================
    # Conectar / Desconectar UART
    # ===============================

    def toggle_connection(self):
        if self.serial_port is None:
            try:
                PORT = "/dev/cu.usbmodem14101"  # <<< CAMBIA ESTO
                self.serial_port = serial.Serial(
                    PORT,
                    115200,
                    timeout=1,
                    dsrdtr=False,
                    rtscts=False
                )

                self.serial_port.setDTR(False)
                self.serial_port.setRTS(False)

                self.serial_thread_running = True
                threading.Thread(
                    target=self.read_serial,
                    daemon=True
                ).start()

                self.status_label.setText("Estado: Conectado")
                self.btn_connect.setText("Desconectar")

            except Exception as e:
                self.status_label.setText("Error al conectar")

        else:
            self.serial_thread_running = False
            self.serial_port.close()
            self.serial_port = None
            self.status_label.setText("Estado: Desconectado")
            self.btn_connect.setText("Conectar")

    # ===============================
    # Leer datos UART
    # ===============================

    def read_serial(self):
        while self.serial_thread_running:
            try:
                if self.serial_port.in_waiting:
                    line = self.serial_port.readline().decode().strip()
                    if line == "LED_ON":
                        signals.led_on.emit()
                    elif line == "LED_OFF":
                        signals.led_off.emit()
            except:
                signals.disconnected.emit()
                break

    # ===============================
    # Enviar comandos
    # ===============================

    def send_on(self):
        if self.serial_port:
            try:
                self.serial_port.write(b"ON\n")
            except:
                signals.disconnected.emit()

    def send_off(self):
        if self.serial_port:
            try:
                self.serial_port.write(b"OFF\n")
            except:
                signals.disconnected.emit()

    # ===============================
    # Actualizar interfaz
    # ===============================

    def update_led_on(self):
        self.led_indicator.setStyleSheet(
            "background-color: green; border-radius: 50px;"
        )
        self.status_label.setText("Estado: LED ENCENDIDO")

    def update_led_off(self):
        self.led_indicator.setStyleSheet(
            "background-color: red; border-radius: 50px;"
        )
        self.status_label.setText("Estado: LED APAGADO")

    def handle_disconnect(self):
        self.serial_thread_running = False
        if self.serial_port:
            self.serial_port.close()
        self.serial_port = None
        self.status_label.setText("Estado: Desconectado")
        self.btn_connect.setText("Conectar")


# ===============================
# MAIN
# ===============================

app = QApplication(sys.argv)
window = MainWindow()
window.show()
sys.exit(app.exec())
