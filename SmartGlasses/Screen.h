#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

static const unsigned char PROGMEM heartBitmap[] = {
    0b00001100, 0b00110000, // Fila 0
    0b00011110, 0b01111000, // Fila 1
    0b00111111, 0b01111110, // Fila 2
    0b01111111, 0b11111111, // Fila 3
    0b01111111, 0b11111111, // Fila 4
    0b00111111, 0b11111110, // Fila 5
    0b00011111, 0b11111100, // Fila 6
    0b00001111, 0b11111000, // Fila 7
    0b00000111, 0b11110000, // Fila 8
    0b00000011, 0b11100000, // Fila 9
    0b00000001, 0b11000000, // Fila 10
    0b00000000, 0b10000000, // Fila 11
    0b00000000, 0b00000000, // Fila 12
    0b00000000, 0b00000000, // Fila 13
    0b00000000, 0b00000000, // Fila 14
    0b00000000, 0b00000000  // Fila 15
}


namespace Screen {
  void printHelloWorld(Adafruit_SH1106G*screen) {
    display->clearDisplay(); // limpia pantalla

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

    display.drawBitmap(x, y, heartBitmap, 16, 16, 1);

    //display->invertDisplay(true); // invierte (true o false)

    display->display(); // pinta todas las primitivas anteriores desde que limpiaste con clearDisplay

  }
};