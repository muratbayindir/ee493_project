#include <HTTPClient.h>
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

#define ESP_TAG "esp1"

String ssid = "BINBIRNET_EMIN";
String password = "eminmerve1223";

//Your IP address or domain name with URL path
String serverName = "http://192.168.4.1/";
String cmd, lastCmd;

unsigned long previousMillis = 0;
const long interval = 1000;
Audio audio;

void setup()
{
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
}

void loop()
{
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval)
	{
		// Check WiFi connection status
		if (WiFi.status() == WL_CONNECTED)
		{
			String reqUrl = serverName + ESP_TAG;
			cmd = httpGETRequest(reqUrl.c_str());

			if (cmd != lastCmd)
			{
				Serial.println("New Audio : " + cmd);
				audio.connecttohost(serverName + cmd);
				lastCmd = cmd;
			}

			// save the last HTTP GET Request
			previousMillis = currentMillis;
		}
		else
		{
			Serial.println("WiFi Disconnected");
		}
	}
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
