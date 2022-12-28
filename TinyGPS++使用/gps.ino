#define USE_GPS 1

#include "LoRaWan.h"

#ifdef USE_GPS
#include "TinyGPS++.h"
TinyGPSPlus gps;
#endif

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
  lora.setDeviceReset();

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
    lat = gps.location.lat();
    lng = gps.location.lng();
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
    sec = gps.time.centisecond();

  }
  else
  {
    Serial.print(F("Time INVALID"));
  }

  sprintf(data, "Location:%f,%f  Date/Time:%d/%d/%d %d:%d:%d", lat, lng, month, day, year, hour, min, sec);
  Serial.println(data);
}
