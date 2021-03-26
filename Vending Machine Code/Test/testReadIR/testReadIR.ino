const int irPin = 2;
bool irVal = 0;

void setup() {
  Serial.begin(9600);
  pinMode(irPin, INPUT);
}

void loop() {
  irVal = digitalRead(irPin);
  delay(50);
  Serial.println(irVal);
}

