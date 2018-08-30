#include <SPI.h>

// PINS

const int SYN= 8;
const int SCS1=7;
const int EN=6;

//VARS

byte tabByte[4];

//INITIALISATION

void init(int EN,int SYN,int SCS1){
digitalWrite(EN,LOW);
digitalWrite(SYN, HIGH);
digitalWrite(SCS1, LOW);
delay(10);
digitalWrite(EN,HIGH);
delay(5);
digitalWrite(SCS1,HIGH);
}

//RESETTING

void reset(int SYN, int SCS1){
  delay(30);

  for(int i=0;i<3;i++)
  {
    digitalWrite(SYN,LOW);
    delay(1);
    digitalWrite(SYN,HIGH);
    delay(1);
  }
  digitalWrite(SCS1,LOW);
  delay(1);
  digitalWrite(SCS1,HIGH);
}

//SENDING BUFFER CONTENT

void sendBufferContent(){
//knowing that it returns the LSByte first
for (int i=0;i<5;i++){
  tabByte[i]=SPI.transfer(0xFF);
  Serial.println(tabByte[i]);
  }
 Serial.println("-------------");
 delay(2000);

}

// CONVERTING 4 BYTES TO CORRESPONDING VALUE

void convertBufferContent(){
  //knowing that it returns the LSByte first
  long total=tabByte[0]+tabByte[1]*256+tabByte[2]*256*256+tabByte[3]*256*256*256;
  Serial.println("Buffer.s value is:");
  Serial.println(total);
//  return total;
}

// Configuration _ Reg DSP_CR3
void configDSP_CR3(){

  Serial.println("config DSP_CR3 en cours ");
  SPI.transfer(0xFF); //read nothing
  SPI.transfer(0x05); //write in DSPCR3
  SPI.transfer(0x80); //autolatch every 128 milli secs (?)
  SPI.transfer(0x00);
  SPI.transfer(0xA7);
  

  SPI.transfer(0x04); //read DSPCR3
  SPI.transfer(0xFF); //write nothing
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0x57);

  Serial.println("DSP_CR3: ");
  sendBufferContent();
  Serial.println("config DSP_CR3 done \n --------------- ");
}


// Configuration _ Reg DSP_CR1
void configDSP_CR1(){
SPI.transfer(0xFF); //read nothing
SPI.transfer(0x00); //write in adress 00: LSWord in reg0
SPI.transfer(0xA0); //See Reg map
SPI.transfer(0x00); //See Reg map
SPI.transfer(0xC9); //crc

SPI.transfer(0xFF); //read nothing
SPI.transfer(0x01); //write in adress 01: MSWord in reg0
SPI.transfer(0x1E); //See Reg map
SPI.transfer(0x04); //See Reg map
SPI.transfer(0x27); //crc
}


//Reading registers

void readActivePowerReg(){
  SPI.transfer(0x5C); //active power reg
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0x1B);
  sendBufferContent();
  convertBufferContent();
}

void readTotalActiveEnergyReg(){
  SPI.transfer(0x84); //active power reg
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0x66);
  sendBufferContent();
  convertBufferContent();
}

void readMomentActivePowerReg(){
  SPI.transfer(0x68); //moment power reg
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xEA);
  sendBufferContent();
  convertBufferContent();
}

void readC1DataReg(){
  SPI.transfer(0x32); //c1 reg
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0x8A);
  sendBufferContent();
  convertBufferContent();
}



//SETUP

void setup() {

Serial.begin(9600);

// PINS
pinMode(SCS1, OUTPUT);
pinMode(SYN,OUTPUT);
pinMode(EN, OUTPUT);

//initialisation
init(EN,SYN,SCS1);

//Resetting DSP
//reset(SYN,SCS1);

digitalWrite(SCS1,LOW);

// REGISTER CONFIGURATION
SPI.begin();
SPI.beginTransaction (SPISettings(9600,MSBFIRST,SPI_MODE3)); //STMP3x getting started with STPM34 page 13

configDSP_CR1();
configDSP_CR3();
SPI.endTransaction();
SPI.end;
}


//SPI CONFIG AND DATA TRANSFER

void loop() {

// Note that for our personnal needs, DSP was initialized outside of the loop   

//long t=micros();

SPI.begin();
SPI.beginTransaction (SPISettings(9600,MSBFIRST,SPI_MODE3)); //STMP3x getting started with STPM34 page 13

//Serial.println("New loop");
Serial.println("Active Power channel 1: ");
readActivePowerReg();

Serial.println("Total Active Energy channel 1: ");
readTotalActiveEnergyReg();

Serial.println("Momentary power channel 1: ");
readMomentActivePowerReg();

Serial.println("c1 data reg : ");
readC1DataReg();

SPI.endTransaction();
SPI.end;


//long t1=micros()-t;
//Serial.println(t1);
}
