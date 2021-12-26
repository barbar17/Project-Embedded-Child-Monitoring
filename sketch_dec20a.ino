#include "DHT.h"

#define sensorSuhu 5
#define kipas 23
#define tipeSensor DHT11
#define sensorPintu 4
#define led 2
#define buzz 18

DHT dht(sensorSuhu, tipeSensor);

bool state;
String doorState;

void setup() {
  Serial.begin(115200);

  //Door Sensor
  pinMode(sensorPintu, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(buzz, OUTPUT);

  //Kipas
  pinMode(kipas,OUTPUT);
  digitalWrite(kipas,1);
  Serial.println(F("Percobaan menggunakan sensor suhu dan kelembapan!"));
  dht.begin();
}

void loop() {
  //Door Sensor
  state = digitalRead(sensorPintu);
  if(state) {
    doorState = "DOOR_CLOSED";
    digitalWrite(buzz, LOW);
  } else {
    doorState = "DOOR_OPEN";
    digitalWrite(buzz, HIGH);
  }
  digitalWrite(led, !state);
  Serial.println(doorState);

  //Kipas
  float suhuC = dht.readTemperature();

  if(isnan(suhuC)){
    Serial.print(F("Sensor DHT11 tidak terbaca!"));
    return;
  }
  
  Serial.print(F("% Suhu : "));
  Serial.print(suhuC);
  Serial.println(F(" C "));
  Serial.println("_________________");

  if (suhuC>=28){
    digitalWrite(kipas, 0);
  }else if (suhuC<= 25) {
    digitalWrite(kipas, 1);
  }
  
  delay(500);
}
