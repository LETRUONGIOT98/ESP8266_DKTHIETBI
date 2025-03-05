//////YÊU CẦU PHẦN CỨNG//////
//Sử dụng Esp32 NODE MCU, Không sử dụng loại ESP32 C3///////
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "BLYNK_AUTH_TOKEN";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
int dkden =0, dkquat =0, dkbom =0,chedo =0;
#define den 13
#define quat 14
#define bom 12
#define bchedo 16
#define bden 17
#define bquat 5
#define bbom 18
#define ldr 34
int giobat = 10;   //////Set hẹn bật
int phutbat =0; ///Set đến h và phút sẽ bật

int giotat = 10;   //////Set hẹn giờ tắt phải lớn hơn giờ bật
int phuttat =5; ///Set đến giừo và phút sẽ tắt

#include "DHT.h"

#define DHTPIN 15  //Chân kết nối dht 11 với esp32
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <virtuabotixRTC.h>
// DS1302:  RST pin    -> ESP32 Digital 25
//          DAT pin   -> ESP32 Digital 26
//          CLK pin  -> ESP32 Digital 27
virtuabotixRTC myRTC(25, 26, 27);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
  // put your setup code here, to run once:
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
Serial.begin(9600);
lcd.setCursor(0, 0);
  lcd.print("  KET NOI WIFI  ");
  lcd.setCursor(0, 1);
  lcd.print("XIN VUI LONG CHO");
 Blynk.begin(auth,ssid, pass);
 dht.begin();
 myRTC.setDS1302Time(00, 05, 00, 6, 21, 05, 2022);  //Set ngày giờ 
 lcd.clear();
}
BLYNK_WRITE(V0)
{
  dkden = param.asInt(); // assigning incoming value from pin V1 to a variable
  if(chedo == 0){
    digitalWrite(den, dkden);
  }
}  
BLYNK_WRITE(V1)
{
  dkquat = param.asInt(); // assigning incoming value from pin V1 to a variable
  if(chedo == 0){
    digitalWrite(quat, dkquat);
  }
} 
BLYNK_WRITE(V2)
{
  dkbom = param.asInt(); // assigning incoming value from pin V1 to a variable
  if(chedo == 0){
    digitalWrite(bom, dkbom);
  }
}  
BLYNK_WRITE(V3)
{
  chedo = param.asInt(); // assigning incoming value from pin V1 to a variable
}
void loop() {
  Blynk.run();
  if(digitalRead(bchedo) == 0){
    chedo = chedo +1;
    if(chedo >= 2)chedo =0;
    Blynk.virtualWrite(V3, chedo);
    delay(500);
  }
//////////THỦ CÔNG/////////////////////
if(chedo == 0){
    if(digitalRead(bden) == 0){
      digitalWrite(den, !digitalRead(den));
      delay(500);
    }
    if(digitalRead(bquat) == 0){
      digitalWrite(quat, !digitalRead(quat));
      delay(500);
  }
    if(digitalRead(bbom) == 0){
      digitalWrite(bom, !digitalRead(bom));
      delay(500);
  }
}
float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
float t = dht.readTemperature();
myRTC.updateTime();
int gio = myRTC.hours;
int phut = myRTC.minutes;
int giay = myRTC.seconds;
int dosang = map(analogRead(A0),0,1023,0,100);
Blynk.virtualWrite(V4, h);
Blynk.virtualWrite(V5, t);
Blynk.virtualWrite(V6, dosang);
lcd.setCursor(0, 0);
  lcd.print("t:    *C");
  lcd.setCursor(3, 0);
  lcd.print(t,1);
  lcd.setCursor(9, 0);
  lcd.print("h:    %");
  lcd.setCursor(11, 0);
  lcd.print(h,0);
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(String(gio) + " : " + String(phut) + " : " +String(giay));
//////////TỰ ĐỘNG////////////////
if(chedo == 1){
 if(dosang <= 40){   //Set độ sáng bật đèn thấp hơn thì bật
  digitalWrite(den, HIGH); 
 }
 else{digitalWrite(den, LOW);}

 if(t >= 30){  //set nhiệt độ bật quạt lớn hơn thfi bật
  digitalWrite(den, HIGH); 
 }
 else{digitalWrite(den, LOW);}

if(gio >= giobat && phut >= phutbat && gio < giotat && phut < phuttat){
  digitalWrite(bom, HIGH);
}
if(gio > giotat && phut > phuttat){
  digitalWrite(bom, LOW);
}
}
}
