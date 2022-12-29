
//lora溫、濕度感應器

#include <LoRaWan.h> //系統自帶頭文件，編譯時會從系統目錄底下開始找
#include "DHT.h"     //用戶自訂頭文件，編譯時會從用戶目錄底下開始尋找

#define DHTPIN            2
#define DHTTYPE           DHT22
//溫溼度感應器的sensor有兩種版本(DHT11、DHT22)，這邊用的是DHT22
//因為接lora，GND、5V、pin腳位D2、白色線不用接

DHT dht(DHTPIN, DHTTYPE);
unsigned char data[2] = {1, 2};
char buffer[256];

void setup(void)
{
  dht.begin();

  SerialUSB.begin(115200);
  //while(!SerialUSB);

  lora.init();

  memset(buffer, 0, 256);
  lora.getVersion(buffer, 256, 1);
  SerialUSB.print(buffer);

  memset(buffer, 0, 256);
  lora.getId(buffer, 256, 1);
  //不用lora.setID()
  SerialUSB.print(buffer);

  lora.setKey("2B7E151628AED2A6ABF7158809CF4F3C", "2B7E151628AED2A6ABF7158809CF4F3C", "2B7E151628AED2A6ABF7158809CF4F3C");
  //固定的key
  lora.setDeciveMode(LWABP);
  lora.setDataRate(DR0, US915);

  lora.setChannel(0, 922.7);
  lora.setChannel(1, 922.9);
  lora.setChannel(2, 923.2);
  lora.setChannel(3, 923.4);
  lora.setChannel(4, 923.7);
  lora.setChannel(5, 923.9);
  lora.setChannel(6, 924.2);
  lora.setChannel(7, 924.4);

  lora.setReceiceWindowFirst(0, 922.7);
  lora.setReceiceWindowSecond(923.2, DR2);

  //lora.setDutyCycle(false);
  //lora.setJoinDutyCycle(false);

  lora.setPower(14);
}

void loop(void)
{
  bool result = false;

  delay(2000);
  int h = dht.readHumidity();
  int t = dht.readTemperature();

  SerialUSB.print("Humidity =");
  SerialUSB.println(h);
  SerialUSB.print("Temperature=");
  SerialUSB.println(t);
  //result = lora.transferPacket("Hello World!", 10);
  data[0] = h;
  data[1] = t;
  result = lora.transferPacket(data, 2, 10);

  if (result)
  {
    short length;
    short rssi;

    memset(buffer, 0, 256);
    length = lora.receivePacket(buffer, 256, &rssi);

    if (length)
    {
      SerialUSB.print("Length is: ");
      SerialUSB.println(length);
      SerialUSB.print("RSSI is: ");
      SerialUSB.println(rssi);
      SerialUSB.print("Data is: ");
      for (unsigned char i = 0; i < length; i ++)
      {
        SerialUSB.print("0x");
        SerialUSB.print(buffer[i], HEX);
        SerialUSB.print(" ");
      }
      SerialUSB.println();
    }
  }
}
