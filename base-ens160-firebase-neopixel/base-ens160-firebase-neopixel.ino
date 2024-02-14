// MURU 7.8
// Respiro Namiki 2.0
// Nic Motta
// 2024

// Versión ENS160 - Semáforo

// Librerias necesarias
#include <ESP32Firebase.h>      // ESP32 Firebase - Rupak Poddar
#include "Adafruit_CCS811.h"    // Adafruit CCS811 - Adafruit
#include <Adafruit_NeoPixel.h>  // Adafruit Neo Pixel
#include "Wire.h"               // Para cambiar el I2C a pines 12 y 14

// ---- CONFIGURACIÓN -------------------------------------------------------
// --------------------------------------------------------------------------
// Configuración de WiFi
#define SSID "SSID"
#define PASSWORD "PASSWORD"
#define FIREBASE_URL "https://respir0-namiki-default-rtdb.firebaseio.com"
// Cambiar número dispositivo correspondiente
#define DIPOSITIVO "data/dispositivo-1/co2"
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------

// Definimos nuevos I2C
#define NEW_SDA 13
#define NEW_SCL 14
// Configuración pixeles
#define PIN 21
#define NUMPIXELS 11
// Frecuencia de respiro
#define RESPIRO 12

// Inicializa libreria de sensor, Firebase y NeoPixel
Adafruit_CCS811 sensor;
Firebase firebase(FIREBASE_URL);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Wire.begin(NEW_SDA, NEW_SCL);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  pixels.begin();
  pixels.clear();
  delay(1000);

  // Connect to WiFi
  Serial.print("Conectando a: ");
  Serial.println(_SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Conectado");

  // Print the IP address
  Serial.print("IP: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  if (!sensor.begin()) {
    Serial.println("¡Error al encontrar el sensor!");
    while (1);
  }

  // Wait for the sensor to be ready
  while (!sensor.available());
}

void loop() {

  if (sensor.available()) {
    if (!sensor.readData()) {
      int value = sensor.geteCO2();
      Serial.print("CO2: ");
      Serial.println(value);

      firebase.setInt(DIPOSITIVO, value);
      setColor(value);
    }
  }
}

// Función para cambiar de color los NeoPixels según valor (value)
// 400: green -- 1000:limeGreen -- 2000: yellow -- 3000: orange -- 4000: red
void setColor(int value) {

  if (value < 1000) {
    for (int g = 20; g < 255; g++) {
      pixels.fill(pixels.Color(g, 0, 0));
      pixels.show();
      delay(RESPIRO);
    }

    for (int g = 255; g > 20; g--) {
      pixels.fill(pixels.Color(g, 0, 0));
      pixels.show();
      delay(RESPIRO);
    }
  }

  if (value > 1000 & value < 2000) {
    for (int r = 20; r < 255; r++) {
      pixels.fill(pixels.Color(r / 2, r, 0));
      pixels.show();
      delay(RESPIRO);
    }

    for (int r = 255; r > 20; r--) {
      pixels.fill(pixels.Color(r / 2, r, 0));
      pixels.show();
      delay(RESPIRO);
    }
  }

  if (value > 2000 & value < 3000) {
    for (int r = 20; r < 255; r++) {
      pixels.fill(pixels.Color(r, r, 0));
      pixels.show();
      delay(RESPIRO);
    }

    for (int r = 255; r > 20; r--) {
      pixels.fill(pixels.Color(r, r, 0));
      pixels.show();
      delay(RESPIRO);
    }
  }

  if (value > 3000 & value < 4000 ) {
    for (int r = 20; r < 255; r++) {
      pixels.fill(pixels.Color(r / 4, r, 0));
      pixels.show();
      delay(RESPIRO);
    }

    for (int r = 255; r > 20; r--) {
      pixels.fill(pixels.Color(r / 4, r, 0));
      pixels.show();
      delay(RESPIRO);
    }
  }

  if (value > 4000) {
    for (int r = 20; r < 255; r++) {
      pixels.fill(pixels.Color(0, r, 0));
      pixels.show();
      delay(RESPIRO);
    }

    for (int r = 255; r > 20; r--) {
      pixels.fill(pixels.Color(0, r, 0));
      pixels.show();
      delay(RESPIRO);
    }
  }
}

