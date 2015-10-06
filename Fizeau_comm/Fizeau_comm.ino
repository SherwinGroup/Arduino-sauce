// Parallel read of the linear sensor array TSL1402R (= the sensor with 256 photodiodes)
//-------------------------------------------------------------------------------------

// Define various ADC prescaler:
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
int TGpin = 23; // This pin can be slow
/* These aren't needed because bitwise hacking   
int IOpin = 5;
int SHOpin = 6;  
int ROpin = 7;
*/
int offsetPin = 12; // Arduino pin that sets bias with PWM
int PWMlevel = 210;
int pixels = 529;
int AOpin = 1;    // <-- Arduino pin connected to pin 4 (analog output 1) of the CCD
int IntArray[1058]; // <-- the array where the readout of the photodiodes is stored, as integers
int holder; 
int incoming = 1; // This is the signal from the computer that the next run should be sent back.  
                  // It sets the integration time.

void setup() 
{
  // Initialize two Arduino pins as digital output:
  pinMode(offsetPin, OUTPUT);
  pinMode(TGpin, OUTPUT); 
  pinMode(53, OUTPUT); // This is the IO clock
  pinMode(6, OUTPUT); // This is the SHO clock
  pinMode(9, OUTPUT); // This is the RO clock
  // To set up the ADC, first remove bits set by Arduino library, then choose 
  // a prescaler: PS_16, PS_32, PS_64 or PS_128:
  //ADCSRA &= ~PS_128;  
  //ADCSRA |= PS_32; // <-- Using PS_32 makes a single ADC conversion take ~30 us

  analogWrite(offsetPin, PWMlevel);
  //analogReference(DEFAULT);
  /*  I don't think this is necessary?
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
  */
  Serial.begin(115200);
  //Serial.setTimeout(10); // This makes parseInt not wait a full 
                         // fucking second before proceeding.  
                         // It can be made shorter, but runs
                         // the risk of not reading things
}

void loop() 
{ 
  // This part of the loop keeps cleaning the CCD
  digitalWrite(TGpin, HIGH);
  delayMicroseconds(2);
  for(int i=0; i<pixels; i++)
  {
    // Read part 1 of bin:
    PORTB &= ~_BV(PB0); //digitalWrite(IOpin, LOW);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); 
              // This delays by 1 clock cycle per nop
    PORTH &= ~_BV(PH3); //digitalWrite(SHOpin, LOW);
    delayMicroseconds(1); 
    PORTH |= _BV(PH3); //digitalWrite(SHOpin, HIGH);
    delayMicroseconds(1);
    PORTH &= ~_BV(PH6); //digitalWrite(ROpin, LOW);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    PORTH |= _BV(PH6); //digitalWrite(ROpin, HIGH);
    
    delayMicroseconds(1);
    PORTB |= _BV(PB0); //digitalWrite(IOpin, HIGH);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //delayMicroseconds(4);
    PORTH &= ~_BV(PH3); //digitalWrite(SHOpin, LOW);
    delayMicroseconds(1);
    PORTH |= _BV(PH3); //digitalWrite(SHOpin, HIGH);
    delayMicroseconds(1);
    PORTH &= ~_BV(PH6); //digitalWrite(ROpin, LOW);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//delayMicroseconds(1);
    PORTH |= _BV(PH6); //digitalWrite(ROpin, HIGH);
    //delayMicroseconds(1);
  }
  digitalWrite(TGpin, LOW);
  //delay(1); We want the delay to happen after everything so that 
           // the incoming data can set integration time

  if (Serial.available() > 0)
  {
    incoming = Serial.parseInt(); // setTimeout made the minimum
                                  // integration time 10 ms.
    // if you pass an ending character so parseInt()
    // returns faster, read the line, so it won't be
    // left in the buffer for the next loop() cycle
    // to catch and act erratically. 
    if (Serial.available()>0){
      Serial.read();
    }
    Serial.print('d');
    Serial.println(incoming);
    delay(incoming); // The integration time for the current run in ms.

    digitalWrite(TGpin, HIGH);
    delayMicroseconds(2);
    for(int i=0; i<pixels; i++)
    {
      // Read part 1 of bin:
      PORTB &= ~_BV(PB0); //digitalWrite(IOpin, LOW);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); 
      PORTH &= ~_BV(PH3); //digitalWrite(SHOpin, LOW);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); 
      PORTH |= _BV(PH3); //digitalWrite(SHOpin, HIGH);
      holder = analogRead(AOpin);
      PORTH &= ~_BV(PH6); //digitalWrite(ROpin, LOW);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      PORTH |= _BV(PH6); //digitalWrite(ROpin, HIGH);
      //Serial.print(", ");
      if (i < pixels)
      {
        IntArray[2*i] = holder;
      }
      // Read nothing?
      PORTB |= _BV(PB0); //digitalWrite(IOpin, HIGH);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); 
      PORTH &= ~_BV(PH3); //digitalWrite(SHOpin, LOW);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); 
      PORTH |= _BV(PH3); //digitalWrite(SHOpin, HIGH);
      holder = analogRead(AOpin);
      PORTH &= ~_BV(PH6); //digitalWrite(ROpin, LOW);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      PORTH |= _BV(PH6); //digitalWrite(ROpin, HIGH);

      if (i < (pixels - 1))
      {
        IntArray[(2*i+1)] = holder;
      }
    }
    for(int i = 1; i < (2*pixels-1); i++)
    {
      Serial.print(IntArray[i]); Serial.print(";");
    }
    Serial.println(""); // Send a linebreak to indicate 
                        // the measurement is transmitted.
    
    digitalWrite(TGpin, LOW);

  }
  delay(1); // Is this necessary?
}
