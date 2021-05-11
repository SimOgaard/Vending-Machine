#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 6, 7, 8, 9, 10);

void setup() {
  lcd.begin(16, 2);
  lcd.print("mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
  lcd.setCursor(0, 1);
  lcd.print("foooooooooooooood");
}

void loop() {
}
