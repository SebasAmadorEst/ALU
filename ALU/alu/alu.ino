#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define SERIAL_BAUD 115200
#define BUFFER_SIZE 32

// Inicializamos la pantalla en la dirección 0x27 (16 columnas, 2 filas)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Variables globales para la ALU y el LCD
int A = 0;
int B = 0;
int op = 0;
int resultado = 0;
int carry = 0;

char inputBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// Nombres cortos optimizados para que quepan milimétricamente en el LCD físico
const char* op_names[] = {
  "ADD", "SUB", "INC", "DEC", "DIV"
};

void actualizarPantalla();

void setup() {
  // 1. INICIAR PUERTO SERIAL
  Serial.begin(SERIAL_BAUD);
  
  // 2. INICIAR I2C: Pines por defecto de la ESP32 (SDA=21, SCL=22)
  Wire.begin(21, 22);
  
  // 3. CONFIGURAR PANTALLA LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Mensaje inicial de espera
  lcd.setCursor(0, 0);
  lcd.print("  ALU - ESP32   ");
  lcd.setCursor(0, 1);
  lcd.print("Esperando PC... ");
}

void loop() {
  // Leemos el puerto serial sin bloquear el flujo principal
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      inputBuffer[bufferIndex] = '\0'; // Cerramos el string de la trama
      procesarTrama(inputBuffer);
      bufferIndex = 0; // Reiniciamos el índice
    } 
    else if (bufferIndex < BUFFER_SIZE - 1) {
      inputBuffer[bufferIndex++] = c;
    }
  }
}

void procesarTrama(char* trama) {
  // Parseamos los tres valores enviados desde el HTML ("A,B,op")
  if (sscanf(trama, "%d,%d,%d", &A, &B, &op) == 3) {
    
    // Forzamos entradas estrictas de 4 bits (0 a 15) por seguridad hardware
    A &= 0x0F;
    B &= 0x0F;

    int resultadoCrudo = 0;
    carry = 0; // Reiniciamos el carry global

    // Procesamos las 16 operaciones del selector HTML
    switch(op) {
      case 0: // Suma
        resultadoCrudo = A + B;
        carry = (resultadoCrudo > 15) ? 1 : 0; 
        break;
        
      case 1: // Resta
        resultadoCrudo = A - B;
        if (resultadoCrudo < 0) {
          resultadoCrudo += 16;
          carry = 1;
        }
        break;
        
      case 2: // Incrementar A
        resultadoCrudo = A + 1; 
        break; 
        
      case 3: // Decrementar A
        resultadoCrudo = A - 1; 
        break;
        
      case 4: // Dividir
        resultadoCrudo = (B != 0) ? (A / B) : 0; 
        break;

      default: resultadoCrudo = 0; break;
    }

    // Máscara final para asegurar salida estricta de 4 bits
    resultado = resultadoCrudo & 0x0F;

    // Actualizamos el display físico con los nuevos datos calculados
    if (op >= 0 && op <= 4) {
      actualizarPantalla();
    }

    // Enviamos la respuesta de vuelta al navegador
    int zero = (resultado == 0) ? 1 : 0;
    Serial.printf("Res: %d, C: %d, Z: %d\n", resultado, carry, zero);
  }
}

void actualizarPantalla() {
  lcd.clear();
  
  // Fila 0: "A:XX B:XX  NAME" (Máximo 16 caracteres distribuidos fijos)
  lcd.setCursor(0, 0);
  lcd.print("A:");
  lcd.print(A);
  
  lcd.setCursor(5, 0);
  lcd.print("B:");
  lcd.print(B);
  
  lcd.setCursor(11, 0);
  lcd.print(op_names[op]);
  
  // Fila 1: "Res: XX    C: X"
  lcd.setCursor(0, 1);
  lcd.print("Res: ");
  lcd.print(resultado);
  
  lcd.setCursor(11, 1);
  lcd.print("C: ");
  lcd.print(carry);
}