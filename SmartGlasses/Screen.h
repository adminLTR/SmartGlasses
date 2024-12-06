﻿#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

const uint8_t heartBitmap[] PROGMEM = {
  0b00001110, 0b00001110,
  0b00011111, 0b00011111,
  0b00111111, 0b10111111,
  0b01111111, 0b11111111,
  0b01111111, 0b11111111,
  0b01111111, 0b11111111,
  0b00111111, 0b10111111,
  0b00011111, 0b00011111,
  0b00001110, 0b00001110,
  0b00000100, 0b00000100,
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

    // Dibuja el corazón en el centro de la pantalla
    screen->drawBitmap(56, 24, heartBitmap, 16, 16, SH110X_WHITE);

    display->invertDisplay(true); // invierte (true o false)

    display->display(); // pinta todas las primitivas anteriores desde que limpiaste con clearDisplay

  }
};