#include <SPI.h>
#include <Wire.h>

#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <TimeLib.h>

#include <LiquidCrystal_I2C.h>

#include "RTClib.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);
String arr_days[] = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
String date_time;


LiquidCrystal_I2C lcd(0x27, 20, 4);

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int A0Value = 0;
#define Active_buzzer D3

#define CLK 14 //D6
#define DT 12  //D5
#define SW 13  //D7

#include "GyverEncoder.h"
Encoder encoder(CLK, DT, SW, TYPE2);

void ICACHE_RAM_ATTR isrCLK();
void ICACHE_RAM_ATTR isrDT();

volatile char directionEncoder = ' ';

void setup() {
  Serial.begin(9600);

  //Active buzzer output
  pinMode (Active_buzzer, OUTPUT);
  digitalWrite(Active_buzzer, HIGH);
  delay(100);
  digitalWrite(Active_buzzer, LOW);


  Serial.println("Screen init...");
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Boiler center");
  lcd.setCursor(0, 1);
  lcd.print("");
  lcd.setCursor(0, 2);
  lcd.print("");
  lcd.setCursor(0, 3);
  lcd.print("");
  digitalWrite(Active_buzzer, HIGH);
  delay(100);
  digitalWrite(Active_buzzer, LOW);
  delay(1000);

  // Wi-Fi connect
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res) {
    Serial.println("Unable to connect to WiFi!");
    lcd.setCursor(3, 0);
    lcd.print("Unable to connect to WiFi!");
    digitalWrite(Active_buzzer, HIGH);
    delay(1000);
    digitalWrite(Active_buzzer, LOW);
    delay(500);
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("Wi-Fi connected!");
    lcd.setCursor(0, 1);
    lcd.print("Wi-Fi connected!");
    digitalWrite(Active_buzzer, HIGH);
    delay(100);
    digitalWrite(Active_buzzer, LOW);
    delay(500);
  }

  //NTP

  timeClient.begin();
  timeClient.setTimeOffset(3 * 3600); // Moscow time GMT+3, Надо вынести в настройки ПО

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
    delay(10000);
    Serial.flush();
    abort();
  }

  //  if (rtc.lostPower()) {
  Serial.println("RTC lost power, let's set the time!");
  rtc.adjust(DateTime(timeClient.getEpochTime()));
  lcd.setCursor(0, 2);
  lcd.print("Time synchronized!");
  digitalWrite(Active_buzzer, HIGH);
  delay(100);
  digitalWrite(Active_buzzer, LOW);
  delay(2000);
  //  }

  attachInterrupt(CLK, isrCLK, CHANGE);    // прерывание на 2 пине! CLK у энка
  attachInterrupt(DT, isrDT, CHANGE);    // прерывание на 3 пине! DT у энка

  lcd.clear();

}

void isrCLK() {

  encoder.tick();
  if (encoder.isRight()) directionEncoder = 'R';
  if (encoder.isLeft())  directionEncoder = 'L';
  Serial.println(directionEncoder);
  /*
     lcd.setCursor(0, 2);
     lcd.print("Rotary:");
     lcd.print(directionEncoder);
  */
}
void isrDT() {
  encoder.tick();
  if (encoder.isRight()) directionEncoder = 'R';
  if (encoder.isLeft())  directionEncoder = 'L';
  Serial.println(directionEncoder);
  /*
     lcd.setCursor(0, 2);
     lcd.print("Rotary:");
     lcd.print(directionEncoder);
  */
}

void loop(void) {
  encoder.tick();
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

  Serial.print("Temp: ");            // Отправка текста в Serial Port
  Serial.println(rtc.getTemperature()); // Отправка в Serial Port температуру

  lcd.setCursor(0, 0);                // Устанавливаем курсор на 0 строку, ячейка 0
  if (now.hour() < 10) {
    lcd.print('0');
  }
  lcd.print(now.hour(), DEC);         // Отправка времени на LCD
  lcd.print(":");                     // Выводим текст на LCD
  if (now.minute() < 10) {
    lcd.print('0');
  }
  lcd.print(now.minute(), DEC);       // Отправка времени на LCD
  lcd.print(":");
  if (now.second() < 10) {
    lcd.print('0');
  }
  lcd.print(now.second(), DEC);

  A0Value = analogRead(A0);
  lcd.setCursor(0, 1);
  lcd.print("A0 value:");
  lcd.print(A0Value);
}
