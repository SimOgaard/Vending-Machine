const byte ir_coin_pin_1 = 2;
const byte ir_coin_pin_2 = 3;

bool ir_coin_val_1 = 0;
bool ir_coin_val_2 = 0;

unsigned long time_ir_sensor_1_first_value;
unsigned long time_ir_sensor_1_last_value;
unsigned long time_ir_sensor_2_first_value;
unsigned long time_ir_sensor_2_last_value;

const byte inductive_pin = 4;
bool inductive_val = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(ir_coin_pin_1, INPUT);
  pinMode(ir_coin_pin_2, INPUT);
  pinMode(inductive_pin, INPUT);
}

void loop()
{
  if (Coin_Inserted())
  {
    for(int i = 0; i < 10; i++){
      Serial.print(time_ir_sensor_1_first_value-time_ir_sensor_1_first_value);      //the first variable for plotting
      Serial.print(",");              //seperator
      Serial.print(time_ir_sensor_2_first_value-time_ir_sensor_1_first_value);      //the first variable for plotting
      Serial.print(",");              //seperator
      Serial.print(time_ir_sensor_1_last_value-time_ir_sensor_1_first_value);      //the first variable for plotting
      Serial.print(",");              //seperator
      Serial.println(time_ir_sensor_2_last_value-time_ir_sensor_1_first_value);      //the first variable for plotting      
    }
  }
}

bool Coin_Inserted()
{
  inductive_val = digitalRead(inductive_pin);

  if (!inductive_val)
  {
    //Coin_Sorter(type_valid_metal);

    // get when ir1 triggerd first
    // get when ir1 stopped trigger
    // get when ir2 triggerd first
    // get when ir2 stopped trigger

    bool ir_2_seen = false;

    while (true)
    {
      ir_coin_val_1 = digitalRead(ir_coin_pin_1);
      if (!ir_coin_val_1){
        time_ir_sensor_1_first_value = micros();
        break;
      }
    }

    while (!ir_coin_val_1 || !ir_coin_val_2)
    {
      ir_coin_val_1 = digitalRead(ir_coin_pin_1);
      ir_coin_val_2 = digitalRead(ir_coin_pin_2);

      if (!ir_coin_val_1)
      {
        time_ir_sensor_1_last_value = micros();
      }
      if (!ir_coin_val_2 && !ir_2_seen)
      {
        time_ir_sensor_2_first_value = micros();
        ir_2_seen = true;
      }
      
      time_ir_sensor_2_last_value = micros();
    }

    //Coin_Sorter(type_invalid_metal);
    return true;
  }

  return false;
}
