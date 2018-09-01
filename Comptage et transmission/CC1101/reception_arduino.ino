#include <EEPROM.h>
#include <cc1101.h>
CC1101 cc1101;
#define LEDOUTPUT 4
byte counter;
byte b;
byte syncWord= 199;
unsigned long  c;
boolean packetAvailable = false;

void blinker(){
  digitalWrite (LEDOUTPUT, HIGH);
  delay(100);
  digitalWrite(LEDOUTPUT, LOW);
  delay(100);
}
void cc1101signalsInterrupt(void){
  packetAvailable = true;
}
void setup() {
Serial.begin(57600);
Serial.println("start");

pinMode(LEDOUTPUT, OUTPUT);
digitalWrite(LEDOUTPUT, LOW);
cc1101.init();
cc1101.setSyncWord(&syncWord, false);
cc1101.setCarrierFreq(CFREQ_433);
cc1101.disableAddressCheck();
Serial.print("CC1101_PARTNUM");
Serial.println(cc1101.readReg(CC1101_PARTNUM, CC1101_STATUS_REGISTER));
Serial.print("CC1101_VERSION");
Serial.println(cc1101.readReg(CC1101_VERSION, CC1101_STATUS_REGISTER));
Serial.print("CC1101_MARCSTATE");
Serial.println(cc1101.readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER));
attachInterrupt(0,cc1101signalsInterrupt,FALLING);
Serial.println("device initialized");
}
void ReadLQI()
{
  byte lqi =0;
  byte value =0;
  lqi=(cc1101.readReg(CC1101_LQI,CC1101_STATUS_REGISTER));
  value =0x3F - (lqi & 0x3F);
  Serial.print("CC1101_LQI");
  Serial.println(value);
}
void ReadRSSI()
{
 byte rssi = 0;
 byte value =0;
 rssi=(cc1101.readReg(CC1101_LQI,CC1101_STATUS_REGISTER));
 if(rssi>=128)
 {
  value=255-rssi;
  value/=2;
  value+=74;
  }
  else
  {
    value=rssi/2;
    value+=74;
}
Serial.print("CC1101_RSSI");
Serial.print(value);
}
void loop() {
  // put your main code here, to run repeatedly:
if(packetAvailable){
  Serial.print("packet recieved");
  detachInterrupt(0);
  ReadRSSI();
  ReadLQI();
  packetAvailable=false;
  CCPACKET packet;
  if(cc1101.receiveData(&packet)>0)
  {if(!packet.crc_ok)
  {Serial.println("crcnot ok");
    }
    if(packet.length>0)
    {Serial.print("packet : len");
    Serial.print(packet.length);
    Serial.print("data:");
    for(int j=0;j<packet.length;j++)
    {Serial.print(packet.data[j],HEX);
    Serial.print(" ");
      }
     Serial.println(".");
    }
    
  }
  attachInterrupt(0,cc1101signalsInterrupt,FALLING);
}
}
