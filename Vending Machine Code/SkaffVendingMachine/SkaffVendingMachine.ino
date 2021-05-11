// TODO
// Test //
// Get_Inserted_Value() needs too be tested so we have the right if(values)

// Set all pins //
// Read button to know when they want money in return
// Start_Motor(y,x) and Stop_Motor(y,x) y to ground and x to power
// Stash_Coin() what servo to turn on to dispose that coin to the stash
// Coin_Sorter() which motor to start to sort the incomming money
// Return_Coin() which motor to return that coin

// Set values (EndGame) //
// Set motormatrix and cost matrix so we know what dispens what, and for how much

bool WTF = false;

// LED
const byte red_pin = A0;
const byte green_pin = A1;
const byte blue_pin = A2;

byte red_value = 0;
byte green_value = 0;
byte blue_value = 0;

enum color_mode
{
  single_color
};

byte const_color[3] = {160, 0, 255};

// IR sensor
const byte ir_coin_pin_1 = 2;
const byte ir_coin_pin_2 = 2;
const byte ir_item_pin = 2;
bool ir_coin_val_1 = 0;
bool ir_coin_val_2 = 0;
bool ir_item_val = 0;
unsigned long time_ir_sensor_1_first_value;
unsigned long time_ir_sensor_1_last_value;
unsigned long time_ir_sensor_2_first_value;
unsigned long time_ir_sensor_2_last_value;

const unsigned int item_sensor_delay = 1000;
const unsigned int motor_time_on = 500;

bool is_active_ir_coin_pin_1 = false;
bool is_active_ir_coin_pin_2 = false;

// INDUCTIVE sensor
const byte inductive_pin = 2;
bool inductive_val = 0;
bool is_metal = false;

// BUTTONS / MOTOR
unsigned long motor_time_start;
unsigned int time = 0;

unsigned int time_coin_movement;
unsigned int time_coin_thickness;

const byte button_pin_start = 40; // ranges from value to value + 9
byte button_value = 0; // range 0 - 10 for each button
bool button_is_down = false;

const byte motor_ground_pin_start = 22; // ranges from value to value + 4
const byte motor_power_pin_start = 27; // ranges from value to value + 3
const byte motor_matrix[5][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}};
const byte cost_matrix[5][4] = {{10, 10, 20, 30}, {40, 50, 60, 70}, {80, 90, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}};
byte motor_ground = 255;
byte motor_power = 255;

// COIN
#include <EEPROM.h>
byte EEPROMMemoryValue = 0;
byte max_coin_in_storage = 20;

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

// LCD
#include <LiquidCrystal.h>

LiquidCrystal lcd(5, 6, 7, 8, 9, 10);

unsigned long last_unique_message = 0;
const unsigned int message_timer = 4000;
const unsigned int message_afk_timer = 15000;
const unsigned int message_left_timer = 35000;

String message0;
String message1;

void setup()
{
  // SERIAL
  Serial.begin(9600);
  lcd.begin(16, 2);
  EEPROM.write(adress_current_inserted_value, 0);
  if (EEPROM.read(adress_current_inserted_value) > 0)
  {
    Write_To_LCD("Hejsan", 0, 0);
    Write_To_LCD("Du har " + String(EEPROM.read(adress_current_inserted_value)) + "kr", 0, 1);
  }
  else
  {
    Write_To_LCD("Konstautomat", 0, 0);
    Write_To_LCD("Nu diabetes fri!", 0, 1);    
  }
        
  // INPUT ANALOG
  // pinMode(pin, INPUT);

  // INPUT DIGITAL
  pinMode(inductive_pin, INPUT);

  // OUTPUT ANALOG
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  // OUTPUT DIGITAL
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

  // INTERRUPT
  attachInterrupt(digitalPinToInterrupt(ir_coin_pin_1), ir_1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ir_coin_pin_2), ir_2, CHANGE);
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
    Coin_Inserted();
    return;
  }
  time_ir_sensor_2_first_value = micros();
  is_active_ir_coin_pin_2 = true;
  // started seeing it
  return;
}

