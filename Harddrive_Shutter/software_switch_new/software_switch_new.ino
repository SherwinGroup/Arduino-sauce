int steps = 30;
int delaylength = 300;
int pwma = 3;
int pwmb = 11;
int brakea = 9;
int brakeb = 8;
int dira = 6;
int dirb = 13;
int loopnum = 1000;
int i = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(10, OUTPUT); //不写这句,或写pin10为input,LED都不会亮
  pinMode(5, INPUT);
  Serial.begin(9600);
  //
  pinMode(dira, OUTPUT);
  pinMode(dirb, OUTPUT);
  pinMode(pwma, OUTPUT);
  pinMode(pwmb, OUTPUT);
  pinMode(brakea, OUTPUT);
  pinMode(brakeb, OUTPUT);

  digitalWrite(brakea, LOW);
  digitalWrite(brakeb, LOW);

}
int myButton = 2;
int origin = 1;
int Origin = 0;
int count, Count = 0;
char sw;
// Bool to monitor whether to accept 
// open requests from the button 
bool triggerFromButton = false;
int shutterOpenTime = 10;
int Time = 0;

String W, w, S;
int j = 0;
int q = 5;
//int i=0;
//bool i=false;
int memory = LOW;
//bool TS;
void loop() {
  Serial.println("looped");
  // check if we're being controlled by the comptuer
  if (Serial.available() > 0) {
    
    S = Serial.readString();
    Serial.println("Read string in: " + S);

    // check if it's done with delay# or toggling
    // source.
    if (S.charAt(0) != 'd') {
      delay(10);
      triggerFromButton = bool(int(S.charAt(0))-48);
      Serial.println("Trigger from button set to: " + String(triggerFromButton));
      delay(10);      
    // parse if it's setting the delay. 
    } else {
      shutterOpenTime = S.substring(5).toInt();
      Serial.println("Setting shutter open time to " + String(shutterOpenTime));
    }
  }

  // Read the state of the button
  //   High = unpressed
  //   Low  =   pressed
  int ButtonState = digitalRead(2);

  int TriggerState = digitalRead(5);

  //keep=ButtonState;
  if (triggerFromButton) {
    if (ButtonState == 0 & origin != ButtonState) {
      count = count + 1;
    }
    origin = ButtonState;
    if (count % 2 == 0) {
      digitalWrite(12, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(dira, HIGH);
      digitalWrite(pwma, HIGH);
      //delay(delaylength);
      digitalWrite(dirb, LOW);
      digitalWrite(pwmb, HIGH);
      //delay(delaylength);
    }

    if (count % 2 == 1) {
      digitalWrite(12, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(dira, LOW);
      digitalWrite(pwma, HIGH);
      //delay(delaylength);
      digitalWrite(dirb, HIGH);
      digitalWrite(pwmb, HIGH);
      //delay(delaylength);
    }
    Serial.println("Button State:" + String(ButtonState));
    delay(100);
  } else {
    
    //      Time=w.toInt(); 
    //      Serial.print(w.toInt());
    //      Serial.println('~');
    if (TriggerState == HIGH & shutterOpenTime >= 0 & TriggerState != memory) { //因为值为1时要执行更多的指令所以serial monitor上显示的会慢一点
      //Count=Count+1;
      digitalWrite(12, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(dira, HIGH);
      digitalWrite(pwma, HIGH);
      digitalWrite(dirb, LOW);
      digitalWrite(pwmb, HIGH);
      delay(shutterOpenTime);
      /*
      digitalWrite(12,HIGH); 
      digitalWrite(10,LOW);
      digitalWrite(dira, HIGH);
      digitalWrite(pwma, HIGH);
      digitalWrite(dirb, LOW);
      digitalWrite(pwmb, HIGH);
      */
    } else {
      digitalWrite(12, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(dira, LOW);
      digitalWrite(pwma, HIGH);
      //delay(delaylength);
      digitalWrite(dirb, HIGH);
      digitalWrite(pwmb, HIGH);
    }
    memory = TriggerState;
    //j=0;
    //Origin=TriggerState; 
    Serial.println("Trigger State:" + TriggerState);
    delay(10);
  }
  delay(500);
  Serial.flush();
}
