#include <FastLED.h>
#include <WiFi.h>
#include "data.h" //only for privacy reasons

#define NUM_LEDS 161

#define DATA_PIN 25
#define CLOCK_PIN 26

CRGB leds[NUM_LEDS];

const char* ssid = ssid_private;            //"YOUR SSID HERE"
const char* password = password_private;    //"YOUR PASSWORD HERE"


void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi ..");
    Serial.println(WiFi.macAddress());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
    wifiConected();
}

void wifiConected(){
	int i;
	for (i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = CRGB::Green;
		FastLED.show();
	}
	for (i = NUM_LEDS; i >= 0; i--)
	{
		leds[i] = CRGB::Black;
		FastLED.show();
	}
}

void setup()
{
	Serial.begin(57600);
	FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, GBR>(leds, NUM_LEDS);
	FastLED.setBrightness(50);
	initWiFi();
}

void loop() {}