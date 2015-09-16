// Parallel read of the linear sensor array TSL1402R (= the sensor with 256 photodiodes)
//-------------------------------------------------------------------------------------

// Define various ADC prescaler:
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
int TGpin = 4;    // <-- Arduino pin delivering the clock pulses to pin 3 (CLK) of the TSL1402R 
int IOpin = 5;
int SHOpin = 6;     // <-- Arduino pin delivering the SI (serial-input) pulse to pin 2 of the TSL1402R 
int ROpin = 7;

void setup() 
{
  // Initialize two Arduino pins as digital output:
  pinMode(TGpin, OUTPUT); 
  pinMode(IOpin, OUTPUT);
  pinMode(SHOpin, OUTPUT);  
  pinMode(ROpin, OUTPUT);
  // To set up the ADC, first remove bits set by Arduino library, then choose 
  // a prescaler: PS_16, PS_32, PS_64 or PS_128:
  ADCSRA &= ~PS_128;  
  ADCSRA |= PS_32; // <-- Using PS_32 makes a single ADC conversion take ~30 us

  analogReference(DEFAULT);

  for(int i=0;i< 529;i++)
  {
    // Read part 1 of bin:
    digitalWrite(IOpin, LOW);
    delayMicroseconds(50);
    digitalWrite(SHOpin, LOW);
    delayMicroseconds(50);
    digitalWrite(SHOpin, HIGH);
    delayMicroseconds(50);
    digitalWrite(ROpin, LOW);
    delayMicroseconds(50);
    digitalWrite(ROpin, HIGH);
    delayMicroseconds(50);

    // Read nothing?
    digitalWrite(IOpin, HIGH);
    delayMicroseconds(50);
    digitalWrite(SHOpin, LOW);
    delayMicroseconds(50);
    digitalWrite(SHOpin, HIGH);
    delayMicroseconds(50);
    digitalWrite(ROpin, LOW);
    delayMicroseconds(50);
    digitalWrite(ROpin, HIGH);
    delayMicroseconds(50);
  }
}

void loop() 
{  
  digitalWrite(TGpin, HIGH);
  for(int i=0; i<529; i++)
  {
    // Read part 1 of bin:
    digitalWrite(IOpin, LOW);
    delayMicroseconds(50);
    digitalWrite(SHOpin, LOW);
    delayMicroseconds(50);
    digitalWrite(SHOpin, HIGH);
    delayMicroseconds(50);
    digitalWrite(ROpin, LOW);
    delayMicroseconds(50);
    digitalWrite(ROpin, HIGH);
    delayMicroseconds(50);

    // Read nothing?
    digitalWrite(IOpin, HIGH);
    delayMicroseconds(50);
    digitalWrite(SHOpin, LOW);
    delayMicroseconds(50);
    digitalWrite(SHOpin, HIGH);
    delayMicroseconds(50);
    digitalWrite(ROpin, LOW);
    delayMicroseconds(50);
    digitalWrite(ROpin, HIGH);
    delayMicroseconds(50);
  }
  digitalWrite(TGpin, LOW);
  delay(10);
}
