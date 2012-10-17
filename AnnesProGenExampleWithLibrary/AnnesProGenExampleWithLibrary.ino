/*
AD9837 Pro Generator sample code
This was written in Arduino 1.0.1,
for an Arduino Pro Mini, 5V, 16MHz

Anne Mahaffey - October 2012

The connections to the AD9837 board are:

FSYNC -> 2 
SCLK -> 13 (SCK)
SDATA -> 11 (MOSI)
+Vin = VCC on Pro Micro
GND -> GND

This code bears the license of the beer. If you make money off of this,
you gotta beer me.
*/

#include <SPI.h>
#include <AD9837.h>
AD9837 ProGen;

void setup()
{  
  long freqTuningWord;

  Serial.begin(9600);                               // start serial communication at 9600bps
  ProGen.begin();                                   // initial setup (SPI, pins, etc)
  freqTuningWord = ProGen.calcDDSTuningWord(17355); // 17.355 kHz
  ProGen.sineWave(freqTuningWord);                  // Sine Wave output
}

void loop()
{
 
}
