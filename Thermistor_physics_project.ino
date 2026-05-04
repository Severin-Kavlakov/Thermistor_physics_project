/* 1602 LCD pinout
  RS -     mega D7 PWM
  Enable - mega D8 PWM
  D4 - 	   mega D9 PWM
  D5 - 	   mega D10 PWM SS
  D6 - 	   mega D11 PWM MOSI
  D7 - 	   mega D12 PWM MISO 
  R/W - GND
  VSS - GND
  VCC - +5v 
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //init library with interface pins */

#include <LiquidCrystal.h>
#include <Arduino.h>
const uint8_t RS = 12,
              Enable = 11,
              D4 = 5,
              D5 = 4,
              D6 = 3,
              D7 = 2;
LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7);

uint8_t  thermistorPin = A1;
uint16_t thermVal = 0, maxThermVal = 0;

void setup() {
  pinMode(thermistorPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  thermVal = analogRead(thermistorPin);

  if (thermVal > maxThermVal) {
    maxThermVal = thermVal;
  }
  
  lcd.setCursor(0, 0); lcd.print(thermVal);
  lcd.setCursor(0, 1); lcd.print(maxThermVal);

  Serial.print(thermVal); Serial.print("    "); Serial.println(maxThermVal);

  delay(100);

  lcd.setCursor(0, 0); lcd.print("    "); //clear first row
}





