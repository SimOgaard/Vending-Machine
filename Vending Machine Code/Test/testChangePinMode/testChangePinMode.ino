#define green_pin 3

typedef enum States {
  Output, Input
};
States State;

void setup() {
  State = Output;
  Serial.begin(9600);
}

void loop() {
  switch (State) {
    case Output:
      pinMode(green_pin, OUTPUT);
      analogWrite(green_pin, 1023);
      delay(2000);
      analogWrite(green_pin, 0);
      State = Input;
      break;

    case Input:
      pinMode(green_pin, INPUT);
      delay(2000);
      Serial.println(analogRead(green_pin));
      State = Output;
      break;
  }
}
