//available for interrupt: 2, 3, 18, 19, 20, 21

const byte ir_coin_pin_1 = 2;
const byte ir_coin_pin_2 = 3;

unsigned long time_ir_sensor_1_first_value;
unsigned long time_ir_sensor_1_last_value;
unsigned long time_ir_sensor_2_first_value;
unsigned long time_ir_sensor_2_last_value;

const byte inductive_pin = 18;

bool should_print = false;
bool is_active_ir_coin_pin_1 = false;
bool is_active_ir_coin_pin_2 = false;

void setup()
{
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(ir_coin_pin_1), ir_1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ir_coin_pin_2), ir_2, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(inductive_pin), detected_metal, FALLING);
}

void ir_1()
{
  if (is_active_ir_coin_pin_1){
    // no longer sees it
    time_ir_sensor_1_last_value = micros();
    is_active_ir_coin_pin_1 = false;
    return;
  }
  time_ir_sensor_1_first_value = micros();
  is_active_ir_coin_pin_1 = true;
  // started seeing it
  return;
}

void ir_2()
{
  if (is_active_ir_coin_pin_2){
    // no longer sees it
    time_ir_sensor_2_last_value = micros();
    is_active_ir_coin_pin_2 = false;
    print_val();
    return;
  }
  time_ir_sensor_2_first_value = micros();
  is_active_ir_coin_pin_2 = true;
  // started seeing it
  return;
}

void print_val()
{
  for (int i = 0; i < 5; i++){  
    Serial.print(time_ir_sensor_2_first_value-time_ir_sensor_1_first_value);      //variable for plotting
    Serial.print(",");              //seperator
    Serial.print(time_ir_sensor_1_last_value-time_ir_sensor_1_first_value);       //variable for plotting
    Serial.print(",");              //seperator
    Serial.println(time_ir_sensor_2_last_value-time_ir_sensor_1_first_value);     //variable for plotting      
  }
}

void loop()
{
//  print_val();
}

