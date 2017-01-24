// Pin numbers for speed on motor shield
int pwma = 3;
int pwmb = 11;
// Pin numbers for brakes on motor shield
int brakea = 9;
int brakeb = 8;
// Pins to specify direction on motor shield
int dira = 12;
int dirb = 13;
// Pin to read state of the shutter Switch
int switchPin = 2;
// Pin to read the level of the trigger
int triggerIn = 5;
int triggerOut = 6;

// Bool to monitor whether to accept 
// open requests from the Switch 
bool triggerFromSwitch = false;
// Duration to keep shutter open after recieving a trigger pulse
int shutterOpenTime = 100;

// string for reading in the serial buffer
String serialBuffer;

// variables for reading in the Switch/trigger levels
int currentSwitchState  = 1;
int currentTriggerState = 0;
// memory for knowing what the previous state was. 
int prevSwitchState  = 1;
int prevTriggerState = 0;


// control the "speed" of the shutter?
int motorPWMHoldLevel = 255;
int motorPWMMoveLevel = 255;
// time to delay before switching the move current to the hold current
int motorMoveTime = 1;

void toggleShutter(int todo=HIGH) {
//  digitalWrite(brakea, LOW);
  analogWrite(pwma, motorPWMMoveLevel);
  digitalWrite(dira, todo);
  delay(motorMoveTime);
  analogWrite(pwma, motorPWMHoldLevel);
//  digitalWrite(brakea, HIGH);
}

void openShutter() {
  toggleShutter(HIGH);
}

void closeShutter() {
  toggleShutter(LOW);
}

void setup() {
  // These pins were used for controlling the R/G LEDs on the protoshield
//  pinMode(12, OUTPUT);
//  pinMode(10, OUTPUT); 
  pinMode(triggerIn, INPUT);
  pinMode(triggerOut, OUTPUT);
  Serial.begin(9600);
  
  // Set pin modes on all of the 
  pinMode(dira, OUTPUT);
  pinMode(dirb, OUTPUT);
  pinMode(pwma, OUTPUT);
  pinMode(pwmb, OUTPUT);
  pinMode(brakea, OUTPUT);
  pinMode(brakeb, OUTPUT);

  digitalWrite(brakea, LOW);
  digitalWrite(brakeb, LOW);

}


void loop() {
  // check if we're being sent commands by the computer
  /*
   * Either "delay<#>", where <#> is an int in ms for how long the shutter should stay open
   * "1" to enable the Switch press to open the shutter (and disable external triggering)
   * "0" to to do the opposite.
   */
  if (Serial.available() > 0) {
    // Read in whatever was sent
    serialBuffer = Serial.readString();
    
//    Serial.println("Read string in: " + serialBuffer);

    // Assume you're enabling/disabling the Switch if youdon't start with 'd'.
    // Should be noted that anything besides '0' will be intepretted as '1'. 
    if (serialBuffer.charAt(0) == 'd') {
      // Note: really works for anything dxxxx where x is any character.
      // leaving it how the undergrads first programmed it.
      shutterOpenTime = serialBuffer.substring(5).toInt();
      Serial.println("Setting shutter open time to " + String(shutterOpenTime)); 
    // debugger to let me set the analog output of the motor
    }
    else if (serialBuffer.substring(0,2) == "ph")
    {
      
      motorPWMHoldLevel = serialBuffer.substring(2).toInt();
      Serial.println("Setting motor hold PWM to " + String(motorPWMHoldLevel));
    }
    else if (serialBuffer.substring(0,2) == "pm")
    {
      
      motorPWMMoveLevel = serialBuffer.substring(2).toInt();
      Serial.println("Setting motor move PWM to " + String(motorPWMMoveLevel));
    }
    else if (serialBuffer.substring(0,2) == "td")
    {
      
      motorMoveTime = serialBuffer.substring(2).toInt();
      Serial.println("Setting motor delay to " + String(motorMoveTime));
    }
    else
    {
      delay(10);
      // Want to cast the response to a bool. chr(48)=='0', 
      // so the subtraction helps convert the string/char '0' to the int 0.
      triggerFromSwitch = bool(int(serialBuffer.charAt(0))-48);
      Serial.println("Trigger from Switch set to: " + String(triggerFromSwitch));
      delay(10);     
    }
  }

  // Read the state of the Switch
  //   High = unpressed
  //   Low  =   pressed
  currentSwitchState = digitalRead(switchPin);
  currentTriggerState = digitalRead(triggerIn);

  // handle passing along the trigger level
  // the simple/naive/stupid way:
  // Unnecessarily writing to the pin every loop, even if it isn't changing
  // Can have issues that the trigger will stay high longer than the actual trigger
  //   if the delay time is longer than the trigger pulse
  digitalWrite(triggerOut, currentTriggerState);
  
  

  // Handle if you want to open the shutter depending on
  // Switch state
  if (triggerFromSwitch) {
    // detect if it's different from the last loop
    if (prevSwitchState != currentSwitchState) {
      // Update the shutter to open/close as necessary
      if (currentSwitchState) {
        openShutter();
      }
      else {
        closeShutter();
      }
    }
//    Serial.println("Switch State:" + String(currentSwitchState));
//    delay(100);
    
  }
  // If not triggering from the Switch, we're being driven by an external trigger
  else {
    if (currentTriggerState == HIGH & shutterOpenTime >= 0 & currentTriggerState != prevTriggerState) {
      openShutter();
      delay(shutterOpenTime);
      closeShutter();
    }
    else {
    }
//    Serial.println("Trigger State:" + currentTriggerState);
//    delay(10);
  }
  prevTriggerState = currentTriggerState;
  prevSwitchState = currentSwitchState;
//  delay(500);
//  Serial.flush();
}







































