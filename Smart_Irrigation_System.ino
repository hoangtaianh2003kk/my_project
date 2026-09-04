#include <ESP8266WiFi.h>
#include <Arduino.h>
#include<ArduinoJson.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6ga7T9Egs"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "R-z12Y01NS1wwLtglwLfxzF5TxSdbhQs"
#define maybom D7
#define SDA D5
#define SCL D6
#define chedomaybom D3
#define dieukhienmaybom D1
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

int s1=0,s2=0,dem=0,level_mua;
const char* ssid = "PTIT_WIFI";  // wifi nhà ông
const char* password = ""; // 
String code_api="178087";
unsigned long previousMillis = 0; // Lưu thời gian trước đó
unsigned long now=0;
unsigned long previous1=0;
unsigned long p1=0;
unsigned long p2=0;
unsigned long p3=0,p4=0,p5=0;
unsigned long time1=5000;
int t1=0;
int a[10]={25,12,14,13,18,17,16,15,26,29}; // 4 CAI DAU NHỎ. 3 CAI SAU VUA, 3 CAI CUOI LON
int b[10]={25,12,39,40,18,41,42,15,26,29};
long interval = 0; // Thời gian cap nhat (1000 ms = 1 giây)
bool chedo=false;
bool mua=false;
bool mua3=false;
bool warning=false;
int icon1=0;
int icon2=0;

DHT dht(D2,DHT11);
LiquidCrystal_I2C lcd(0x27,16,2);
// URL của Google Apps Script
const char* host = "script.google.com";
const int httpsPort = 443;
const String GAS_URL = "/macros/s/AKfycbx6z-gDpoNL1ql21aZG21EeTueqf8OY061Nb3Y2h7gE8_4rC56AZ8nparoOivyRJciy/exec";

WiFiClientSecure client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  pinMode(chedomaybom,INPUT_PULLUP);
pinMode(dieukhienmaybom,INPUT_PULLUP);
pinMode(maybom,OUTPUT);
Blynk.begin("R-z12Y01NS1wwLtglwLfxzF5TxSdbhQs",ssid,password);
dht.begin();
Wire.begin(D5,D6); // SDA , SCL
lcd.begin(16,2);
lcd.backlight();
lcd.setContrast(100);

  Serial.print("Đang kết nối WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nKết nối WiFi thành công!");

  client.setInsecure(); // Bỏ qua chứng chỉ bảo mật (nếu muốn bảo mật hơn, cần cài chứng chỉ)
}
void chedo_bom(){
  if(digitalRead(chedomaybom)==LOW){
    lcd.clear();
  chedo=!chedo;
  if(chedo==false){
     Blynk.virtualWrite(V4, 0);
  }else{
     Blynk.virtualWrite(V4, 1);
  }
  while(digitalRead(chedomaybom)==LOW){
    delay(50);
  }
}
}
void bat_tat_bom(){
  if(digitalRead(dieukhienmaybom)==LOW){
    digitalWrite(maybom,!digitalRead(maybom));
    if(digitalRead(maybom)==HIGH){
         Blynk.virtualWrite(V3, 1);
    }else{
         Blynk.virtualWrite(V3, 0);
    }
    while(digitalRead(dieukhienmaybom)==LOW){
      delay(50);
    }
  }
}
void lcd_thucong(){
  if(digitalRead(maybom)==HIGH){
   lcd.setCursor(3,0);
  lcd.print("Dang Bat");
}else{
     lcd.setCursor(3,0);
  lcd.print("Dang Tat");
}
}
bool weather(){
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://dataservice.accuweather.com/forecasts/v1/daily/1day/"+ code_api+ "?apikey=trdQ8sKqvBkUJX1h2yGHiyGE477pAwuU")) {  // HTTP
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
             Serial.println(payload);
           DynamicJsonDocument doc(3056); 
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        }else{
          Serial.println("success........@!!!!!!!!!!!");
        }
           icon1 = doc["DailyForecasts"][0]["Day"]["Icon"];
           icon2 = doc["DailyForecasts"][0]["Night"]["Icon"];
           Serial.print("day la icon");
           Serial.print(icon1);
           Serial.println(icon2);
              bool mua1 = doc["DailyForecasts"][0]["Day"]["HasPrecipitation"];
              bool mua2 = doc["DailyForecasts"][0]["Night"]["HasPrecipitation"];
              if(mua1==true || mua2==true){
                mua=true;
                  Blynk.virtualWrite(V5, 1);
                  if(mua1==true){
                  for(int i=0;i<=9;i++){
                  if(a[i]==icon1){
                    s1=i;
                  }
                  }
                  }else{
                    s1=-1;
                  }
                  if(mua2==true){
                  for(int i=0;i<=9;i++){
                  if(b[i]==icon2){
                    s2=i;
                  }
                  }
                  }else{
                    s2=-1;
                  }
                  if(s1>s2){
                    s2=s1;
                  }
                  Serial.print("dya la  S1");
                  Serial.print(s1);
                  Serial.println(s2);
                switch(s2){
                  case 0: case 1: case 2: case 3:
                  level_mua=1; break;
                   case 4: case 5: case 6:
                   level_mua=2; break;
                    case 7: case 8: case 9:
                    level_mua=3; break;
                }
                Serial.print(level_mua);
              }else{
                mua=false;
                level_mua=0;
                  Blynk.virtualWrite(V5, 0);
              }

            return mua;
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.println("[HTTP] Unable to connect");
    }
  }
  return 0;}
