#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <WiFiAP.h>

#include <HTTPClient.h> 
#include <base64.h>

// Pines seguros para usar con ESP32
#define OLED_MOSI 23 // Cambiado de 12 a 23
#define OLED_CLK 18  // Cambiado de 13 a 18
#define OLED_DC 4    // Cambiado de 27 a 4
#define OLED_CS -1   // Sigue sin usarse
#define OLED_RST 5   // Cambiado de 14 a 5

#define BTN_CHANGE_MODE 12

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

const char *ssid = "LT";
const char *password = "petrikldeidad";

HTTPClient http;
int httpCode;
String response;

String mostConfidentLabel;
double confidence;

const char* serverUrl = "http://192.168.252.193:8000/api/sendImg/";
const char* contentType = "application/json";
const char* cameraServer = "http://192.168.252.129/capture"; 

bool automatic = true; // Estado inicial
bool btnPreviousState = HIGH; // Estado previo del botón

static const unsigned char PROGMEM logoBitmap[] = {
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B11111001, B11110000,
    B11111111, B11110000,
    B10011111, B11110000,
    B11111001, B11110000,
    B11111001, B11110000,
    B01111001, B11100000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000
};

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

class Screen
{
private:
public:

  static void setConfiguration()
  {
    // text display tests
    display.setTextSize(1);
    display.setTextColor(1);
    // Special characters
  }

  static String getPlate()
  {
    String plateNumber = "ABC123"; // Placa almacenada en la variable
    return plateNumber;
  }

  static String getOwner(String plate)
  {
    String result = "Jose Perez";
    // String result = "No regist.";
    return result;
  }

  static String getObs(String plate)
  {
    String result = "Sin RQ";
    // String result = "Con RQ";
    // String result = "-";
    return result;
  }

  void showBegin(){
    display.clearDisplay();
    // Icon
    display.drawBitmap(23, 15, logoBitmap, 12, 12, 1);
    // Text
    display.setCursor(40, 17);
    display.print("Smart Glass");
    display.setCursor(30, 32);
    display.write(173);
    display.print("Bienvenido!");
    display.display();
  }

  void showEnd(){
    display.clearDisplay();
    // Icon
    display.drawBitmap(23, 15, logoBitmap, 12, 12, 1);
    // Text
    display.setCursor(40, 17);
    display.print("Smart Glass");
    display.setCursor(20, 32);
    display.write(173);
    display.print("Hasta luego!");
    display.display();
  }

  static void showAutomaticModeTitle()
  {
    display.clearDisplay();
    // Icon
    display.drawBitmap(0, 0, radarBitmap, 12, 12, 1);
    // Text
    display.setCursor(15, 2);
    display.print("Modo : Automático");
    display.display();
  }

  static void showManualModeTitle()
  {
    display.clearDisplay();
    // Icon
    display.drawBitmap(0, 0, cameraBitmap, 12, 12, 1);
    // Text
    display.setCursor(15, 2);
    display.print("Modo : Manual");
    display.display();
  }

  static void initAutomaticMode()
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

  static void initManualMode()
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

  static void showPlate()
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

  static void connecting()
  {

    display.clearDisplay();
    display.display();
    // Text
    display.setCursor(15, 2);
    display.print("Conectando...");
    display.display();
  }
};
void parseJsonString(String jsonString, String& mostConfidentLabel, double& confidence) {
    if (jsonString.indexOf("error")>=0) {
        mostConfidentLabel = "Error";
        confidence = 0;
    } else {
        int index = jsonString.indexOf(',');
        mostConfidentLabel = jsonString.substring(26, index-1);
        confidence = jsonString.substring(index+16, jsonString.length()-1).toDouble();
    }
}
void setup()
{
  Serial.begin(9600);
  
  pinMode(BTN_CHANGE_MODE, INPUT);

  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);

  display.begin(0, true); // Reset incluido

  // Set configuration
  Screen::setConfiguration();

  while (WiFi.status() != WL_CONNECTED) {
    Screen::connecting();
  }

  // Automatic mode
  Screen::initAutomaticMode();
}

void loop()
{
  // El código principal para repetirse continuamente puede ir aquí
    /* if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(cameraServer);
      httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
          String image_data = http.getString();
          http.end();
          http.begin(serverUrl);
          http.addHeader("Content-Type", contentType);
          String payload = "{\"frame\":\"" + base64::encode((uint8_t*)image_data.c_str(), image_data.length()) + "\"}";
          httpCode = http.POST(payload);
          if (httpCode > 0) {

            response = http.getString();

            Serial.print("Response: ");
            Serial.println(response);
            
            parseJsonString(response, mostConfidentLabel, confidence);
            http.end();

            if (mostConfidentLabel!="Error") {
              if (confidence<60) {
                servos.openGeneral();
                mostConfidentLabel = "GENERALES";
              } else {
                if (mostConfidentLabel == "PLASTICO" || mostConfidentLabel == "VIDRIO") {
                  servos.openPlastic();
                } else if (mostConfidentLabel == "CARTON" || mostConfidentLabel == "PAPEL") {
                  servos.openPaper();
                } else {
                  servos.openGeneral();
                }
              }
              lcd.printDetection(mostConfidentLabel, confidence);
            }
          }
        }
      }
    } */
  Serial.begin(9600);
  display.begin(0, true); // Reset incluido
  //Screen::paintMain(&display);

  // Set configuration
  Screen::setConfiguration();

  // Begin
  showBegin();
  delay(3000);

  // Automatic mode
  initAutomaticMode();
  delay(3000);

  // Manual mode
  initManualMode();
  delay(3000);

  showEnd();

}
