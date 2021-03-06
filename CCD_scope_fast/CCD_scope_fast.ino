// Parallel read of the linear sensor array TSL1402R (= the sensor with 256 photodiodes)
//-------------------------------------------------------------------------------------

// Define various ADC prescaler:
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
int TGpin = 4;    // <-- Arduino pin delivering the clock pulses to pin 3 (CLK) of the TSL1402R 
int IOpin = 5;
int SHOpin = 6;     // <-- Arduino pin delivering the SI (serial-input) pulse to pin 2 of the TSL1402R 
int ROpin = 7;
int AOpin = 1;    // <-- Arduino pin connected to pin 4 (analog output 1)of the TSL1402R
int IntArray[400]; // <-- the array where the readout of the photodiodes is stored, as integers
int holder; 

void setup() 
{
  // Initialize two Arduino pins as digital output:
  pinMode(TGpin, OUTPUT); 
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);  
  pinMode(7, OUTPUT);
  // To set up the ADC, first remove bits set by Arduino library, then choose 
  // a prescaler: PS_16, PS_32, PS_64 or PS_128:
  ADCSRA &= ~PS_128;  
  ADCSRA |= PS_32; // <-- Using PS_32 makes a single ADC conversion take ~30 us

  //analogReference(DEFAULT);

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
  Serial.begin(115200);
}

void loop() 
{  
  digitalWrite(TGpin, HIGH);
  delayMicroseconds(2);
  for(int i=0; i<525; i++)
  {
    // Read part 1 of bin:
    PORTD &= ~_BV(PD5); //digitalWrite(IOpin, LOW);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //delayMicroseconds(4);
    PORTD &= ~_BV(PD6); //digitalWrite(SHOpin, LOW);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //delayMicroseconds(1); This delays by 1 clock cycle per nop
    PORTD |= _BV(PD6); //digitalWrite(SHOpin, HIGH);
    holder = analogRead(AOpin);
    //delayMicroseconds(35);
    PORTD &= ~_BV(PD7); //digitalWrite(ROpin, LOW);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//delayMicroseconds(1);
    PORTD |= _BV(PD7); //digitalWrite(ROpin, HIGH);
    //delayMicroseconds(1);
    if (i < 400)
    {
      IntArray[i] = holder;
    }
    // Read nothing?
    PORTD |= _BV(PD5); //digitalWrite(IOpin, HIGH);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //delayMicroseconds(4);
    PORTD &= ~_BV(PD6); //digitalWrite(SHOpin, LOW);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //delayMicroseconds(1);
    PORTD |= _BV(PD6); //digitalWrite(SHOpin, HIGH);
    holder = analogRead(AOpin);
    //delayMicroseconds(35);
    PORTD &= ~_BV(PD7); //digitalWrite(ROpin, LOW);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//delayMicroseconds(1);
    PORTD |= _BV(PD7); //digitalWrite(ROpin, HIGH);
    //delayMicroseconds(1);
    if (i < 399)
    {
      IntArray[i+1] = holder;
    }
  }
  for(int i = 0; i < 400; i++)
  {
    Serial.print(IntArray[i]); Serial.print(";");
  }
  Serial.println(""); // <-- Send a linebreak to indicate the measurement is transmitted.
  
  digitalWrite(TGpin, LOW);
  delay(1);
}
