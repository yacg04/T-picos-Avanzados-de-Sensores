#include <SoftwareSerial.h>

SoftwareSerial gps(3,2);  // RX en 3, TX en 2

String gpsData = "";
String latitud = "";
String altitud = "";

void setup() {
  Serial.begin(9600);
  gps.begin(9600);

  Serial.println("Latitud,Altitud");  // Encabezado
}

void loop() {
  while (gps.available()) {
    char c = gps.read();
    gpsData += c;

    // Detecta el final de la línea de datos
    if (c == '\n') {
      if (gpsData.startsWith("$GPGGA")) {  // Solo procesa la trama GPGGA
        extraerDatos(gpsData);
      }
      gpsData = "";  // Limpia el buffer para la siguiente lectura
    }
  }
}

// Extrae latitud y altitud de la trama $GPGGA
void extraerDatos(String data) {
  int indices[15];  // Posiciones de las comas
  int index = -1;

  // Encuentra las posiciones de las comas
  for (int i = 0; i < 15; i++) {
    index = data.indexOf(",", index + 1);
    indices[i] = index;
  }

  // Extraer latitud (campo 2 y 3)
  latitud = data.substring(indices[1] + 1, indices[2]) + " " + data.substring(indices[2] + 1, indices[3]);

  // Extraer altitud (campo 5)
  altitud = data.substring(indices[3] + 1, indices[5]);

  // Imprimir resultados
  Serial.print("Latitud: ");
  Serial.print(latitud);
  Serial.print(" | Altitud: ");
  Serial.println(altitud);
}