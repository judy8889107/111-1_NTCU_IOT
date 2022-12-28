
#include <LoRaWan.h>

/*
ABP(activation by personalization) 開機即加入
ABP模式下，DevAddr、NwkSKey 與 AppSKey 是直接儲存在裝置上的
*/
unsigned char data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA,};
char buffer[256];


void setup(void)
{
  SerialUSB.begin(115200);
  while (!SerialUSB);

  lora.init();

  memset(buffer, 0, 256);
  lora.getVersion(buffer, 256, 1);
  SerialUSB.print(buffer);
  //輸出結果：
  //+VER: 2.0.10


  memset(buffer, 0, 256);
  lora.getId(buffer, 256, 1);
  //輸出結果：
  //+ID: DevAddr, 01:29:A1:7E
  //+ID: DevEui, 47:86:C5:8B:00:3C:00:2A
  //+ID: AppEui, 52:69:73:69:6E:67:48:46

  SerialUSB.print(buffer);
  SerialUSB.println("That was the id");
  // void setId(char *DevAddr, char *DevEUI, char *AppEUI);
  lora.setId("0129A17E", "4786C58B003C002A", "526973696E674846");
  //set的時候也會印出資訊
  //+ID: DevAddr, 01:29:A1:7E //裝置入網後，像是IP位置的概念
  //+ID: DevEui, 47:86:C5:8B:00:3C:00:2A
  //+ID: AppEui, 52:69:73:69:6E:67:48:46
  

  // setKey(char *NwkSKey, char *AppSKey, char *AppKey);
  //lora.setKey(NULL, NULL, "2B7E151628AED2A6ABF7158809CF4F3C");
  lora.setKey("2B7E151628AED2A6ABF7158809CF4F3C", "2B7E151628AED2A6ABF7158809CF4F3C", "2B7E151628AED2A6ABF7158809CF4F3C");
  //輸出結果：
  //+ KEY: NWKSKEY 2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C //終端裝置的network session key
  //+ KEY: APPSKEY 2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C //終端裝置的application session key
  //+ KEY: APPKEY 2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C  //終端裝置的application key
  
  lora.setDeciveMode(LWABP); //set的時候印出輸出前面會多一個+號
  lora.setDataRate(DR2, AS923);

  lora.setChannel(0, 922.7); //設定channel(老師應該都設定好了)
  lora.setChannel(1, 922.9);
  lora.setChannel(2, 923.2);
  lora.setChannel(3, 923.4);
  lora.setChannel(4, 923.7);
  lora.setChannel(5, 923.9);
  lora.setChannel(6, 924.2);
  lora.setChannel(7, 924.4);

  lora.setReceiceWindowFirst(0, 922.7);
  lora.setReceiceWindowSecond(923.2, DR2);
  lora.setAdaptiveDataRate(false);

  lora.setPower(14);
}

void loop(void)
{
  bool result = false;

  result = lora.transferPacket("1234567890", 10);
  //result = lora.transferPacket(data, 10, 10);
  
  //若傳送成功
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
