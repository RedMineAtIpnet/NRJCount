#include <FreqCount.h>
unsigned long count=0;
unsigned long prior_count=0;
#include <EEPROM.h>
#include <cc1101.h>
CC1101 cc1101;
#define LEDOUTPUT 13
byte counter;
byte b;
byte syncWord= 199;
unsigned long  c;

void blinker(){
  digitalWrite (LEDOUTPUT, HIGH);
  delay(100);
  digitalWrite(LEDOUTPUT, LOW);
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(57600);
FreqCount.begin(200);
Serial.println("start");

pinMode(LEDOUTPUT, OUTPUT);
digitalWrite(LEDOUTPUT, LOW);

counter=0;
Serial.println("initializing..");
cc1101.init();
cc1101.setSyncWord(&syncWord, false);
cc1101.setCarrierFreq(CFREQ_868);
cc1101.disableAddressCheck();
cc1101.setChannel(1);
cc1101.setDevAddress(1); 

delay(1000);

Serial.print("CC1101_PARTNUM");
Serial.println(cc1101.readReg(CC1101_PARTNUM, CC1101_STATUS_REGISTER));
Serial.print("CC1101_VERSION");
Serial.println(cc1101.readReg(CC1101_VERSION, CC1101_STATUS_REGISTER));
Serial.print("CC1101_MARCSTATE");
Serial.println(cc1101.readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER));
Serial.println("device initialized");
}

void send_data(){
  CCPACKET data;
  data.length=8;
  //byte blinkCount=counter++;
  data.data[0]=c&0x000000ff;
  data.data[1]=c&0x0000ff00;
  data.data[2]=c&0x00ff0000;
  data.data[3]=c&0xff000000;
  Serial.println("CC1101_MARCSTATE");
  Serial.println(cc1101.readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER) & 0x1f);
  if(cc1101.sendData(data)){
    //Serial.println(blinkCount,HEX);
    Serial.println(data.data[0]);
    Serial.println(data.data[1]);
    Serial.println(data.data[2]);
    Serial.println(data.data[3]);
    //Serial.println(data.data[4]);
    Serial.println("sent ok :) ");
    blinker();
    }
    else {
      Serial.println("sent failed :(");
      blinker();
      blinker();
    }
  }
void loop() {
  // put your main code here, to run repeatedly:
  if(FreqCount.available()){
    c=FreqCount.read();
  count=count+c;
  
  // if (count !=prior_count){
 Serial.println(count);
  send_data();
   prior_count=count;
// }
}

//Serial.println(millis());
}
