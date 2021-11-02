#include <SPI.h>
#include <Wire.h>

#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <TimeLib.h>

#include <LiquidCrystal_I2C.h>

#include "RTClib.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000);
String arr_days[]={"Воскресенье","Понедельник","Вторник","Среда","Четверг","Пятница","Суббота"};
String date_time;


LiquidCrystal_I2C lcd(0x27,20,4);

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(9600);
  Serial.println("Screen init..."); 

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");
  lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
  lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");

  delay(2000);

  // Wi-Fi connect
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!res) {
      Serial.println("Unable to connect to WiFi!");
      lcd.setCursor(3,0);
      lcd.print("Unable to connect to WiFi!");
      delay(500);
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("Wi-Fi connected!");
      lcd.setCursor(0,0);
      lcd.print("Wi-Fi connected!");
      delay(500);
  }

  //NTP

  timeClient.begin();
  timeClient.setTimeOffset(3*3600); // Moscow time GMT+3

  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0

  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

//  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(timeClient.getEpochTime()));
//  }

}


void loop(void) {

    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

  Serial.print(" - Temp: ");          // Отправка текста в Serial Port
  Serial.print(rtc.getTemperature()); // Отправка в Serial Port температуру
    
  //lcd.clear();
  lcd.setCursor(0, 0);                // Устанавливаем курсор на 0 строку, ячейка 0
  if (now.hour() < 10) {lcd.print('0');}
  lcd.print(now.hour(), DEC);         // Отправка времени на LCD
  lcd.print(":");                     // Выводим текст на LCD
  if (now.minute() < 10) {lcd.print('0');}
  lcd.print(now.minute(), DEC);       // Отправка времени на LCD
  lcd.print(":");
  if (now.second() < 10) {lcd.print('0');}
  lcd.print(now.second(), DEC); ;
  
  delay(500);
}
