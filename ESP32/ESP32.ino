#include "DHT.h"
#include <BlynkSimpleEsp32.h>

//ESP32 PIN
#define sensorSuhu 5
#define kipas 23
#define sensorPintu 4
#define led 2
#define buzz 18
#define alarm 21

//Blynk Virtual PIN
#define blynk_tmpt V1
#define lcd_pin V2

//Inisiasi LCD pada Blynk
WidgetLCD lcd(lcd_pin);

//Konfigurasi Token Blynk, dan Jaringan WiFi
char auth[]="KWt6YDepcUpaQnigGsX9GT2bC_hHV49s";
char ssid[]="MSI_173";
char pass[]="123456789";

//Konfigurasi Sensor DHT11
#define tipeSensor DHT11
DHT dht(sensorSuhu, tipeSensor);

bool state;
bool alert;
bool stateChange;
String doorState;

void setup() {
  Serial.begin(115200);
  
  //Inisiasi Blynk
  Blynk.begin(auth, ssid, pass);
  
  //Mode Alarm
  pinMode(alarm, INPUT);

  //Door Sensor
  pinMode(sensorPintu, INPUT_PULLUP);
  stateChange = 0;

  //LED pada ESP32
  pinMode(led, OUTPUT);

  //Buzzer
  pinMode(buzz, OUTPUT);

  //Kipas
  pinMode(kipas,OUTPUT);
  digitalWrite(kipas,1);

  //Inisiasi DHT11
  dht.begin();
}

void loop() {
  //Menjalankan Blynk
  Blynk.run();
  
  //Membaca Kondisi Pintu
  state = digitalRead(sensorPintu);
  
  //Membaca Mode Alarm
  alert = digitalRead(alarm);

  //Membaca Perubahan Kondisi Pintu
  if(stateChange != state) {
    lcd.clear();
    stateChange = state;
  }

  //Membaca Kondisi Pintu
  if(state) {
      doorState = "Pintu Tertutup";
    } else {
      doorState = "Pintu Terbuka";
    }

  //Print Kondisi Pintu Pada LCD Blynk
  lcd.print(0, 0, doorState);

  //Kondisi Alarm Saat Menyala
  if(alert == HIGH){
    if(state) {
      digitalWrite(buzz, LOW);
    } else {
      digitalWrite(buzz, HIGH);
    }
  }

  //Menyalakan LED Pada ESP32
  digitalWrite(led, !state);
  
  //Membaca Temperatur Ruangan
  float suhuC = dht.readTemperature();

  //Menampilkan Suhu Pada Blynk
  Blynk.virtualWrite(blynk_tmpt, suhuC);

  //Kondisi Kipas Menyala
  if (suhuC>=28){
    digitalWrite(kipas, 0);
  }else if (suhuC<= 25) {
    digitalWrite(kipas, 1);
  }
  
  delay(500);
}
