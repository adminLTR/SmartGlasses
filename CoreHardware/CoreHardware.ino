#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

class Screen {
  private:
    
  public:
    static void paintMain(Adafruit_SH1106G*display) {
        static const unsigned char PROGMEM carBitMap[] = {
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

      display->clearDisplay();

      int x = (display->width() - 12) / 2;
      int y = (display->height() - 12) / 2;

      // Dibuja el bitmap del corazón
      display->drawBitmap(x, y, carBitMap, 12, 12, 1);

      // Muestra texto adicional
      display->setTextSize(1);
      display->setTextColor(1);
      display->setCursor(15, 2);
      display->println("Placa:");
      display->display();

      String plateNumber = "ABC-173"; // Placa almacenada en la variable
      display->setCursor(52, 2);
      display->println(plateNumber);
      display->setRotation(2);
      display->display();
    }
};

// Pines seguros para usar con ESP32
#define OLED_MOSI 23 // Cambiado de 12 a 23
#define OLED_CLK  18 // Cambiado de 13 a 18
#define OLED_DC   4  // Cambiado de 27 a 4
#define OLED_CS   -1 // Sigue sin usarse
#define OLED_RST  5  // Cambiado de 14 a 5

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

void setup() {
    Serial.begin(9600);
    display.begin(0, true); // Reset incluido
    Screen::paintMain(&display);
    delay(3000); // Pausa antes de continuar
}

void loop() {
    // El código principal para repetirse continuamente puede ir aquí
}