void loop()
{
  // LED
  Change_Color_Mode(single_color);

  EEPROMMemoryValue = EEPROM.read(adress_current_inserted_value);
  if (EEPROMMemoryValue > 0)
  {
    // du är en gammal kund
    if (millis() - last_unique_message > message_left_timer)
    {
      Write_To_LCD("Avbryter kopet", 0, 0);
      Return_Money(EEPROMMemoryValue);
    }
    else if (millis() - last_unique_message > message_afk_timer)
    {
      Write_To_LCD("Hallo", 0, 0);
      Write_To_LCD("Ar du kvar?", 0, 1);      
    }
    else if (millis() - last_unique_message > message_timer)
    {
      Write_To_LCD("Hejsan", 0, 0);
      Write_To_LCD("Du har " + String(EEPROMMemoryValue) + "kr", 0, 1);
    }
  }
  else
  {
    // du är ny kund
    if (millis() - last_unique_message > message_timer)
    {
      Write_To_LCD("Konstautomat", 0, 0);
      Write_To_LCD("Nu diabetes fri!", 0, 1);
    }
  }
    
  inductive_val = digitalRead(inductive_pin);
  if (!inductive_val && !WTF)
  {
    is_metal = true;
  }

  // When you proceed to try and buy
  if (Button_Search())
  {
    // a button has been pressed
    if (Motor_Search())
    {
      // an item has been dispensed
      Return_Money(EEPROMMemoryValue);
    }
  }
}

