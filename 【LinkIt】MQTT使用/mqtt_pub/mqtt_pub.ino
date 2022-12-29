#include <LWiFi.h>
#include <PubSubClient.h>
#include "Ultrasonic.h"

#define WIFI_SSID "Chou"
#define WIFI_PASSWORD "ed061200"

#define MQTT_SERVER_IP "192.168.5.191"
#define MQTT_SERVER_PORT 1883
#define MQTT_TOPIC "distance"
#define MQTT_CLIENT_ID "ACS108130"

// http://man.hubwiz.com/docset/Arduino.docset/Contents/Resources/Documents/www.arduino.cc/en/Reference/WiFiStatus.html
// Wifi 狀態對照表
int status = WL_IDLE_STATUS;
//WL_IDLE_STATUS:調用WiFi.begin()時分配到的臨時狀態值，保持直到: WL_CONNECT_FAIL或WL_CONNECTED
WiFiClient client;
PubSubClient upload(client);

Ultrasonic ultrasonic(2);// pin D2

void reconnect()
{
  // Loop until we're reconnected
  while (!upload.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (upload.connect(MQTT_CLIENT_ID))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(upload.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

  upload.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  delay(1500);
}

void loop()
{
  if (!upload.connected())
  {
    reconnect();
  }
  else
  {
    // send distance
    String payload = String(ultrasonic.MeasureInCentimeters());

    // publish to MQTT Broker
    // c_str():函数返回一个指向正规C字符串的指针, 内容与本string串相同
    /*C++中 c_str( )主要用法就是这是为了与c语言兼容，在c语言中没有string类型，故必须通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式。
      c_str() 以 char* 形式传回 string 内含字符串。

      char c[20];
      string s="1234";
      strcpy(c,s.c_str());

      这样才不会出错，c_str()返回的是一个临时指针，不能对其进行操作*/

    if (upload.publish(MQTT_TOPIC, payload.c_str()))
    {
      Serial.println("Message => " + payload + " has been sent to " + MQTT_SERVER_IP + ".");
    }
    delay(100);
  }

  upload.loop();
  /*loop():
    它允許客戶端執行任何保持活動的處理，例如發送ping數據包，以確保連接保持活動狀態。默認的 keepalive 是 15 秒，所以客戶端需要在這種時間範圍內做一些事情。
    它檢查是否有任何傳入數據要從網絡讀取 - 例如已發布的消息。
    如果您的客戶端只是發布，那麼您可以loop每隔幾秒調用一次以確保運行保持活動處理。
    如果您的客戶已訂閱並希望收到消息，那麼您應該決定在發布消息和触發客戶端回調之間可以處理多長時間的延遲。*/
    
  delay(2000);
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
