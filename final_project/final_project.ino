#include <LWiFi.h>
#include <PubSubClient.h>

#include "LDHT.h"

//#define WIFI_SSID "MouseLaoJi_ap"
//#define WIFI_PASSWORD "12345678"

#define WIFI_SSID "FAST_9C8D52"
#define WIFI_PASSWORD "048889107"

//#define WIFI_SSID "loraap"
//#define WIFI_PASSWORD "1234567890"

#define MQTT_SERVER_IP "mqtt3.thingspeak.com"  //192.168.5.191
#define MQTT_SERVER_PORT 1883
#define DHTPIN 2
#define DHTTYPE DHT22
#define MAXTEMP 80

LDHT dht(DHTPIN, DHTTYPE);
float tempC = 0.0;

const char MQTT_TOPIC[] =  "channels/1766228/publish";
const char sub_topic[] = "channels/1766228/subscribe/fields/field2";
const char clientID[] = "KAwSMyYdLwQiBiMyFRwkCz0";
const char userName[] = "KAwSMyYdLwQiBiMyFRwkCz0";
const char passwd[] = "PBYVhBJPMy+KrZEJKX9oq/N+";

int status = WL_IDLE_STATUS;

int turn_on_LED = 1;
WiFiClient client;
PubSubClient upload(client);


// 重連 function
void reconnect()
{

  while (!upload.connected())
  {
    Serial.print("嘗試連接 MQTT...");
    // 以clientID, username, passwd 建立連線
    if (upload.connect(clientID, userName, passwd))
    {
      Serial.println("MQTT 連接成功!");
      upload.subscribe(sub_topic); //訂閱灑水器開關主題
    }
    else
    {
      Serial.print("MQTT 連接失敗, rc=");
      Serial.print(upload.state());
      Serial.println(" 5秒後重試...");
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
    Serial.print("嘗試連接 SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
  Serial.println("連接WiFi成功!");
  printWifiStatus();

  //dht sensor 啟動
  dht.begin();
  tempC = dht.readTemperature();
  Serial.println("啟動DHT sensor...當前溫度為: " + (String)tempC);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // 關閉LED

  upload.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  upload.setCallback(callback); // 設定callBack function

  delay(1500);
}

//call back函數
void callback(char* topic, byte* payload, unsigned int length) {   //MQTT sub
  //  Serial.print("Input Message arrived [");
  //  Serial.print(sub_topic);
  //  Serial.print("]: ");
  String str = "";

  //串接資料
  for (int i = 0; i < length; i++)
    str += (char)payload[i];

  // 接收灑水器 field 訊息
  turn_on_LED = str.toInt();
  //  Serial.println("sub訊息: " + (String)turn_on_LED);
  if (turn_on_LED) {
    Serial.println("已關閉灑水器");
  }
  digitalWrite(LED_BUILTIN, turn_on_LED ? HIGH : LOW);

}

void loop()
{

  if (!upload.connected())
  {
    reconnect();
  }


  else
  {

    float now_temp = dht.readTemperature();
    Serial.println("當前真實溫度為: " + (String)now_temp);

    // 監控視窗輸入溫度進行測試
    String input = "";
    while (Serial.available() > 0) {
      input += (char)Serial.read();
    }
    if (input != "") {
      Serial.print("輸入溫度: " + input);
      now_temp = input.toFloat();
    }

    // 建立 payload
    String payload = "field1=" + ((String)now_temp);


    // 若溫度超過 MAXTEMP 度 (sensor偵測範圍：-40~80C), MAXTEMP預設80度
    if (now_temp >= MAXTEMP) {
      Serial.println("超過設定最高溫度，要火災了!!");
      payload += "&status=超過設定最高溫度，要火災了!!";

      // 判斷是否要將灑水器打開(因初始值為0，所以到達80度時，灑水器一開始會被打開)
      if (turn_on_LED) {
        payload += "&field2=1";
      } else { // 若使用者將灑水器關閉
        payload += "&field2=0";
      }
      digitalWrite(LED_BUILTIN, turn_on_LED ? HIGH : LOW);

    } else { //若溫度未超過MAXTEMP
      // 判斷溫度是否有持續升高，並更新status
      if (now_temp - tempC > 0) {
        payload += "&status=溫度持續升高中...";
        tempC = now_temp;
      }
      else {
        payload += "&status=溫度維持不變";
      }

    }



    // publish 資料到 MQTT
    if (upload.publish(MQTT_TOPIC, payload.c_str()))
    {
      Serial.println(payload + " 成功傳送!");
    }
    delay(100);
  }

  upload.loop();
  delay(10000); // 10 秒更新一次
}

// WiFi 狀態
void printWifiStatus()
{
  // 印出SSID
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // 印出 IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // 印出 RSSI
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
