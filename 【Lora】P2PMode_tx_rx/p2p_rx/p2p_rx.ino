
#include <LoRaWan.h>


unsigned char buffer[128] = {0, };


void setup(void)
{
    SerialUSB.begin(115200);
    
    lora.init();
    
    lora.initP2PMode(433, SF12, BW125, 8, 8, 20); //應該是兩台機器要設定同個頻段(頻道)才可以互相傳輸
}

void loop(void)
{
    short length = 0;
    short rssi = 0; //received signal strength(接收訊號強度)
    
    memset(buffer, 0, 128);
    length = lora.receivePacketP2PMode(buffer, 128,  &rssi, 1);
    
    if(length)
    {
        // SerialUSB.print("Length is: ");
        // SerialUSB.println(length);
        // SerialUSB.print("RSSI is: ");
        // SerialUSB.println(rssi);
        SerialUSB.print("Data is: ");
        for(unsigned char i = 0; i < length; i ++)
        {
            // SerialUSB.print("0x");
            SerialUSB.print((char)buffer[i]);
            // SerialUSB.print(" ");
        }
        SerialUSB.println();
    }
}
