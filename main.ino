//library 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <BH1750.h>
#include <SFE_BMP180.h>
//Blynk
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "zPhTRw1mgdVNjbg4i6gi1OWqa2XS_dIq";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wifi-name";
char pass[] = "password";

//Thingspeak
#include "ThingSpeak.h"
WiFiClient  client;
//unsigned long myChannelNumber = "1697736";
const char * myWriteAPIKey = "G6D1RICPDASOXPVU";

//OLED
#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 14     
#define DHTTYPE    DHT11     
SFE_BMP180 pressure;
BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);//BAUDRADE
  Blynk.begin(auth, ssid, pass);
  ThingSpeak.begin(client);
  Serial.begin(115200);//BAUDRADE
  Wire.begin();
  dht.begin();
  lightMeter.begin();
  pressure.begin();
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  
  // sensor result reset after 2 seconds
  delay(2000);
  //read LIGHT INTENSITY lux 
  float lux = lightMeter.readLightLevel();
  //read Temperature and Humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  float pres = 0;
  float temp = 0;
  double T, P, p0, a;
  pressure.startTemperature();
  pressure.getTemperature(T);
  pressure.startPressure(3);
  pressure.getPressure(P, T);
  pres = P;
  temp = T;
  Blynk.run();
  // clear display
  display.clearDisplay();
  
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,3);
  display.print("Temperature: ");
  display.setTextSize(1);
  display.print(temp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);// Celsius 
  display.setTextSize(1);
  display.print("C");
  Blynk.virtualWrite(V0, temp);
  ThingSpeak.writeField(1697736, 1, temp, myWriteAPIKey);
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0,18);
  display.print("Humidity: ");
  display.setTextSize(1);
  display.print(h);
  display.print(" %"); 
  Blynk.virtualWrite(V1, h);
  ThingSpeak.writeField(1697736, 2, h, myWriteAPIKey);

  // display light intensity
  display.setTextSize(1);
  display.setCursor(0,30);
  display.print("Light: ");
  display.print(lux);
  display.print(" lux");
  Blynk.virtualWrite(V3, lux);
  ThingSpeak.writeField(1697736, 3, lux, myWriteAPIKey);
  
   // display pressure
  display.setTextSize(1);
  display.setCursor(0,45);
  display.print("Air Pressure: ");
  display.print(pres);
  Blynk.virtualWrite(V2, pres);
  ThingSpeak.writeField(1697736, 4, pres, myWriteAPIKey);
  
  display.display(); 
  
}
