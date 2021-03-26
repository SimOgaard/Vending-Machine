// TODO
// Define values in onWake that are currently defined in code

// LED
const byte red_pin = 2;
const byte green_pin = 3;
const byte blue_pin = 4;

byte red_value = 0;
byte green_value = 0;
byte blue_value = 0;

enum color_mode
{
  single_color
};

const byte[3] const_color = {255, 255, 255};

// IR sensor
const byte ir_coin_pin_1 = 2;
const byte ir_coin_pin_2 = 2;
const byte ir_item_pin = 2;
bool ir_coin_val_1 = 0;
bool ir_coin_val_2 = 0;
bool ir_item_val = 0;
unsigned long time_ir_sensor_1_first_value;
unsigned long time_ir_sensor_1_last_value;
unsigned long time_ir_sensor_2_last_value;

const unsigned int item_sensor_delay = 1000;
const unsigned int motor_time_on = 500;

// INDUCTIVE sensor
const byte inductive_pin = 2;
bool inductive_val = 0;

// BUTTONS / MOTOR
const byte button_pin_start = 5; // ranges from value to value + 9
byte button_value = 0; // range 0 - 10 for each button

const byte motor_ground_pin_start = 14; // ranges from value to value + 4
const byte motor_power_pin_start = 18; // ranges from value to value + 3
byte motor_matrix[5][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}}; // holds button value, search through it find button value, return index 1 and index 2
byte cost_matrix[5][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}}; // holds button value, search through it find button value, return index 1 and index 2
byte motor_ground = 255;
byte motor_power = 255;

// COIN
#include <EEPROM.h>
enum coin_adress
{
  adress_coin_1,
  adress_coin_2,
  adress_coin_5,
  adress_coin_10,
  adress_current_inserted_value
};

enum coin_type
{
  type_invalid = 0,
  type_coin_1 = 1,
  type_coin_2 = 2,
  type_coin_5 = 5,
  type_coin_10 = 10,
  type_valid_metal = 11,
  type_invalid_metal = 12
};

void setup()
{
  // SERIAL
  Serial.begin(9600);

  // INPUT ANALOG
  // pinMode(pin, INPUT);

  // INPUT DIGITAL
  pinMode(ir_coin_pin_1, INPUT);
  pinMode(ir_coin_pin_2, INPUT);
  pinMode(inductive_pin, INPUT);

  // OUTPUT ANALOG
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  // OUTPUT DIGITAL
  // pinMode(pin, OUTPUT);
}

void loop()
{
  // LED
  Change_Color_Mode(color_mode single_color);

  // check coin thingies
  if (Coin_Inserted())
  {
    // valid coin has been inserted
    byte value = EEPROM.read(adress_current_inserted_value);
    value += Get_Inserted_Value();
    EEPROM.write(adress_current_inserted_value, value);
  }

  // check if they want money in return
  if ()
  {
    Return_Money(EEPROM.read(adress_current_inserted_value));
  }

  // When you proceed to try and buy
  if (Button_Search())
  {
    // a button has been pressed
    if (Motor_Search())
    {
      // an item has been dispensed
      Return_Money(EEPROM.read(adress_current_inserted_value));
    }
  }
}

// LED functions
void Change_Color_Mode(color_mode mode)
{
  switch (color_mode mode)
  {
    case single_color:
      Color_Mode_Single_Color();
      break;
    
    default:
      Color_Mode_Single_Color();
      break;
  }

  Apply_RGB();
}

void Color_Mode_Single_Color()
{
  red_value = const_color[0];
  green_value = const_color[1];
  blue_value = const_color[2];
}

void Apply_RGB()
{
  analogWrite(red_pin, red_value);
  analogWrite(green_pin, green_value);
  analogWrite(blue_pin, blue_value);
}

// COIN functions
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

