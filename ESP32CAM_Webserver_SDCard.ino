/*
Rayko Panteleev ESP32 webserver built with files from a microSD card on ESP32 Camera module
*/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD_MMC.h"

// Replace with your network credentials
const char* ssid = "NetworkID"; // Replace with your wireless network ID
const char* password = "password"; // Replace with your password

// Create AsyncWebServer object on port 80
AsyncWebServer server(80); // Server Port

void initSDCard(){
  if(!SD_MMC.begin()){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  initSDCard();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD_MMC, "/index.html", "text/html");
  });

  server.serveStatic("/", SD_MMC, "/");

  server.begin();
}

void loop() {
  
}
