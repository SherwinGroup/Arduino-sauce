void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char ser = Serial.read();
    if (ser == "on"){
      digitalWrite(13, HIGH);
    }
    else if (ser == "off"){
      digitalWrite(13, LOW);
    }
  }
}
