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

char* query = " { \"query\": \"query { viewer { contributionsCollection { contributionCalendar { weeks { contributionDays { color } } } } } }\" } ";

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

    int i = 0;
    for(int week = 30; week <= 52; week++){
        for(int day = 0; day < 7; day++){
            JSONVar color = decoded_data["data"]["viewer"]["contributionsCollection"]["contributionCalendar"]["weeks"][week]["contributionDays"][day]["color"];
            //Serial.println(color);
            setColor(i, color);
            i++;
        }
    }
}

void setColor(int i, JSONVar color){
    String hexColor = JSON.stringify(color);
    hexColor.remove(0, 2);
    hexColor.remove(6, 1);

    long number = (long) strtol( &hexColor[1], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;

    leds[i].setRGB(r, g , b);
    if(hexColor == "ebedf0"){       //no contributions
		leds[i] = CRGB::Black;
    }
    else{
        leds[i].setRGB(r, g , b);
    }
	FastLED.show();
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
            //Serial.println(payload);
            parseJSON(payload);
        }
        else {
            Serial.println("ERROR!");
        }
    }
    delay(300000);
}