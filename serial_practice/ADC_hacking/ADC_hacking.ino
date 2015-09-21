// Arrays to save our results in
unsigned long start_times[100];
unsigned long stop_times[100];
unsigned long values[100];

// Setup the serial port and pin 2
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
}


void loop() {  
  unsigned int i;

  // capture the values to memory
  for(i=0;i<100;i++) {
    start_times[i] = micros();
    values[i] = analogRead(2);
    stop_times[i] = micros();
  }

  // print out the results
  Serial.println("\n\n--- Results ---"); 
  for(i=0;i<100;i++) {
    Serial.print(values[i]);
    Serial.print(" elapse = ");
    Serial.print(stop_times[i] - start_times[i]);
    Serial.print(" us\n");
  }
  delay(6000);
}
