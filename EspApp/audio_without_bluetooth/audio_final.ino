#include <HTTPClient.h>
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

#define ESP_TAG "esp1"
            

String ssid = "BINBIRNET_EMIN";
String password = "eminmerve1223";

//Your IP address or domain name with URL path
String serverName = "http://08007fd64982.ngrok.io/";
String cmd, lastCmd ,url;
int newurl = 0;

unsigned long previousMillis = 0;
Audio audio;
TaskHandle_t Task1;
void setup()
{
  
  Serial.begin(115200);
  xTaskCreatePinnedToCore(
                 Task1code,   /* Task function. */
                 "Task1",     /* name of task. */
                 10000,       /* Stack size of task */
                 NULL,        /* parameter of the task */
                 1,           /* priority of the task */
                 &Task1,      /* Task handle to keep track of created task */
                 0);          /* pin task to core 0 */      
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  audio.setInternalDAC(true);
  audio.setVolume(21);
  cmd = "";
  lastCmd = "";


}
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
         
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      String reqUrl = serverName + ESP_TAG;
      cmd = httpGETRequest(reqUrl.c_str());
      Serial.println(cmd);
      if (cmd != lastCmd)
      {
        Serial.println("New Audio : " + cmd);
        url = serverName + cmd;
        newurl = 1;
        Serial.println(url.c_str());
        lastCmd = cmd;
        
      }
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
  delay(5000);
  } 
}
void loop()
{
  if(newurl == 1){
    audio.connecttohost(url.c_str());
    newurl = 0;
  }
audio.loop();
}

String httpGETRequest(const char *serverName)
{
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  // Free resources
  http.end();
  return payload;

}

