#define USE_GPS 1

#include "LoRaWan.h"

#ifdef USE_GPS
#include "TinyGPS++.h"
TinyGPSPlus gps;
#endif
/*
#ifdef  宏名
    程序段1
#else
    程序段2
#endif
它的意思是，如果当前的宏已被定义过，则对“程序段1”进行编译，否则对“程序段2”进行编译。
*/

/*
TinyGPS++ 詳細使用說明：http://arduiniana.org/libraries/tinygpsplus/
*/
char data[200] = {'\0'};
void setup(void)
{

  char c;
#ifdef USE_GPS
  bool locked;
#endif

  SerialUSB.begin(115200);
  while (!SerialUSB);

  lora.init();
  lora.setDeviceReset();//reset裝置

#ifdef USE_GPS
  Serial.begin(9600);     // open the GPS
  locked = false;

  // For S&G, let's get the GPS fix now, before we start running arbitary
  // delays for the LoRa section

  while (!gps.location.isValid()) {
    while (Serial.available() > 0) {
      if (gps.encode(c = Serial.read())) {
        displayInfo();
        if (gps.location.isValid()) {
          //            locked = true;
          break;
        }
      }
      //        SerialUSB.print(c);
    }

    //      if (locked)
    //        break;

    //gps.charsProcessed(): 收到多少characters(the total number of characters received by the object)，失敗印出0
    //millis(): 若未在15秒內看見data，可能是裝置出錯(if we haven't seen lots of data in 15 seconds, something's wrong.)
    if (millis() > 15000 && gps.charsProcessed() < 10)
    {
      SerialUSB.println(F("No GPS detected: check wiring."));
      SerialUSB.println(gps.charsProcessed()); 
      while (true);
    }
    else if (millis() > 20000) {
      SerialUSB.println(F("Not able to get a fix in alloted time."));
      break;
    }
  }
#endif
}


void loop(void)
{
  displayInfo();
  
  delay(1000);
}

void displayInfo()
{
  
  double lat, lng;
  int month, day, year, hour, min, sec;
  if (gps.location.isValid())
  {
    lat = gps.location.lat(); //緯度
    lng = gps.location.lng(); //經度
  }
  else
  {
    Serial.print(F("Location INVALID"));
  }

  if (gps.date.isValid())
  {
    month = gps.date.month();
    day = gps.date.day();
    year = gps.date.year();


  }
  else
  {
    Serial.print(F("Date INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    hour = gps.time.hour();
    min = gps.time.minute();
    sec = gps.time.second();

  }
  else
  {
    Serial.print(F("Time INVALID"));
  }

  sprintf(data, "Location:%f,%f  Date/Time:%d/%d/%d %d:%d:%d", lat, lng, month, day, year, hour, min, sec);
  Serial.println(data);
}
