/*
  BRAZO ROBÓTICO DE 4 SERVOS + MEDIDOR DE BATERÍA
  - Cada servo se controla con un potenciómetro.
  - La batería (2S, 8.4 V máx.) se mide con un divisor de tensión
    y se indica con 3 LEDs: verde, amarillo y rojo.
*/

#include <Servo.h>

// ---------- SERVOS Y POTENCIÓMETROS ----------
const int NUM = 4;                                  // Cantidad de servos
Servo servos[NUM];

const int pinServo[NUM] = {2, 3, 4, 5};             // Pines de señal de los servos
const int pinPot[NUM]   = {A0, A1, A2, A3};         // Pines de los potenciómetros

const int pulsoMin[NUM] = {544, 544, 544, 544};     // Pulso mínimo (µs) de cada servo
const int pulsoMax[NUM] = {2200, 2400, 2400, 2400}; // Pulso máximo (µs) de cada servo

const int angMin[NUM] = {15, 30, 30, 5};            // Ángulo mínimo permitido (límite mecánico)
const int angMax[NUM] = {170, 120, 90, 90};         // Ángulo máximo permitido

// Suavizado: valores bajos = movimiento más suave pero más lento
const float factorSuavizado[NUM] = {0.20, 0.20, 0.40, 0.45};

float suave[NUM];        // Lectura filtrada de cada potenciómetro
int ultimoAngulo[NUM];   // Último ángulo enviado al servo

// ---------- LEDs INDICADORES ----------
const int LED_VERDE = 8, LED_AMARILLO = 7, LED_ROJO = 6;
const int ROJO = 0, AMARILLO = 1, VERDE = 2;        // Estados posibles
int estadoLED = -1;                                 // -1 = aún sin definir

// ---------- MEDICIÓN DE BATERÍA ----------
const int PIN_BAT = A7;                             // Entrada del divisor de tensión
const float R1 = 4700.0, R2 = 2400.0;               // Resistencias del divisor (ohmios)
const float VREF = 5.09;                            // Voltaje real de referencia del Arduino
const float FACTOR_CAL = 1.006;                     // Ajuste fino comparando con un multímetro

float voltajeFiltrado = 0.0;
bool bateriaInicializada = false;

const unsigned long INTERVALO_MEDICION = 3000;      // Medir la batería cada 3 segundos
unsigned long ultimaMedicion = 0;

// Enciende solo el LED que corresponde al estado
void mostrarLED(int estado) {
  digitalWrite(LED_ROJO,     estado == ROJO);
  digitalWrite(LED_AMARILLO, estado == AMARILLO);
  digitalWrite(LED_VERDE,    estado == VERDE);
}

// Lee el voltaje real de la batería (promedio de 64 lecturas para reducir ruido)
float leerVoltajeBateria() {
  unsigned long suma = 0;
  for (int i = 0; i < 64; i++) {
    suma += analogRead(PIN_BAT);
    delay(2);
  }
  float voltajeA7 = (suma / 64.0) * VREF / 1023.0;    // Voltaje en el pin
  return voltajeA7 * (R1 + R2) / R2 * FACTOR_CAL;     // Se deshace el divisor de tensión
}

// Convierte voltaje a porcentaje (aproximación por tramos)
int calcularPorcentaje(float v) {
  if (v >= 8.40) return 100;
  if (v >= 8.20) return 90 + (int)((v - 8.20) * 50);
  if (v >= 8.00) return 80 + (int)((v - 8.00) * 50);
  if (v >= 7.80) return 60 + (int)((v - 7.80) * 100);
  if (v >= 7.60) return 40 + (int)((v - 7.60) * 100);
  if (v >= 7.40) return 20 + (int)((v - 7.40) * 100);
  if (v >= 7.00) return 5 + (int)((v - 7.00) * 37.5);
  if (v >= 6.60) return (int)((v - 6.60) * 12.5);
  return 0;
}

