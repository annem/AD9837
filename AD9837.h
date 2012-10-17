/*
Arduino Library for Analog Devices AD9837 - Low Power Programmable Waveform Generator
go to http://www.analog.com/AD9837 for datasheet
License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free
to use and abuse this code however you'd like. If you find it useful
please attribute, and SHARE-ALIKE!
Created October 2012
by Anne Mahaffey - hosted on http://annem.github.com/AD9837
*/

#include "Arduino.h"

#ifndef AD9837_h
#define AD9837_h


class AD9837
{
public:

AD9837();

//
// Basic Device control functions
//
void begin(); 
long calcDDSTuningWord(long frequency);
void sineWave(long frequency);
void WriteRegister(int dat);

private:

};

#endif