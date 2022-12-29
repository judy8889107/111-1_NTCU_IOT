#include <LWiFi.h>
#include <PubSubClient.h>
#include <Grove_LED_Bar.h>

char ssid[] = "Chou";
char password[] = "ed061200";
char mqtt_server[] = "192.168.5.191";
char sub_topic[] = "distance";
char client_Id[] = "ACS108130";

int status = WL_IDLE_STATUS;

WiFiClient mtclient;
PubSubClient client(mtclient);
Grove_LED_Bar bar(4, 3, 0, LED_BAR_10); // Clock pin, Data pin, Orientation D3
#define RPIN 2                          // relay pin in D2

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
  // wait for serial port to connect. Needed for native USB port only
  //}
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //LED bar
  bar.begin();
  //Relay initial
  pinMode(RPIN, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}


void printWifiStatus() { //print Wifi status
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

void setup_wifi() {                       //setup Wifi
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros()); //也可以用randomSeed(analogRead(0));設定亂數種子
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void callback(char* topic, byte* payload, unsigned int length) {   //MQTT sub
  Serial.print("Input Message arrived [");
  Serial.print(sub_topic);
  Serial.print("]: ");
  String str = "";

  //receive data
  for (int i = 0; i < length; i++)
    str += (char)payload[i];

  
  int distance = str.toInt();
  Serial.println(distance);
  
  if (distance < 100) {
    digitalWrite(RPIN, HIGH);
  } else {
    digitalWrite(RPIN, LOW);
    if (distance == 800) {
      bar.setBits(0x3ff);
    }
    bar.setLevel(distance / 100);
  }



}

void reconnect() {  //reconnect MQTT
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = client_Id;
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
