#include <LWiFi.h>
#include "MCS.h"
#include "LDHT.h"

char _lwifi_ssid[] = "EnvyWifi";
char _lwifi_pass[] = "048889107";


#define DHTPIN 2          // what pin we're connected to
#define DHTTYPE DHT22     // using DHT22 sensor
#define RPIN 3            // relay pin in D3
LDHT dht(DHTPIN, DHTTYPE);
float tempC = 0.0;

//MCSLiteDevice mcs("BkstUW3uj", "a6aaf0e8a6aaf0e80bd4add50bd4add5062d81c5a6aaf0e8aa97751d0bd4add5", "210.240.193.178", 3000);
MCSLiteDevice mcs("BkstUW3uj", "a6aaf0e8a6aaf0e80bd4add50bd4add5062d81c5a6aaf0e8aa97751d0bd4add5", "25.17.204.111", 3000);
MCSDisplayFloat temp("temp");
MCSDisplayGPIO relay("relay"); 
void setup() {
  Serial.begin(9600);
  mcs.addChannel(temp);
  mcs.addChannel(relay);

  Serial.println("Wi-Fi Start Connecting...");
  //http://man.hubwiz.com/docset/Arduino.docset/Contents/Resources/Documents/www.arduino.cc/en/Reference/WiFiStatus.html (Wifi狀態對照表)
  while (WiFi.begin(_lwifi_ssid, _lwifi_pass) != WL_CONNECTED) { //若沒連上，則delay 1秒後重連接
    delay(1000);
  }
  Serial.println("Wi-Fi Connect Success");
  mcs.connect();//連接mcs
  while (!mcs.connected()) {
    Serial.println("MCS connect error!!");
    mcs.connect();
  }
  Serial.println("MCS Connect Success");


  //Sensor initial
  dht.begin();

  //Relay initial
  pinMode(RPIN, OUTPUT);

}

void loop() {

  //Read temp
  if (dht.read()) {
    tempC = dht.readTemperature();
  }

  //MCS Reconnect
  while (!mcs.connected()) {
    mcs.connect();
    if (mcs.connected()) {
      Serial.println("MCS Reconnect");
    }
  }
  temp.set(tempC);
  Serial.print("temp value : ");
  Serial.println(tempC);
  Serial.println(relay.value());

  //Turn relay on/off
  if (tempC > 20) {
    relay.set(1);
  }
  else {
    relay.set(0);
  }
  digitalWrite(RPIN, relay.value()?HIGH:LOW);
  delay(1000);

}
