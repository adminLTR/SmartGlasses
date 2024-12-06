#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

class Screen
{
private:
public:
  static const unsigned char PROGMEM vehicleBitmap[] = {
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
      B00000000, B00000000};

  static const unsigned char PROGMEM cameraBitmap[] = {
      B00000000, B00000000,
      B00001111, B00000000,
      B01111111, B11100000,
      B01111111, B11100000,
      B01110000, B11100000,
      B01110110, B11100000,
      B01110110, B11100000,
      B01110000, B11100000,
      B01111111, B11100000,
      B01111111, B11100000,
      B00000000, B00000000,
      B00000000, B00000000};

  static const unsigned char PROGMEM alertBitmap[] = {
      B00000000, B00000000,
      B00000110, B00000000,
      B00000110, B00000000,
      B00001111, B00000000,
      B00001111, B00000000,
      B00011001, B10000000,
      B00111111, B11000000,
      B00111001, B11000000,
      B01111001, B11100000,
      B01111111, B11100000,
      B00000000, B00000000,
      B00000000, B00000000};

  static const unsigned char PROGMEM radarBitmap[] = {
      B00000000, B00000000,
      B00000000, B00000000,
      B00000000, B00000000,
      B01000000, B00100000,
      B11010000, B10110000,
      B11110110, B11110000,
      B11110110, B11110000,
      B11010000, B10110000,
      B01000000, B00100000,
      B00000000, B00000000,
      B00000000, B00000000,
      B00000000, B00000000};

  static const unsigned char PROGMEM checkBitmap[] = {
      B00000000, B00000000,
      B00001111, B00000000,
      B00111111, B11000000,
      B00111111, B11000000,
      B01111111, B01100000,
      B01111110, B11100000,
      B01101101, B11100000,
      B01110011, B11100000,
      B00111111, B11000000,
      B00111111, B11000000,
      B00001111, B00000000,
      B00000000, B00000000};

  static const unsigned char PROGMEM personBitmap[] = {
      B00001111, B00000000,
      B00001111, B00000000,
      B00011111, B10000000,
      B00011111, B10000000,
      B00001111, B00000000,
      B00001111, B00000000,
      B00000110, B00000000,
      B00011111, B10000000,
      B00111111, B11000000,
      B01111111, B11100000,
      B01111111, B11100000,
      B00111111, B11000000};

  void setConfiguration()
  {
    // text display tests
    display.setTextSize(1);
    display.setTextColor(1);
    // Special characters
  }

  String getPlate()
  {
    String plateNumber = "ABC123"; // Placa almacenada en la variable
    return plateNumber;
  }

  String getOwner(String plate)
  {
    String result = "Jose Perez";
    // String result = "No regist.";
    return result;
  }

  String getObs(String plate)
  {
    String result = "Sin RQ";
    // String result = "Con RQ";
    // String result = "-";
    return result;
  }

  void showAutomaticModeTitle()
  {
    display.clearDisplay();
    // Icon
    display.drawBitmap(0, 0, radarBitmap, 12, 12, 1);
    // Text
    display.setCursor(15, 2);
    display.print("Modo : Automático");
    display.display();
  }

  void showManualModeTitle()
  {
    display.clearDisplay();
    // Icon
    display.drawBitmap(0, 0, cameraBitmap, 12, 12, 1);
    // Text
    display.setCursor(15, 2);
    display.print("Modo : Manual");
    display.display();
  }

  void initAutomaticMode()
  {
    showAutomaticModeTitle();

    // Text
    display.setCursor(25, 25);
    display.print("Escaneando...");
    display.display();
    delay(2000);

    // Si presiona el boton de capturar imagen, la procesa y mmuestra su resutado
    // Se limpia la pantalla
    showAutomaticModeTitle();

    // Aqui se debe imprimir el resultado
    showPlate();
    display.display();
  }

  void initManualMode()
  {
    showManualModeTitle();

    // Text
    display.setCursor(10, 25);
    display.print("> Captura imagen <");
    display.display();
    delay(2000);

    // Si presiona el boton de capturar imagen, la procesa y mmuestra su resutado
    // Se limpia la pantalla
    showManualModeTitle();

    // Aqui se debe imprimir el resultado de la plac detectada
    display.setCursor(10, 25);
    showPlate();
    display.display();
    delay(2000);
  }

  void showPlate()
  {

    String plateNumber = getPlate();
    String owner = getOwner(plateNumber);
    String obs = getObs(plateNumber);

    // Plate
    display.drawBitmap(0, 20, vehicleBitmap, 12, 12, 1);
    display.setCursor(15, 22);
    display.print("Placa: " + plateNumber);

    // Owner
    display.drawBitmap(0, 35, personBitmap, 12, 12, 1);
    display.setCursor(16, 37);
    display.print("Tit. : " + owner);

    // Data
    display.drawBitmap(0, 50, checkBitmap, 12, 12, 1);
    display.setCursor(16, 52);
    display.print("Obs. : " + obs);

    display.display();
  }

  static void paintMain(Adafruit_SH1106G *display)
  {
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
        B00000000, B00000000};

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
#define OLED_CLK 18  // Cambiado de 13 a 18
#define OLED_DC 4    // Cambiado de 27 a 4
#define OLED_CS -1   // Sigue sin usarse
#define OLED_RST 5   // Cambiado de 14 a 5

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

void setup()
{
  Serial.begin(9600);
  display.begin(0, true); // Reset incluido
  //Screen::paintMain(&display);

  // Set configuration
  Screen::setConfiguration();

  // Automatic mode
  Screen::initAutomaticMode();
  delay(3000);

  // Manual mode
  Screen::initManualMode();
  delay(3000);

  delay(3000); // Pausa antes de continuar
}

void loop()
{
  // El código principal para repetirse continuamente puede ir aquí
}
