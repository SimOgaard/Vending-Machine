const int inductivePin = 2;
bool inductiveVal = 0;

void setup() {
  Serial.begin(9600);
  pinMode(inductivePin, INPUT);
}

void loop() {
  inductiveVal = digitalRead(inductivePin);
  if (!inductiveVal)
  {
    Serial.println("FoundCoin");
  }
}

