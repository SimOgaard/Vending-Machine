const byte motor_ground_pin_start = 22; // ranges from value to value + 4
const byte motor_power_pin_start = 28; // ranges from value to value + 3

void setup()
{
  for (byte i = 0; i <= 4; i++)
  {
    pinMode(motor_ground_pin_start + i, OUTPUT);
    digitalWrite(motor_ground_pin_start + i, LOW);
  }
  for (byte i = 0; i <= 3; i++)
  {
    pinMode(motor_power_pin_start + i, OUTPUT);
    digitalWrite(motor_power_pin_start + i, LOW);
  }
  delay(100);
}

void TESTMOTOR()
{
  digitalWrite(motor_power_pin_start, LOW);
  delay(100);
  digitalWrite(motor_power_pin_start, HIGH);
  delay(4000);
}

void loop()
{
  //TESTMOTOR();
}
