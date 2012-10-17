/*
AD9837 Pro Generator sample code
This was written in Arduino 1.0.1,
for an Arduino Pro Mini, 5V, 16MHz
Pete Dokter, 9/2/12

With this code running, open up a teminal window at 9600 baud,
enter your desired frequency followed by <enter>. The code translates
the ascii characters and sets the frequency on the AD9837.

The connections to the AD9837 board are:

FSYNC -> 2 
SCLK -> 13 (SCK)
SDATA -> 11 (MOSI)

+Vin = VCC on Pro Micro
GND -> GND

This code bears the license of the beer. If you make money off of this,
you gotta beer me.
*/

//Define the SPI lines. These are standard pins,
//but you can move them around by changing the numbers
#define FSYNC 2
#define SDATA 11
#define SCLK 13

long freq;  //32-bit global frequency variable

//The write sequence for the AD9835 happens via SPI in 2 byte chunks.
//Generally, the first byte is a command or address, the second
//is data to be written. It's bit-banged here
void AD9837Write(char cmd, char dat);

//To set the frequency, you need to write to the AD9835 six times. This function does that.
void set_freq(long frequency);


void setup()
{
  pinMode(FSYNC, OUTPUT);  //FSYNC
  pinMode(SDATA, OUTPUT);  //MOSI
  pinMode(SCLK, OUTPUT);  //SCK
  
  Serial.begin(9600); // start serial communication at 9600bps
  
  digitalWrite(FSYNC, HIGH);
  digitalWrite(SDATA, HIGH);
  digitalWrite(SCLK, HIGH);
  
}

void loop()
{
  char data_in;
  long temp_freq = 0;
  
  delay(100);  //A little set up time, just to make sure everything's stable
  
  //Initial frequency
  freq = 1000;
  set_freq(freq);
  
  Serial.print("Frequency is ");
  Serial.print(freq);
  Serial.println("");
  
  //Get characters (numbers), multiply accordingly, set frequency
  while (1)
  {
    
    if (Serial.available())  // if data is available to read
    {
      data_in = Serial.read();
      
      //Only take numbers, not other weirdness
      if ((data_in > 47) & (data_in < 58))
      {
        temp_freq *= 10;  //multiply the exisisting number by ten
        temp_freq += (data_in - 48);  //add the next entry in the "ones" place
      }
      
      //If we get <enter>, set the freqnency and report it back over the serial port
      else if (data_in == 13)
      {
        freq = temp_freq;        
        set_freq(freq);
        temp_freq = 0;  //zero for the next entry
        
        Serial.print("Frequency is ");
        Serial.print(freq);
      }
         
    }
    
    /*
    set_freq(1000);
    
    delay(2000);
    
    set_freq(5000);
    
    delay(2000);
    
    set_freq(10000);
    
    delay(2000);
    */
    
  }
 
}


void set_freq(long frequency)
{
  //
  int MSB;
  int LSB;
  int phase = 0;
  
  //We can't just send the actual frequency, we have to calculate the "frequency word".
  //This amounts to ((desired frequency)/(reference frequency)) x 0x10000000.
  //calculated_freq_word will hold the calculated result.
  long calculated_freq_word;
  float AD9837Val = 0.00000000;
  
  AD9837Val = (((float)(frequency))/16000000);
  calculated_freq_word = AD9837Val*0x10000000;
  
  /*
  Serial.println("");
  Serial.print("Frequency word is ");
  Serial.print(calculated_freq_word);
  Serial.println("");
  */
  
  //Once we've got that, we split it up into separate bytes.
  MSB = (int)((calculated_freq_word & 0xFFFC000)>>14); //14 bits
  LSB = (int)(calculated_freq_word & 0x3FFF);
  
  //Set control bits DB15 ande DB14 to 0 and one, respectively, for frequency register 0
  LSB |= 0x4000;
  MSB |= 0x4000;
  
  //set for 5KHz
  //LSB = 0x47AE;
  //MSB = 0x4005;
  
  //set for 10KHz
  //LSB = 0x4F5C;
  //MSB = 0x400A;
  
  //set for ?
  //LSB = 0x6F5C;
  //MSB = 0x40F5;
  
  phase &= 0xC000;
  
  AD9837Write(0x2100);
  
  delay(500);
  
  //Set the frequency==========================
  AD9837Write(LSB);  //lower 14 bits

  AD9837Write(MSB);  //upper 14 bits

  AD9837Write(phase);  //mid-low
  
  //Power it back up
  //AD9837Write(0x2020); //square
  AD9837Write(0x2000); //sin
  //AD9837Write(0x2002); //triangle
  
}

//This is the guy that does the actual talking to the AD9837
//via bit-bang SPI...
void AD9837Write(int dat)
{
  int x;
  
  digitalWrite(FSYNC, LOW);  //Set FSYNC low
  
  //
  for (x = 0; x < 16; x++)
  {
    if (dat & 0x8000) digitalWrite(SDATA, HIGH);   //Set SDATA according to MSB in cmd
    else digitalWrite(SDATA, LOW);

    digitalWrite(SCLK, LOW);  //CLK transistion to latch the bit into the AD9835
    digitalWrite(SCLK, HIGH);  
    
    dat <<= 1;  //Shift to next bit in cmd
  }
  
  digitalWrite(FSYNC, HIGH);  //Set FSYNC high
}

