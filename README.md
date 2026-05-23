## 🎛️ ALU de 4 bits con Control por Interfaz Web (ESP32)

Este proyecto implementa una Unidad Aritmético Lógica (ALU) de 4 bits utilizando un microcontrolador ESP32. El sistema permite realizar operaciones matemáticas básicas controladas desde una interfaz web moderna que se comunica mediante la Web Serial API, visualizando los resultados tanto en la web como en una pantalla LCD 16x2 I2C física.

## 🚀 Características
* Procesamiento de 4 Bits: Las entradas A y B están restringidas mediante software a un rango de 0 a 15 (4 bits) para asegurar la integridad de la operación.
* Operaciones Soportadas: Incluye Suma (ADD), Resta (SUB), Incremento (INC), Decremento (DEC) y División (DIV).
* Interfaz Web Serial: Control total mediante interruptores binarios (switches) y selectores desde el navegador.
* Detección de Flags: Cálculo y visualización en tiempo real de banderas de **Acarreo (Carry) y Cero (Zero).
* Visualización Dual: Monitor simulado en la interfaz HTML y salida física en tiempo real en el LCD.

## 📂 Estructura del Repositorio
El repositorio está organizado de la siguiente manera:
```text
ALU/
├── alu/
│   └── alu.ino         # Código fuente para la ESP32 (Arduino IDE)
└── interfaz_alu.html   # Panel de control web (HTML/CSS/JS)
```

## 🛠️ Requisitos de Hardware

* **Placa:** ESP32 (DevKit V1 o similar).
* **Pantalla:** LCD 16x2 con adaptador I2C (PCF8574).
* **Cables:** Jumpers Dupont (Macho-Hembra).
* **Conexión:** Cable Micro-USB para datos y alimentación.

### 🔌 Conexiones (Pinout)

| LCD I2C | ESP32 GPIO | Descripción |
| --- | --- | --- |
| **GND** | GND | Tierra |
| **VCC** | VIN (5V) | Alimentación (Se recomienda 5V para el contraste del LCD) |
| **SDA** | GPIO 21 | Datos de la comunicación I2C |
| **SCL** | GPIO 22 | Reloj de la comunicación I2C |

## 💻 Configuración de Software

### 1. ESP32 (Firmware)

1. Abre `ALU/alu/alu.ino` en el Arduino IDE.
2. Instala la librería **LiquidCrystal I2C** (por Frank de Brabander) desde el Gestor de Librerías.
3. Selecciona tu placa ESP32 y el puerto COM correcto.
4. Carga el código al microcontrolador. *Nota: La dirección I2C por defecto es `0x27`.*

### 2. Interfaz Web

1. No requiere servidor externo. Simplemente abre el archivo `ALU/interfaz_alu.html` en un navegador compatible (Google Chrome o Microsoft Edge).
2. Haz clic en el botón **"Conectar ESP32"**.
3. Selecciona el puerto serial asignado a tu placa.
4. **⚠️ NOTA CRÍTICA DE CONEXIÓN:** La Web Serial API solo puede acceder al puerto si este se encuentra libre. **No podrás conectar la interfaz web si tienes abierto el Monitor Serial del IDE de Arduino** (o cualquier otra aplicación que esté usando el puerto COM), ya que el recurso estará bloqueado. Asegúrate de cerrar el monitor del IDE antes de intentar la conexión desde el navegador.

## ⚙️ Cómo funciona

1. **Entrada:** El usuario manipula los switches binarios en la interfaz web para definir los valores de A y B en formato decimal y binario.
2. **Transmisión:** La aplicación web envía un **mensaje** o **comando** de texto con el formato `A,B,op\n` (por ejemplo: `5,3,0\n` para realizar una suma).
3. **Procesamiento:** La ESP32 recibe este **mensaje**, separa los números y ejecuta la operación correspondiente (Suma, Resta, etc.) en su bloque ALU lógico.
4. **Actualización:** El microcontrolador actualiza inmediatamente la pantalla LCD física con los nuevos valores calculados.
5. **Feedback:** La ESP32 devuelve los resultados calculados (Resultado, Carry, Zero) al monitor de la interfaz web mediante el puerto serial para confirmar la operación.

## 📝 Notas de Uso

* **Contraste:** Si el LCD enciende la luz de fondo pero no muestra caracteres, ajusta el potenciómetro azul ubicado en la parte trasera del módulo I2C.
* **Seguridad de Hardware:** El firmware aplica automáticamente una máscara `& 0x0F` a las entradas para garantizar que el sistema siempre opere dentro del rango estricto de 4 bits, evitando desbordamientos no controlados.

---

**Proyecto académico desarrollado para la Universidad Tecnológica de Pereira (UTP).**
