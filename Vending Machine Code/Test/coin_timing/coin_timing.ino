const byte ir_coin_pin_1 = 2;
const byte ir_coin_pin_2 = 2;

bool ir_coin_val_1 = 0;
bool ir_coin_val_2 = 0;

unsigned long time_ir_sensor_1_first_value;
unsigned long time_ir_sensor_1_last_value;
unsigned long time_ir_sensor_2_last_value;

const byte inductive_pin = 2;
bool inductive_val = 0;

void setup()
{
  pinMode(ir_coin_pin_1, INPUT);
  pinMode(ir_coin_pin_2, INPUT);
  pinMode(inductive_pin, INPUT);
}

void loop()
{
  if (Coin_Inserted())
  {
    Serial.println("speed: " + String(time_ir_sensor_1_last_value - time_ir_sensor_2_last_value));
    Serial.println("width: " + String(time_ir_sensor_1_first_value - time_ir_sensor_1_last_value));    
  }
}

bool Coin_Inserted()
{
  inductive_val = digitalRead(inductive_pin);

  if (inductive_val)
  {
    Coin_Sorter(type_valid_metal);

    while (true)
    {
      ir_coin_val_1 = digitalRead(ir_coin_pin_1);
      if (!ir_coin_val_1){
        time_ir_sensor_1_first_value = millis();
        break;
      }
    }

    while (!ir_coin_val_1 || !ir_coin_val_2)
    {
      ir_coin_val_1 = digitalRead(ir_coin_pin_1);
      ir_coin_val_2 = digitalRead(ir_coin_pin_2);

      if (!ir_coin_val_1)
      {
        time_ir_sensor_1_last_value = millis();
      }
      
      time_ir_sensor_2_last_value = millis();
    }

    Coin_Sorter(type_invalid_metal);
  }

  return inductive_val;  
}