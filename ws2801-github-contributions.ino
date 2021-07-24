#include <FastLED.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "data.h" //only for privacy reasons

#define NUM_LEDS 161

#define DATA_PIN 25
#define CLOCK_PIN 26

CRGB leds[NUM_LEDS];

const char* ssid = ssid_private;            //"YOUR SSID HERE"
const char* password = password_private;    //"YOUR PASSWORD HERE"

char* query = " { \"query\": \"query { viewer { contributionsCollection { contributionCalendar { weeks { contributionDays { date color } } } } } }\" } ";

void initWiFi(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi ..");
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

void parseJSON(String JSON_Data){
    JSONVar decoded_data = JSON.parse(JSON_Data);
    if (JSON.typeof(decoded_data) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
    }

    JSONVar keys = decoded_data.keys();

    Serial.println("JSON Sample:");
    Serial.println();
}

void setup(){
	Serial.begin(57600);
	FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, GBR>(leds, NUM_LEDS);
	FastLED.setBrightness(50);
	initWiFi();
}

void loop(){
    if (WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin("https://api.github.com/graphql");
        http.addHeader("Authorization", githubToken);       //githubToken = "bearer YOUR_TOKEN"
        int httpCode = http.POST(query);
        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
            parseJSON(payload);
        }
        else {
            Serial.println("ERROR!");
        }
    }
    delay(300000);
}