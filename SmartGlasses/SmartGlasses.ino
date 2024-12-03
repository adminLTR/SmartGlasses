#include "esp_camera.h"
#include <WiFi.h>

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "LT";
const char *password = "petrikldeidad";

const char* api_url = "http://192.168.1.1:8000/sendImg/";

void startCameraServer();
void setupLedFlash(int pin) {
    ledcAttach(pin, 5000, 8);
}

// Función para capturar y enviar la imagen
void captureAndSendImage() {
  // Capturar la imagen
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Error al capturar la imagen");
    return;
  }

  // Crear la solicitud HTTP
  HTTPClient http;
  http.begin(api_url);

  String boundary = "----ESP32Boundary";
  String contentType = "multipart/form-data; boundary=" + boundary;

  // Crear el cuerpo de la solicitud
  String bodyStart = "--" + boundary + "\r\n";
  bodyStart += "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n";
  bodyStart += "Content-Type: image/jpeg\r\n\r\n";

  String bodyEnd = "\r\n--" + boundary + "--\r\n";

  // Calcular la longitud del contenido
  int contentLength = bodyStart.length() + fb->len + bodyEnd.length();

  // Establecer encabezados
  http.addHeader("Content-Type", contentType);
  http.addHeader("Content-Length", String(contentLength));

  // Enviar la solicitud
  WiFiClient* stream = http.getStreamPtr();
  stream->print(bodyStart);
  stream->write(fb->buf, fb->len); // Datos binarios de la imagen
  stream->print(bodyEnd);

  // Obtener la respuesta del servidor
  int httpResponseCode = http.POST("");
  if (httpResponseCode > 0) {
    Serial.printf("Código de respuesta HTTP: %d\n", httpResponseCode);
  } else {
    Serial.printf("Error al enviar imagen: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  // Liberar el frame buffer y finalizar la solicitud HTTP
  esp_camera_fb_return(fb);
  http.end();
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  config.fb_location = CAMERA_FB_IN_PSRAM;

  config.jpeg_quality = 10;
  config.fb_count = 2;
  config.grab_mode = CAMERA_GRAB_LATEST;
    
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

  setupLedFlash(LED_GPIO_NUM);

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  // Do nothing. Everything is done in another task by the web server
  captureAndSendImage();
  delay(10000);
}
