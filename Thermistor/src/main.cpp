#include <Arduino.h>
#include <LiquidCrystal.h>

const uint8_t RS = 12,
          Enable = 11,
              D4 = 5,
              D5 = 4,
              D6 = 3,
              D7 = 2;
LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7);

uint8_t thermistorPin = A0;

float thermVal      = 0.00, maxThermVal     = 0.00;
float thermVoltage  = 0.00, maxThermVoltage = 0.00;

uint16_t R     = 0;
uint16_t minR  = 15000;

uint16_t prevR   = 0;
uint16_t avgR    = 0;

uint32_t sumR    = 0;
uint32_t counter = 0;

uint16_t RdeltaThreshold = 400;

uint16_t SERIESRESISTOR = 10000;
/* const uint16_t NOMINAL_R 10000
const uint8_t NOMINAL_TEMPERATURE 25
const uint16_t BCOEFFICIENT 3950 */




char floatOut[5];
char* format_float(float f) {
  dtostrf(f, sizeof(floatOut), 2, floatOut); //decimals
  return floatOut;
}

char uint16Out[5];
char* format_uint16(uint16_t i) {
  sprintf(uint16Out, "%5d", i);
  return uint16Out;
}




void setup() {
  Serial.begin(9600);
  pinMode(thermistorPin, INPUT);
  lcd.begin(16, 2);

  lcd.setCursor(0, 0); lcd.print("T1 R=");
  lcd.setCursor(0, 1); lcd.print("T2 R=");
}

void loop() {
  
  thermVal = analogRead(thermistorPin);
  if (thermVal > maxThermVal) maxThermVal = thermVal;

  thermVoltage = thermVal * (5.0 / 1023.0);
  if (thermVoltage > maxThermVoltage) maxThermVoltage = thermVoltage;

  R = SERIESRESISTOR * ((1023 / thermVal) - 1);
  if (R < minR) minR = R;



  //if(abs(prevR) - abs(R) > RdeltaThreshold) {}
  
  sumR += R;
  counter++;
  avgR = sumR/counter;

  
  prevR = R;




  lcd.setCursor(5, 0); lcd.print(format_uint16(avgR));
  //lcd.setCursor(5, 1); lcd.print(format_uint16());

  delay(50); // display frame


  Serial.print(thermVal    ); Serial.print(" value"); Serial.print("  "); Serial.print(maxThermVal    ); Serial.print  (" max value"); 
  Serial.print("      ");
  Serial.print(thermVoltage); Serial.print(" V"    ); Serial.print("  "); Serial.print(maxThermVoltage); Serial.print  (" max V"    ); 
  Serial.print("      ");
  Serial.print(R  ); Serial.print(" ohm"  ); Serial.print("  "); Serial.print(minR  ); Serial.println(" min ohm"  ); 
}
