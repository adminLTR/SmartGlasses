#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{
    B00001100, B00110000, // Fila 0
    B00011110, B01111000, // Fila 1
    B00111111, B01111110, // Fila 2
    B01111111, B11111111, // Fila 3
    B01111111, B11111111, // Fila 4
    B00111111, B11111110, // Fila 5
    B00011111, B11111100, // Fila 6
    B00001111, B11111000, // Fila 7
    B00000111, B11110000, // Fila 8
    B00000011, B11100000, // Fila 9
    B00000001, B11000000, // Fila 10
    B00000000, B10000000, // Fila 11
    B00000000, B00000000, // Fila 12
    B00000000, B00000000, // Fila 13
    B00000000, B00000000, // Fila 14
    B00000000, B00000000  // Fila 15
};


namespace Screen {
  void printHelloWorld(Adafruit_SH1106G*screen) {
    Serial.begin(9600);

    //display.setContrast (0); // dim display

    // Start OLED
    display.begin(0, true); // we dont use the i2c address but we will reset!

    display.clearDisplay(); // limpia pantalla

    /*
    display->drawPixel(10, 10, SH110X_WHITE); // pinta en x=10, y=10 un pixel de color WHITE

    display->setTextSize(1); // ratio
    display->setTextColor(SH110X_WHITE); // color de texto
    display->setCursor(0, 0); // pones el cursor en donde empieza el texto
    display->println("Failure is always an option"); // imprimes imbecil
    // eres una basura imbecil
    */

    // Dibuja el corazón de 64x64 en el centro de una pantalla de 128x64
    // display.drawRGBBitmap(32, 0, heartBitmap, 64, 64);

    // Coordenadas para centrar el corazón en la pantalla (128x64):
    int x = display.height() / 2;  // 56
    int y = (display.width() - 16) / 2; // 24

    display.drawBitmap(y, x, logo16_glcd_bmp, 16, 16, SH110X_WHITE);

    //display->invertDisplay(true); // invierte (true o false)

    display.display(); // pinta todas las primitivas anteriores desde que limpiaste con clearDisplay

  }
};