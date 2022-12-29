#include <LWiFi.h> 
#include "MCS.h" 
char _lwifi_ssid[] ="ssid";
char _lwifi_pass[] ="key";
MCSLiteDevice mcs("DeviceId", "DeviceKey", "MCS's IP Address", 3000);
MCSControllerOnOff myswitch("myswitch");
void setup() { 
  Serial.begin(9600); 
  mcs.addChannel(myswitch); 
  Serial.println("Wi-Fi Start Connecting..."); 
  while (WiFi.begin(_lwifi_ssid, _lwifi_pass) != WL_CONNECTED) { delay(1000); } 
  Serial.println("Wi-Fi Connect Success"); 
  while(!mcs.connected()) { mcs.connect(); } 
  Serial.println("MCS Connect Success"); 
  Serial.begin(9600); 
  pinMode(LED_BUILTIN, OUTPUT); //內置的LED燈
} 
void loop() { 
  while (!mcs.connected()) { 
    mcs.connect(); 
    if (mcs.connected()) { Serial.println("MCS Reconnect"); } 
  } 
  mcs.process(100);  //MCS timeout設定：void process(int timeout_ms=0)
  if (myswitch.updated()) { //當MCS控制面板改變開關值
    Serial.print("Channel Update :"); 
    Serial.println(myswitch.value()); //讀取值並讓燈泡亮/暗
    if (myswitch.value()) { 
      digitalWrite(LED_BUILTIN, HIGH); 
    } else { 
      digitalWrite(LED_BUILTIN, LOW); 
    } } 
  delay(1000); 
}