// LED functions
void Change_Color_Mode(color_mode mode)
{
  switch (mode)
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
void Coin_Inserted()
{  
  Serial.print(is_metal);      //variable for plotting
  Serial.print(time_ir_sensor_2_first_value-time_ir_sensor_1_first_value);      //variable for plotting
  Serial.print(",");              //seperator
  Serial.print(time_ir_sensor_1_last_value-time_ir_sensor_1_first_value);       //variable for plotting
  Serial.print(",");              //seperator
  Serial.println(time_ir_sensor_2_last_value-time_ir_sensor_1_first_value);     //variable for plotting      

  if (is_metal)
  {
    is_metal=false;
  }
  else
  {
    Coin_Sorter(type_invalid_metal);
    return;
  }

  EEPROMMemoryValue += Get_Inserted_Value();
  EEPROM.write(adress_current_inserted_value, EEPROMMemoryValue);
  Write_To_LCD("Du har " + String(EEPROMMemoryValue) + "kr ", 0, 1);
}

byte Get_Inserted_Value()
{
  time_coin_movement = time_ir_sensor_1_last_value - time_ir_sensor_2_last_value; // hur länge det tog coinet att flytta sig från sensor 1 till 2
  time_coin_thickness = time_ir_sensor_1_first_value - time_ir_sensor_1_last_value; // hur länge sensor 1 har sett coinet

  Serial.println("Get_Inserted_Value");
  last_unique_message = millis();
  if (false)
  {
    Add_To_EEPROM_Coin_Memory(adress_coin_1, type_coin_1);
    Coin_Sorter(type_coin_1);
    Write_To_LCD("+" + String(type_coin_1) + "kr", 0, 0);
    return type_coin_1;
  }
  else if (false)
  {
    Add_To_EEPROM_Coin_Memory(adress_coin_2, type_coin_2);
    Coin_Sorter(type_coin_2);
    Write_To_LCD("+" + String(type_coin_2) + "kr", 0, 0);
    return type_coin_2;
  }
  else if (true)
  {
    Add_To_EEPROM_Coin_Memory(adress_coin_5, type_coin_5);
    Coin_Sorter(type_coin_5);
    Write_To_LCD("+" + String(type_coin_5) + "kr", 0, 0);
    return type_coin_5;
  }
  else if(false)
  {
    Add_To_EEPROM_Coin_Memory(adress_coin_10, type_coin_10);
    Coin_Sorter(type_coin_10);
    Write_To_LCD("+" + String(type_coin_10) + "kr", 0, 0);
    return type_coin_10;
  }
  else
  {
    Coin_Sorter(type_invalid);
    Write_To_LCD("Ogiltig peng", 0, 0);
    return type_invalid;
  }
}

void Add_To_EEPROM_Coin_Memory(byte address, coin_type type)
{
  byte value = EEPROM.read(address);
  if (value + 1 > max_coin_in_storage)
  {
    Stash_Coin(type);
  }
  else
  {
    EEPROM.write(address, value + 1);
  }
}

void Stash_Coin(coin_type type)
{
  // ta bort 1 coin av typen type ner till big box of storage
  switch (type)
  {
    case type_coin_1:
      break;
    case type_coin_2:
      break;
    case type_coin_5:
      break;
    case type_coin_10:
      break;
  }
}

void Coin_Sorter(coin_type type)
{
  switch (type)
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
  last_unique_message = millis();
  Serial.println("Return_Money");
  Write_To_LCD("Returnerar " + String(money_to_return) + "kr", 0, 1);
  while (money_to_return != 0)
  {
    if (money_to_return >= 10){
      Return_Coin(type_coin_10);
      money_to_return -= 10;
    }
    else if (money_to_return >= 5)
    {
      Return_Coin(type_coin_5);
      money_to_return -= 5;
    }
    else if (money_to_return >= 2)
    {
      Return_Coin(type_coin_2);
      money_to_return -= 2;
    }
    else
    {
      Return_Coin(type_coin_1);
      money_to_return -= 1;
    }
  }
  EEPROM.write(adress_current_inserted_value, 0);
}

void Return_Coin(coin_type type)
{
  // motor functions to return sed money
}

// BUTTON functions
bool Button_Search()
{
  if (WTF)
  {
    button_value = 255;
    button_is_down = false;
    return false;
  }
  
  for (byte search_index = 0; search_index < 1; search_index++) // 10 123 ABC
  {
    if (!digitalRead(button_pin_start + search_index))
    {
      if (button_is_down)
      {
        return false;
      }
      button_value = search_index;
      button_is_down = true;
      Serial.println("is true WTF WTF");
      return true;
    }
  }
  button_value = 255;
  button_is_down = false;
  return false;
}

bool Motor_Search()
{
  for (byte y = 0; y < 5; y++)
  {
    for (byte x = 0; x < 4; x++)
    {
      if (motor_matrix[y][x] == button_value){
        if (cost_matrix[y][x] > EEPROMMemoryValue)
        {
          Serial.println("Motor_Search");
          last_unique_message = millis();
          Write_To_LCD("Kostnad " + String(cost_matrix[y][x]) + "kr", 0, 0);
          Write_To_LCD("Saknar " + String(cost_matrix[y][x] - EEPROMMemoryValue) + "kr", 0, 1);

          return false;
        }
        else if (Return_Items(y, x))
        {
          EEPROMMemoryValue -= cost_matrix[y][x];
          EEPROM.write(adress_current_inserted_value, EEPROMMemoryValue);

          motor_ground = y;
          motor_power = x;

          Serial.println("Motor_Search");
          last_unique_message = millis();
          Write_To_LCD("Bra kop!", 0, 0);
          return true;
        }
      }
    }
  }

  Serial.println("Motor_Search");
  last_unique_message = millis();
//  Write_To_LCD("Sokt produkt ar", 0, 0);
//  Write_To_LCD("slut i lager", 0, 1);  

  motor_ground = 255;
  motor_power = 255;
  return false;
}

bool Return_Items(byte y, byte x)
{
  Write_To_LCD("Bra kop!", 0, 0);
  Write_To_LCD("Returnerar " + String(0) + "kr", 0, 1);
  EEPROM.write(adress_current_inserted_value, 0);
  Start_Motor(y, x);
  delay(200);
  Stop_Motor(y, x);
  
  return;
  
  y += motor_ground_pin_start;
  x += motor_power_pin_start;

  motor_time_start = millis();
  unsigned long time_1 = 0;

  Start_Motor(y, x);

  do
  {
    time = millis() - motor_time_start;

    if (time_1 >= motor_time_on){
      Stop_Motor(y, x);
    }

    ir_item_val = digitalRead(ir_item_pin);
    if (ir_item_val)
    {
      return true;
    }

  } while (time_1 < item_sensor_delay);

  return true;
}

void Start_Motor(byte y, byte x)
{
  Serial.println("StartMotor");
  WTF = true;
  //digitalWrite(motor_ground_pin_start + y, HIGH);
  //digitalWrite(motor_power_pin_start + x, HIGH);
  for (byte i = 0; i <= 4; i++)
  {
    digitalWrite(motor_ground_pin_start + i, HIGH);
  }
  for (byte i = 0; i <= 3; i++)
  {
    digitalWrite(motor_power_pin_start + i, HIGH);
  }
}

void Stop_Motor(byte y, byte x)
{
  Serial.println("StopMotor");
  for (byte i = 0; i <= 4; i++)
  {
    digitalWrite(motor_ground_pin_start + i, LOW);
  }
  for (byte i = 0; i <= 3; i++)
  {
    digitalWrite(motor_power_pin_start + i, LOW);
  }
}

// LCD
void Write_To_LCD(String message, byte col, byte row)
{
  if (row == 0 && message0 != message)
  {
    lcd.setCursor(col, row);
    lcd.print(message+"                ");
    message0 = message;
    return;
  }
  if (row == 1 && message1 != message)
  {
    lcd.setCursor(col, row);
    lcd.print(message+"                ");
    message1 = message;
    return;
  }
}