// Estado según el porcentaje, sin histéresis (se usa solo en la primera medición)
int estadoNormal(int p) {
  if (p <= 20) return ROJO;
  if (p <= 60) return AMARILLO;
  return VERDE;
}

// Histéresis: los umbrales para subir y bajar son distintos,
// así el LED no parpadea cuando el porcentaje oscila cerca de un límite.
int estadoConHisteresis(int p) {
  if (estadoLED == -1) return estadoNormal(p);

  if (estadoLED == VERDE)
    return (p <= 57) ? AMARILLO : VERDE;

  if (estadoLED == AMARILLO) {
    if (p <= 17) return ROJO;
    if (p >= 64) return VERDE;
    return AMARILLO;
  }

  // estadoLED == ROJO
  if (p >= 64) return VERDE;
  if (p >= 24) return AMARILLO;
  return ROJO;
}

// Mide la batería, filtra el valor, actualiza los LEDs y muestra datos por serial
void actualizarBateria() {
  float voltajeNuevo = leerVoltajeBateria();

  // Filtro: 80% valor anterior + 20% valor nuevo (evita saltos bruscos)
  if (!bateriaInicializada) {
    voltajeFiltrado = voltajeNuevo;
    bateriaInicializada = true;
  } else {
    voltajeFiltrado = voltajeFiltrado * 0.80 + voltajeNuevo * 0.20;
  }

  int porcentaje = calcularPorcentaje(voltajeFiltrado);
  int nuevoEstado = estadoConHisteresis(porcentaje);

  if (nuevoEstado != estadoLED) {   // Solo cambia los LEDs si el estado cambió
    estadoLED = nuevoEstado;
    mostrarLED(estadoLED);
  }

  Serial.println("\n------------------------------");
  Serial.print("Voltaje bateria: ");
  Serial.print(voltajeFiltrado, 2);
  Serial.println(" V");
  Serial.print("Porcentaje: ");
  Serial.print(porcentaje);
  Serial.println(" %");
  Serial.print("LED: ");
  if (estadoLED == ROJO)          Serial.println("ROJO");
  else if (estadoLED == AMARILLO) Serial.println("AMARILLO");
  else                            Serial.println("VERDE");
  Serial.println("------------------------------");
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  mostrarLED(-1);                   // Todos los LEDs apagados al inicio

  for (int i = 0; i < NUM; i++) {
    // Se lee primero la posición del potenciómetro para que el servo
    // arranque donde está la perilla y no dé un salto brusco
    suave[i] = analogRead(pinPot[i]);
    ultimoAngulo[i] = map((int)suave[i], 0, 1023, angMin[i], angMax[i]);

    servos[i].attach(pinServo[i], pulsoMin[i], pulsoMax[i]);
    servos[i].write(ultimoAngulo[i]);
    delay(300);                     // Pausa para no activar todos los servos a la vez
  }

  Serial.println("\n==============================");
  Serial.println("BRAZO ROBOTICO");
  Serial.println("4 SERVOS + BATERIA");
  Serial.println("==============================");

  actualizarBateria();              // Primera medición de batería
  ultimaMedicion = millis();
}

void loop() {
  // --- Control de los servos ---
  for (int i = 0; i < NUM; i++) {
    int lectura = analogRead(pinPot[i]);

    // Filtro suavizador de la lectura del potenciómetro
    suave[i] = suave[i] * (1.0 - factorSuavizado[i]) + lectura * factorSuavizado[i];

    // Convierte 0-1023 al rango de ángulos permitido para ese servo
    int angulo = map((int)suave[i], 0, 1023, angMin[i], angMax[i]);

    // Solo mueve el servo si el ángulo cambió (evita temblores)
    if (abs(angulo - ultimoAngulo[i]) >= 1) {
      servos[i].write(angulo);
      ultimoAngulo[i] = angulo;
    }
  }

  // --- Medición de batería cada INTERVALO_MEDICION (sin bloquear los servos) ---
  if (millis() - ultimaMedicion >= INTERVALO_MEDICION) {
    actualizarBateria();
    ultimaMedicion = millis();
  }

  delay(10);
}
