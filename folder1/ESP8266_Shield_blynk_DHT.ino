/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 *
 * This example shows how to use ESP8266 Shield (with AT commands)
 * to connect your project to Blynk.
 *
 * WARNING!
 *   It's rather tricky to get it working, please read this article:
 *   https://github.com/blynkkk/blynk-library/wiki/ESP8266-with-AT-firmware
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 * Feel free to apply it to any other example. It's simple!
 *
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleDHT.h>
#include <SimpleTimer.h>
//#include <ESP8266WiFi.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "7d8f8af1075442398792d24f0b30b889";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "SK_WiFi7915";
//char pass[] = "1307010367";
char ssid[] = "PSTORM_C";
char pass[] = "1234asdf";


// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(8, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

SimpleTimer timer;

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11;

LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void DHT_Read(){

    
    // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }
    Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" %");
  
  Blynk.virtualWrite(V0, (int)temperature);
  Blynk.virtualWrite(V1, (int)humidity);


  // write a temperature
  lcd.setCursor(0,0);
  lcd.print("temp:");
  lcd.setCursor(8,0);
  lcd.print((int)temperature);
  lcd.setCursor(11,0);
  lcd.print("C");
  
  // write a humidity
  lcd.setCursor(0,1);
  lcd.print("Hum:");
  lcd.setCursor(8,1);
  lcd.print((int)humidity);
  lcd.setCursor(11,1);
  lcd.print("%");



}



void setup()
{
  // Set console baud rate
  Serial.begin(115200);
  delay(10);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Connecting to"); 
  lcd.setCursor(0,1);
  lcd.print(ssid);
  Blynk.begin(auth, wifi, ssid, pass);
  // Setup a function to be called every second
  timer.setInterval(2000L, DHT_Read);  

  lcd.clear();
  if(wifi.getAPIp()!=NULL){
    lcd.setCursor(0,0);
    lcd.print("WiFi connected");   // test edit 
  }else{
    lcd.setCursor(0,0);
    lcd.print("WiFi failed");      
  }
  delay(3000);
  lcd.clear();
  //lcd.noBacklight();
}

void loop()
{
  Blynk.run();
  timer.run();

  
}

