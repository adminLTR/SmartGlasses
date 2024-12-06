#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

static const unsigned char PROGMEM heartBitmap[] = {
    B00000000, B00000000,
    B00111111, B11000000,
    B00111111, B11000000,
    B01100000, B11100000,
    B01111111, B11100000,
    B11111111, B11110000,
    B11111111, B11110000,
    B11111111, B11110000,
    B11111111, B11110000,
    B11100000, B01110000,
    B01000000, B00100000,
    B00000000, B00000000
};

namespace Screen {
  void printHelloWorld(Adafruit_SH1106G*screen) {
    Serial.begin(9600);

    //display.setContrast (0); // dim display

    // Start OLED
    display.begin(0, true); // we dont use the i2c address but we will reset!

    // Clear the buffer
  display.clearDisplay();

  // Coordenadas para centrar el corazón en la pantalla (128x64):
  int x = 0; //(display.width() - 0 ) / 2;
  int y = 0; //(display.height() - 16) / 2; // 24

  display.drawBitmap(x, y, heartBitmap, 12, 12, 1);

  String plateNumber = "ABC-123";  // Placa almacenada en la variable
  // text display tests
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(15, 2);
  display.println("Placa: "+plateNumber);

  display.display(); // pinta todas las primitivas anteriores desde que limpiaste con clearDisplay


  }
};