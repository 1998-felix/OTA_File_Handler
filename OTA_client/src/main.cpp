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

// Objects
HTTPClient http;

// variables
unsigned long last_time = 0;
unsigned long timer_delay = 5000;

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

  } else {
    Serial.println("_________________________________");
    Serial.println("Fatal Error. Wifi connection Lost");
    Serial.println(".................................");
  }
}