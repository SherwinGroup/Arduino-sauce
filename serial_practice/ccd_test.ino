
/* Pulsed stripping, cyclic, timed, and loged stripping voltammetry sketch
   for use with the processing GUI 
   Based on BHickman work --current sketch assumes hardware defined pulse size
   Energia help and download (http://energia.nu/)
 
/*/////////////////////////sub-programs used in main loop///////////////////*/ 

//#include "wiring_analog.c"
//PWMWrite(pwm_pin,1000,500,1000);
/**********USER DEFINED VARIABLES*******************/


/*********NON-USER VARIABLES*********************/
long place[6]={100000,10000,1000,100,10,1};           /* {serial read conversions. each incoming byte in multiplied 
                            by the corrosponding place value then summed to get the
                            original vlaue} */
long incomingByte[6] = {0,0,0,0,0,0};                 // serial read variable. each incoming byte (multiplied by the place)
                                                      // is stored here. later the get summed to obtain the correct number      
int pgm;                                              // 0: absorbance, 1: transmitance, 2: series
int ard;
int aread[4][768];                                    // used for 4 run average
int itteration = 0;                                   //
int inttime = 0;                                      //0-500
int runinterval;
int numruns;
//#define  pwm_pin  9            // Pin 23
#define  CLK  PD_0//PF_2//PD_0                         // Pin 24
#define  SI  PD_1//PF_3//PD_1                          // Pin 25/PD2
#define  ARD  A5


/**********************************************/
void setup() 
  {

  pinMode (CLK,OUTPUT);      // set pin 2.1 for output   
  pinMode (SI,OUTPUT);      // set pulse pin as output
  pinMode (ARD,INPUT);
  //pinMode (Iread_pin,INPUT);
  //pinMode (Offset_read_pin,INPUT); 
  //pinMode (stir_pin,OUTPUT);      /// set pin 2.4 for output
  //analogResolution(1023);         // divide the full duty cycle into 1024 steps
  //analogFrequency(5000);          // set the full duty cycle to 0.2 ms
 // Serial.begin(9600);             // begin serial comm. at 9600 baud

  }

void D_array()
  {
  if(itteration == 0)
    {
    digitalWrite(SI,1);            // Start reading data
    digitalWrite(CLK,1);
    delayMicroseconds(80);
    //ard[0] = analogRead(ARD);
    int dumy = analogRead(ARD);
    digitalWrite(SI,0);
    digitalWrite(CLK,0);
    delayMicroseconds(90);
    for(int i=0;i<767;i++)
      {
      digitalWrite(CLK,1);         
      delayMicroseconds(90);
      //ard[i+1] = analogRead(ARD);
      int dumy = analogRead(ARD);
      digitalWrite(CLK,0);
      delayMicroseconds(90);
      } 
    for(int i = 0;i<inttime;i++)
      {    // added intergration time
      digitalWrite(CLK,1);         
      delayMicroseconds(90);
      digitalWrite(CLK,0);
      delayMicroseconds(90);
      }
    } // end initial array clear

    
  /*  digitalWrite(SI,1);            // Start signal intergration for n+1 clock cycles
    digitalWrite(CLK,1);
    delayMicroseconds(90);
    digitalWrite(SI,0);            // 
    digitalWrite(CLK,0);
    delayMicroseconds(90);
   for(int i=0;i<768;i++)
    {
      digitalWrite(CLK,1);         
      delayMicroseconds(90);
      digitalWrite(CLK,0);
      delayMicroseconds(90);
    } 
    //delayMicroseconds(30);         // delay to allow last pixle to trasnfer charge  
    int inttime = 6500;
    for(int i = 0;i<inttime;i++){
      digitalWrite(CLK,1);         
      delayMicroseconds(90);
      digitalWrite(CLK,0);
      delayMicroseconds(90);
      }   */
   
    
  digitalWrite(SI,1);            // Start reading data
  digitalWrite(CLK,1);
  delayMicroseconds(80);
  aread[itteration][0] = analogRead(ARD);
  digitalWrite(SI,0);
  digitalWrite(CLK,0);
  delayMicroseconds(90);
  for(int i=0;i<767;i++)
    {
    digitalWrite(CLK,1);         
    delayMicroseconds(90);
    aread[itteration][i+1] = analogRead(ARD);
    digitalWrite(CLK,0);
    delayMicroseconds(90);
    } 
  for(int i = 0;i<inttime;i++)
      {    // adjust intergration time
      digitalWrite(CLK,1);         
      delayMicroseconds(90);
      digitalWrite(CLK,0);
      delayMicroseconds(90);
      }
  }

