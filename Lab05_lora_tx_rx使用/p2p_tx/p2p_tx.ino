
#include <LoRaWan.h>


unsigned char buffer[128] = {
  0xef,
  0xff,
  0x55,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
};


void setup(void) {
  SerialUSB.begin(115200);

  lora.init();

  lora.initP2PMode(433, SF12, BW125, 8, 8, 20); //應該是兩台機器要設定同個頻段(頻道)才可以互相傳輸
}

void loop(void) {
  // read the input on analog pin 4:
  int sensorValue = analogRead(A4); //讀取A4腳位並傳送
  // print out the value you read:
  Serial.println(sensorValue);
  char buf[30];
  sprintf(buf, "%d", sensorValue);
  lora.transferPacketP2PMode(buf);
  SerialUSB.println("Send string.");
  delay(3000);
  // lora.transferPacketP2PMode(buffer, 10);
  // SerialUSB.println("Send hex.");
  // delay(3000);
}
