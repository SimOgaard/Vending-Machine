#define red_pin A0
#define green_pin A1
#define blue_pin A2
/*
int red_value = 0;
int green_value = 0;
int blue_value = 0;
*/
void setup() {
  Serial.begin(9600);
  
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
}

void loop() {
  int red_value = 0;
  int green_value = 255;
  int blue_value = 0;
  RGB_color(red_value, green_value, blue_value);
  /*
  for (blue_value = 0; blue_value < 255; blue_value++) {
    RGB_color(red_value, green_value, blue_value);
  }
  for (red_value = 255; red_value > 0; red_value--) {
    RGB_color(red_value, green_value, blue_value);
  }
  for (green_value = 0; green_value < 255; green_value++) {
    RGB_color(red_value, green_value, blue_value);    
  }
  for (blue_value = 255; blue_value > 0; blue_value--) {
    RGB_color(red_value, green_value, blue_value);
  }
  for (red_value = 0; red_value < 255; red_value++) {
    RGB_color(red_value, green_value, blue_value);
  }
  for (green_value = 255; green_value > 0; green_value--) {
    RGB_color(red_value, green_value, blue_value);    
  }*/
/*
  int red_value = 0;
  int green_value = 0;
  int blue_value = 0;

  for (red_value = 0; red_value < 255; red_value++) {
    RGB_color(red_value, green_value, blue_value);
  }
  for (green_value = 0; green_value < 255; green_value++) {
    RGB_color(red_value, green_value, blue_value);    
  }
  for (blue_value = 0; blue_value < 255; blue_value++) {
    RGB_color(red_value, green_value, blue_value);
  }
*/
/*
  for (red_value = 0; red_value <= 255; red_value++) {
    for (int green_value = 0; green_value <= 255; green_value++) {
      for (int blue_value = 0; blue_value <= 255; blue_value++) {
        RGB_color(red_value, green_value, blue_value);
      }
    }
  }
*/
/*
  for (int rgb = 0; rgb <= 16777216; rgb++) {
    red_value = (rgb >> 16) & 0xFF;
    green_value = (rgb >> 8) & 0xFF;
    blue_value = (rgb) & 0xFF;

    RGB_color(red_value, green_value, blue_value);
  }
*/
}

void RGB_color(int red_value, int green_value, int blue_value) {
  Serial.println(String(red_value)+"|"+String(green_value)+"|"+String(blue_value));
  delay(50);
  analogWrite(red_pin, red_value);
  analogWrite(green_pin, green_value);
  analogWrite(blue_pin, blue_value);
}

