#include "Adafruit_VL53L0X.h" // Librería para el sensor VL53L0X
#include <Servo.h> // Librería para controlar el servo motor

Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // Crear un objeto para el sensor VL53L0X
Servo servo; // Crear un objeto para el servo motor

float ema_distance = 0;  // Valor inicial del filtro EMA (Exponential Moving Average)
const float alpha = 0.2; // Factor de suavizado (0 < alpha <= 1). Un valor más bajo suaviza más.
const int servoPin = 9; // Pin donde está conectado el servo motor
const int angleStep = 5; // Paso del ángulo en grados para el escaneo

void setup() {
  Serial.begin(115200); // Iniciar la comunicación serial a 115200 baudios
  while (!Serial) {
    delay(1); // Esperar a que se inicialice el puerto serial (solo para placas con USB nativo)
  }
  Serial.println("Adafruit VL53L0X Geometric Reconstruction"); // Mensaje de inicio
  
  if (!lox.begin()) { // Inicializar el sensor VL53L0X
    Serial.println(F("Failed to boot VL53L0X")); // Mensaje de error si no se inicializa
    while (1); // Detener el programa si el sensor no se inicializa
  }
  
  servo.attach(servoPin); // Conectar el servo motor al pin definido
  Serial.println(F("Angle, Distance(mm), Time(ms)")); // Encabezado para los datos
}

void loop() {
  // Escaneo desde 0° hasta 180° en pasos de "angleStep" grados
  for (int angle = 0; angle <= 180; angle += angleStep) {
    servo.write(angle); // Mover el servo al ángulo actual
    delay(200); // Esperar 200 ms para que el servo se estabilice
    
    VL53L0X_RangingMeasurementData_t measure; // Estructura para almacenar la medición
    lox.rangingTest(&measure, false); // Realizar una medición de distancia

    if (measure.RangeStatus != 4) { // Si la medición es válida (4 indica error)
      float raw_distance = measure.RangeMilliMeter; // Obtener la distancia en milímetros
      
      // Aplicar el filtro EMA (Exponential Moving Average)
      ema_distance = (alpha * raw_distance) + ((1 - alpha) * ema_distance);
      
      // Imprimir los datos en el formato: Ángulo, Distancia, Tiempo
      Serial.print(angle);
      Serial.print(", ");
      Serial.print(ema_distance);
      Serial.print(", ");
      Serial.println(millis()); // Tiempo actual en milisegundos
    } else {
      // Si la medición está fuera de rango, imprimir el ángulo y un mensaje
      Serial.print(angle);
      Serial.println(", out of range");
    }
  }
  
  delay(1000); // Esperar 1 segundo antes de repetir el escaneo
}