void setup_run()
  {
  int n=0;
  inttime = 0;
  runinterval = 0;
  numruns = 0;
  pgm = 0;


  ////////////////mode//////////////////////////////////////  
  Serial.println("Mode (1:Absorbance, 2:Transmitance, 3:Series)");
  int mod = 0; 
  //n=5;
  delayMicroseconds(300);  
  // while (Serial.read() >= 0); // clear serial available   
  while (Serial.available()<= 0) 
    {}
  while (Serial.available() > 0)
    {  
    incomingByte[5] = Serial.read()-48;
    }
  pgm = incomingByte[5];
  incomingByte[5] = 0;
  Serial.println(pgm);
  //Serial.println("Voltage\tBack I\tForward I");
  //Serial.println('&');
  //////////start voltage/////////////////////////////////
  Serial.println("Intergration Time (ms)");
  delayMicroseconds(300); 
  long It = 0;                       // local variable for temporary storage of intergration time
  n = 0;                            // loop count variable
  // while (Serial.read() >= 0);       // dont do anything untill buffer is clean   
  while (Serial.available()<= 0) 
    {}                                  //wait for incoming datat
  while (Serial.available() > 0)
    {
    incomingByte[n] = Serial.read()-48;   // read incoming Byte and store in incomingByte array
    delay(2);
    ++n;          
    }
  for(int n=0;n<=5;++n) //for(int n=2;n<=5;++n)    for(int n=0;n<5;n++)
    { 
    It = It + (place[n]*(incomingByte[n]));  // convert from ascii to decimal 
    }
  inttime=It;                // global varaible Vinit now holds the initial voltage
  Serial.println(inttime); //Serial.println(Vinit-1800);   

  if(pgm == 3)
    {
    /////////////////////ending voltage/////////////////////
    Serial.println("Run Interval (sec)");  
    delayMicroseconds(300);   
    long Ri = 0;                                      // run interval local
    n=0;   
    //  while (Serial.read() >= 0); // clear serail available
    while (Serial.available()<= 0)
      {}
    while (Serial.available() > 0)
      {      
      incomingByte[n] = Serial.read()-48;
      delay(2);
      ++n;
      }
    for(int n=0;n<=5;++n)
      {
      Ri = Ri + (place[n]*(incomingByte[n])); 
      }
    runinterval = Ri;
    Serial.println(runinterval);  
    
    Serial.println("Number of Runs");  
    delayMicroseconds(300);   
    long Nr = 0;                                      // run interval local
    n=0;   
    //  while (Serial.read() >= 0); // clear serail available
    while (Serial.available() <= 0)
      {}
    while (Serial.available() > 0)
      {      
      incomingByte[n] = Serial.read()-48;
      delay(2);
      ++n;
      }
    for(int n=0;n<=5;++n)
      {
      Nr = Nr + (place[n]*(incomingByte[n])); 
      }
    numruns = Nr;
    Serial.println(numruns); 
    } 
        
    
  if(pgm == 1 || pgm == 3)
    {  
    Serial.println("Absorbance"); 
    }
  if(pgm ==2)
    {
    Serial.println("Transmitance");
    }
  Serial.println('&');
  }

void conversions ()
  {
  if(pgm == 3)
    {
    runinterval = runinterval*1000;
    }
  }

void loop() 
  {
  Serial.begin(9600);
  delay(100);    
  //while (Serial.read() >= 0);            // clear serial available
  Serial.flush();
  setup_run();
  conversions ();
  while (Serial.available()<= 0)      // do nothing if no GO signal
    {}
  if (Serial.read() == '*')          // start if recieved GO signal from GUI
    {
   //Serial.println("Start");
    for(itteration = 0; itteration < 4; itteration++)
      {
      D_array();                       // 
      }
    for(int i =768;i>0;i--)
      {
      ard = 0;
      for(int j =0;j<4;j++)
        {
        ard += aread[j][i];
        }
      ard = ard/4;
      Serial.println(ard);
      //Serial.print('\n');
      delay(1);
      }  
 /*  for(int i =768;i>0;i--){
   Serial.println(ard[i]);
   }*/
    }
  //else{} 
  delay(10);
  Serial.println("@");                     // signal GUI to end run
  delay(10);
  Serial.flush();
  delay(10);
  Serial.end();

  }  // end loop