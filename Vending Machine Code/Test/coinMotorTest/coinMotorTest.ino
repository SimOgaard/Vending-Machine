const byte motor = 2;

void setup() {
  pinMode(motor, OUTPUT);
}

void loop()
{
  digitalWrite(motor, HIGH);
  delay(5000);
  digitalWrite(motor, LOW);
  delay(5000);
}

