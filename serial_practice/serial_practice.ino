char ser;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    ser = Serial.read();
    if (ser == 'i') {
      digitalWrite(13, HIGH);
    }
    else if (ser == 'o') {
      digitalWrite(13, LOW);
    }
  }
}
