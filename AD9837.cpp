/*
Arduino Library for Analog Devices AD9837 - Low Power Programmable Waveform Generator
go to http://www.analog.com/AD9837 for datasheet
License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free
to use and abuse this code however you'd like. If you find it useful
please attribute, and SHARE-ALIKE!
Created October 2012
by Anne Mahaffey - hosted on http://annem.github.com/AD9837
*/

#include <Arduino.h>
#include <AD9837.h>
#include <SPI.h>

const int slaveSelectPin = 2;  // Called FSYNC in datasheet
const bool debugSerial = 1;

AD9837::AD9837() {

}

void AD9837::begin() {
  pinMode(slaveSelectPin, OUTPUT);
  SPI.setDataMode(SPI_MODE2);	
  SPI.begin();
  delay(100);
  if (debugSerial) {Serial.println("Begin...\n"); }
 }
 
long AD9837::calcDDSTuningWord(long frequency) {
 //We can't just send the actual frequency, we have to calculate the "frequency word".
  //This amounts to ((desired frequency)/(reference frequency)) x 0x10000000.
  //calculated_freq_word will hold the calculated result.
long FTW; 

float refFreq = 16000000;         // Using MHz Crystal for AD9837 Clk Input (24bits)
float FTWResolution = 0x10000000;

float stepSize;

if (debugSerial) {
    Serial.println("Calculating FTW........");
	Serial.print("   frequency = "); 
	Serial.println(frequency); 
	Serial.print("   refFreq = "); 
	Serial.println(refFreq); 
    Serial.print("   FTWResolution = "); 
	Serial.println(FTWResolution); 

}

stepSize = FTWResolution/refFreq;
FTW = stepSize*frequency;
 
  if (debugSerial) {
  	Serial.print("   StepSize = "); 
    Serial.println(stepSize);
	Serial.print("   FTW hex = "); 
	Serial.println(FTW, HEX);
	Serial.print("   FTW = "); 
    Serial.println(FTW);	
	}
return FTW;
}

 
void AD9837::sineWave(long FTW)
{

  if (debugSerial) {
	Serial.print("\nSetup AD9837 for Sine Wave, FTW is "); 
	Serial.print(FTW);
	Serial.println(".......");
  }
  int MSB;
  int LSB;
  int phase = 0;
  
  
  //Once we've got that, we split it up into separate bytes.
  MSB = (int)((FTW & 0xFFFC000)>>14); //14 bits
  LSB = (int)(FTW & 0x3FFF);
  
  //Set control bits DB15 ande DB14 to 0 and one, respectively, for frequency register 0
  LSB |= 0x4000;
  MSB |= 0x4000; 
  phase &= 0xC000;
  
  WriteRegister(0x2100);   delay(500);
  WriteRegister(LSB);  //lower 14 bits
  WriteRegister(MSB);  //upper 14 bits
  WriteRegister(phase);  //mid-low
  //Power it back up
  //AD9837Write(0x2020); //square
  WriteRegister(0x2000); //sin
  //AD9837Write(0x2002); //triangle
  


}

//This is the guy that does the actual talking to the AD9837
void AD9837::WriteRegister(int dat)
{  
  digitalWrite(slaveSelectPin, LOW);  //Set FSYNC low
  delay(10);
  
  SPI.transfer(highByte(dat)); 
  SPI.transfer(lowByte(dat));  
  
  if (debugSerial) {
	Serial.print("      Writing SPI - hex "); 
	Serial.println(dat, HEX);
  }
  
  delay(10);
  digitalWrite(slaveSelectPin, HIGH);  //Set FSYNC high
}
