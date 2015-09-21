int num;
unsigned long start_times;
unsigned long stop_times;
unsigned long values[100];

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(10);
}

void loop()
{
  //unsigned int i;
  if (Serial.available())
  {
    start_times = micros();
    
    num = Serial.parseInt();
    
    if (num == 12)
    {
      num += num;
      Serial.println(num);
      Serial.println("AAAAAHHHHH");

    }
    
    stop_times = micros();
    Serial.println(num);
    Serial.print(" elapse = ");
    Serial.print(stop_times - start_times);
    Serial.print(" us\n");
    //delayMicroseconds(1);
  }
  //Serial.println("wait");
  //delayMicroseconds(1);
}