byte Get_Inserted_Value()
{
  unsigned int time_movement = time_ir_sensor_1_last_value - time_ir_sensor_2_last_value; // hur länge det tog coinet att flytta sig från sensor 1 till 2
  unsigned int time_coin = time_ir_sensor_1_first_value - time_ir_sensor_1_last_value; // hur länge sensor 1 har sett coinet

  if ()
  {
    Add_To_EEPROM_Coin_Memory(adress_coin_1);
    Coin_Sorter(type_coin_1);
    return type_coin_1;
  }
  else if ()
  {
    Add_To_EEPROM_Coin_Memory(adress_coin_2);
    Coin_Sorter(type_coin_2);
    return type_coin_2;
  }
  else if ()
  {
    Add_To_EEPROM_Coin_Memory(adress_coin_5);
    Coin_Sorter(type_coin_5);
    return type_coin_5;
  }
  else if()
  {
    Add_To_EEPROM_Coin_Memory(adress_coin_10);
    Coin_Sorter(type_coin_10);
    return type_coin_10;
  }
  else
  {    
    Coin_Sorter(type_invalid);
    return type_invalid;
  }
}

void Add_To_EEPROM_Coin_Memory(byte adress)
{
  byte value = EEPROM.read(address);
  if (value + 1 > max_coin_in_storage)
  {
    // ta bort 1 coin ner till big box of storage
  }
  else
  {
    EEPROM.write(address, value + 1);
  }
}

void Coin_Sorter(coin_type type)
{
  switch (coin_type type)
  {
    case type_coin_1:
      break;
    case type_coin_2:
      break;
    case type_coin_5:
      break;
    case type_coin_10:
      break;
    case type_valid_metal:
      // den är metall
      break;
    case type_invalid_metal:
      // den är inte metall
      break;
    case type_invalid:
    default:
      // den är metall men den är inte rätt storlek
      break;
  }
}

void Return_Money(byte money_to_return)
{
  while (mone_to_return != 0)
  {
    if (mone_to_return >= 10){
      Return_Coin(10);
      mone_to_return -= 10;
    }
    else if (mone_to_return >= 5)
    {
      Return_Coin(5);
      mone_to_return -= 5;
    }
    else if (mone_to_return >= 2)
    {
      Return_Coin(2);
      mone_to_return -= 2;
    }
    else
    {
      Return_Coin(1);
      mone_to_return -= 1;
    }
  }
  EEPROM.write(adress_current_inserted_value, 0);
}

// BUTTON functions
bool Button_Search()
{
  for (byte search_index = 0; search_index < 10; search_index++)
  {
    if (digitalRead(button_pin + search_index))
    {
      button_value = search_index;
      return true;
    }
  }
  button_value = 255;
  return false;
}

bool Motor_Search()
{
  for (byte y = 0; y < 5; y++)
  {
    for (byte x = 0; x < 4; x++)
    {
      if (motor_matrix[y][x] == button_value){
        if (cost_matrix[y][z] > EEPROM.read(adress_current_inserted_value)) // no monie
        {
          Serial.println("no monie");
        }
        else if (Return_Items(y, x))
        {
          byte value = EEPROM.read(adress_current_inserted_value);
          value -= cost_matrix[y][z];
          EEPROM.write(adress_current_inserted_value, value);

          motor_ground = y;
          motor_power = x;
          return true;
        }
      }
    }
  }

  Serial.println("either no monie or item is not existence");

  motor_ground = 255;
  motor_power = 255;
  return false;
}

bool Return_Items(byte y, byte x)
{
  y += motor_ground_pin_start;
  x += motor_power_pin_start;

  const unsigned long motor_time_start = millis();
  unsigned int time = 0; 

  Start_Motor(y, x);

  do
  {
    time = millis() - motor_time_start;

    if (time >= motor_time_on){
      Stop_Motor(y, x); // stop motor
    }

    ir_item_val = digitalRead(ir_item_pin);
    if (ir_item_val)
    {
      return true;
    }

  } while (time < item_sensor_delay);

  return false;
}

void Start_Motor(byte y, byte x)
{
  // start motor
}

void Stop_Motor(byte y, byte x)
{
  // stop motor
}