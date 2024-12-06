#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

const uint8_t heart16x16[] = {
    0b00001100, 0b00110000,
    0b00011110, 0b01111000,
    0b00111111, 0b01111110,
    0b01111111, 0b11111111,
    0b01111111, 0b11111111,
    0b00111111, 0b11111110,
    0b00011111, 0b11111100,
    0b00001111, 0b11111000,
    0b00000111, 0b11110000,
    0b00000011, 0b11100000,
    0b00000001, 0b11000000,
    0b00000000, 0b10000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000
};


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
    int x = (SCREEN_WIDTH - 16) / 2;  // 56
    int y = (SCREEN_HEIGHT - 16) / 2; // 24

    display.drawBitmap(x, y, heart16x16, 16, 16, SH110X_WHITE);

    //display->invertDisplay(true); // invierte (true o false)

    display->display(); // pinta todas las primitivas anteriores desde que limpiaste con clearDisplay

  }
};