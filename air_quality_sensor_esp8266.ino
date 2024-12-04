#include "Adafruit_CCS811.h"
#include <LiquidCrystal_I2C.h>

#include <DHT.h>

#define DHTPIN 14     // GPIO14 -> D5

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT22

DHT dht(DHTPIN, DHTTYPE);
// https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/

// https://randomnerdtutorials.com/esp32-esp8266-i2c-lcd-arduino-ide/
Adafruit_CCS811 ccs;
//set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
float co2_ppms = 0;
float tvoc = 0;
float temp = 0;
float humidity = 0;
int counter = 0;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {

  Serial.begin(9600);
  Serial.println("CCS811 test");

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  //calibrate temperature sensor
  while(!ccs.available());

  lcd.init();
  // lcd.backlight();
  lcd.noBacklight();

  dht.begin();
}

void loop() {
    float temp;
    float co2_ppms;
    float tvoc;
    if(ccs.available()){
      co2_ppms = ccs.geteCO2();
      tvoc = ccs.getTVOC();
      if(!ccs.readData()){
        // Successfully read data
      }
      else {
        Serial.println("ERROR!");
        while(1);
      }
    }

    temp = dht.readTemperature();
    if (isnan(temp)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    humidity = dht.readHumidity();
    if (isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
    }

  delay(500);
  counter += 1;

  if (counter == 10) {

    Serial.print("CO2:");
    Serial.print(co2_ppms);
    Serial.print(" ppm, TVOC:");
    Serial.print(tvoc);
    Serial.print(" ppb, temp:");
    Serial.print(temp);
    Serial.print(" , hum:");
    Serial.print(humidity);
    Serial.print(" %");
    Serial.println();

    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("C:" + String(int(co2_ppms)) + " TV:" + String(int(tvoc)));
    lcd.setCursor(0,1);
    Serial.print("CO2 ");
    Serial.print(co2_ppms);
    Serial.print("ppm TVOC ");
    Serial.print(tvoc);
    Serial.print("ppb temp ");
    Serial.print(temp);
    Serial.print(" hum ");
    Serial.print(humidity);
    Serial.print("%");
    lcd.print("H:" + String(int(humidity)) + " T:" + String(int(temp)));

    counter = 0;
  }
  delay(1000);
}
