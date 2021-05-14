#include <HTTPClient.h>
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
String ssid =     "BINBIRNET_EMIN";
String password = "eminmerve1223";

//Your IP address or domain name with URL path
const char* serverNameItem = "http://192.168.4.1/item";

String item;

unsigned long previousMillis = 0;
const long interval = 5000; 
Audio audio;
void setup() {
  
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  audio.setInternalDAC(true);
  if(item == "0"){
  audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac");
  } 
  if(item == "1"){
  audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac");
  } 
  if(item == "2"){
  audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac");
  } 
  if(item == "3"){
  audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac");
  } 
  if(item == "4"){
  audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac");
}
}
void loop() {
  unsigned long currentMillis = millis(); 
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      item = httpGETRequest(serverNameItem);
      Serial.println("Item: " + item);      
      // save the last HTTP GET Request
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}
String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  return payload;
}