void mh1(int nhietdo,int doam,int dat0,bool thoitiet){
lcd.setCursor(0,0);
lcd.print("T=");
lcd.setCursor(2,0);
lcd.print(nhietdo);
lcd.setCursor(5,0);
lcd.print("H=");
lcd.setCursor(7,0);
lcd.print(doam);
lcd.setCursor(10,0);
lcd.print("SWC=");
lcd.setCursor(14,0);
lcd.print(dat0);
if(dat0<10){
  lcd.setCursor(15,0);
  lcd.print(" ");
}
if(digitalRead(maybom)==HIGH){
 lcd.setCursor(0,1);
    lcd.print("ON ");
}else{
 lcd.setCursor(0,1);
    lcd.print("OFF");
}
if(thoitiet==true){
 switch(level_mua){
  case 1:
  lcd.print(" mua nho");
  break;
  case 2:
   lcd.print(" mua vua");
   break;
   case 3:
    lcd.print(" mua lon");
    break;
 }
}else{
  lcd.print(" Khong mua ");
}
}
BLYNK_WRITE(V4){
  chedo=!chedo;
  lcd.clear();
}
  BLYNK_WRITE(V3)
  {
    
      digitalWrite(maybom,!digitalRead(maybom));
  }
void loop() {
   Blynk.run();
  now=millis();
int nhietdo=dht.readTemperature();
int doam=dht.readHumidity();
int dat0=map(analogRead(A0),0,1023,400,0);
if(dat0>99){
  dat0=99;
}
if(nhietdo>100){
  doam=50;
  nhietdo=21;
}
if(now-p1>=5000){
Blynk.virtualWrite(V0, nhietdo);
Blynk.virtualWrite(V1, doam);
Blynk.virtualWrite(V2, dat0);
p1=now;
}
if(now-p4>=time1){
  t1=nhietdo;
  p4=now;
}
if(t1 - nhietdo > 5){
  warning=true;
  Serial.println("da warning");
  Serial.println(t1);
  Serial.println(nhietdo);
  Serial.println(t1-nhietdo);
  
}

chedo_bom();
if(now-previousMillis>=interval){
  //mua3=mua;
  weather();
  interval=86400000;
  previousMillis=now;
}
if(now-previous1>=60000){
if (client.connect(host, httpsPort)) {
    Serial.println("Kết nối đến Google Script thành công!");

    String url = GAS_URL + "?nhietdo=" + String(nhietdo) + "&doam=" + String(doam) + "&dat0=" + String(dat0);

    Serial.print("Đang gửi dữ liệu tới URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break;
    }

    String response = client.readString();
    Serial.println("Phản hồi từ server:");
    Serial.println(response);

  } else {
    Serial.println("Kết nối đến Google Script thất bại.");
  }

  client.stop();
previous1=now;
}
if(chedo==false){
  bat_tat_bom();
  lcd_thucong();
  warning=false;
  }else{
  mh1(nhietdo,doam,dat0,mua);
  if(now-p3>=1000){
    p3=now;
    if(warning==false){
if(mua==false ){
  if(dat0<10){
  digitalWrite(maybom,HIGH);
  Blynk.virtualWrite(V3, 1);}
  if(dat0>50){
    digitalWrite(maybom,LOW);
  Blynk.virtualWrite(V3, 0);
  }
}else{
  switch(level_mua){
    case 1:                            // MUA NHO 35%
    if(dat0<10){
  digitalWrite(maybom,HIGH);
  Blynk.virtualWrite(V3, 1);}
  if(dat0>35){
    digitalWrite(maybom,LOW);
  Blynk.virtualWrite(V3, 0);
  } break;
  case 2:                                   //MUA VUA 25%
  if(dat0<10){
  digitalWrite(maybom,HIGH);
  Blynk.virtualWrite(V3, 1);}
  if(dat0>25){
    digitalWrite(maybom,LOW);
  Blynk.virtualWrite(V3, 0);
} break;
 case 3:                                    // MUA LON 20%
  if(dat0<10){
  digitalWrite(maybom,HIGH);
  Blynk.virtualWrite(V3, 1);}
  if(dat0>20){
    digitalWrite(maybom,LOW);
  Blynk.virtualWrite(V3, 0);}
}
} }
}
  }

}
