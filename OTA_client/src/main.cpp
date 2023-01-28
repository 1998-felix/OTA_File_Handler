#include <Arduino.h>
#include <HTTPClient.h>
#include <Update.h>
#include <WiFi.h>

#include "credentials.h"
#include "serverdetails.h"

// function prototypes
void scanNetworks();
void connnectWiFi();
void displayWiFiDetails();
void connectToServer();
void flashBinary();
void updateFirmware(uint8_t* data, size_t len);

// Objects
HTTPClient http;

// variables
unsigned long last_time = 0;
unsigned long timer_delay = 5000;
int total_length;
int current_length = 0;

void setup() {
  // set up serial
  Serial.begin(115200);

  // WI-FI Protocols
  // 1. Scan and display networks
  scanNetworks();
  // 2. Connect to network
  connnectWiFi();
  // 3. List wi-fi connection details
  displayWiFiDetails();

  // Connect to server
  connectToServer();

  // Download and flash content
  flashBinary();


  // Built in LED
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(1000);
}
void scanNetworks() {
  Serial.println(".................");
  Serial.println("Scanning networks");
  Serial.println(".................");
  byte num_ssid = WiFi.scanNetworks();
  for (int i = 0; i < num_ssid; i++) {
    Serial.print("Network # ");
    Serial.printf(" %d ", i);
    Serial.println(WiFi.SSID(i));
  }
  Serial.println(".................");
  Serial.println("Scan complete....");
  Serial.println("..................");
}
void connnectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf("Attempting to connect to: %s \n", my_credentials.ssid);
    WiFi.begin(my_credentials.ssid, my_credentials.password);
    delay(3000);
    if (WiFi.status() == WL_IDLE_STATUS) {
      Serial.println("Attempting connection. Device Idle ");
    }
  }
  Serial.println("Wifi connected succesfully!!!!!");
}
void displayWiFiDetails() {
  long rssi = WiFi.RSSI();
  IPAddress ip;
  ip = WiFi.localIP();
  Serial.print("Wifi Network: ");
  Serial.print(my_credentials.ssid);
  Serial.print(" RSSI : ");
  Serial.print(rssi);
  Serial.println(" dbm");
  Serial.print("Device Ip Address: ");
  Serial.println(ip);
}
void connectToServer() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(server_URL_path);
    int http_response_code = 0;
    do {
      http_response_code = http.GET();
      Serial.print("http Response code:");
      Serial.println(http_response_code);
      delay(3000);
    } while (http_response_code != 200);
    Serial.print("Successfully connected to: ");
    Serial.println(server_URL);
   // http.end(); kills wifi for some reason. To be investigated.

  } else {
    Serial.println("_________________________________");
    Serial.println("Fatal Error. Wifi connection Lost");
    Serial.println(".................................");
  }
}
void flashBinary(){
  if(WiFi.status() == WL_CONNECTED){
    http.begin(server_bin_path);
    int http_server_response = http.GET();
    if(http_server_response == 200){
      total_length = http.getSize();
      
    int len = total_length;
    Update.begin(UPDATE_SIZE_UNKNOWN);
    Serial.printf("FW Size: %u\n", total_length);

    uint8_t buff[128] = {0};
    WiFiClient * stream = http.getStreamPtr();

    Serial.println("Updating firmware ....");

    while(http.connected() && (len > 0 || len == -1)){
      size_t size = stream->available();
      if(size){
        //read
        int c = stream->readBytes(buff,((size > sizeof(buff))? sizeof(buff):size));

        updateFirmware(buff,c);
        if(len> 0){
          len -= c;
        }
      }
      delay(1);
    }


    }else{
      Serial.print("Invalid response:");
      Serial.println(http_server_response);
    }

  }else{
    Serial.println("_________________________________");
    Serial.println("Fatal Error.Wifi connection lost");
    Serial.println("..................................");
  }

}
void updateFirmware(uint8_t* data, size_t len){
  Update.write(data, len);
  current_length += len;
  
  Serial.print(".");

  if(current_length != total_length) return;
  Update.end(true);
  Serial.printf("\nUpdate Success,Total Size: %u\n Rebooting...\n",current_length);

  ESP.restart();

}