﻿#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

const uint8_t happyFaceBitmap[] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xcc, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0xfb, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x7e, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfd, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfb, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xbf, 0xff, 0xff, 
	0xff, 0xff, 0xfb, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xbf, 0xff, 0xff, 
	0xff, 0xff, 0xfb, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xf9, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xf8, 0x3e, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x03, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0xe6, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xe3, 0x8e, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0xe3, 0x8e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf7, 0xde, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xfe, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xef, 0xfe, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0xe7, 0xde, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0x3e, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xf0, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xe1, 0xe7, 0xe7, 0x87, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xf1, 0xff, 0xff, 
	0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 
	0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 
	0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 
	0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 
	0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 
	0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 
	0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 
	0xff, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
	0xff, 0xff, 0x9f, 0xfc, 0x3f, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xf0, 0x0f, 0x87, 0xff, 0xff, 
	0xff, 0xff, 0xf8, 0x07, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
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
    display.drawRGBBitmap(32, 0, heartBitmap, 64, 64);

    display->invertDisplay(true); // invierte (true o false)

    display->display(); // pinta todas las primitivas anteriores desde que limpiaste con clearDisplay

  }
};