#include <Arduino.h>
#include <LiquidCrystal.h>
const uint8_t 
  RS = 12 ,
  E  = 11 ,
  D4 = 5  ,
  D5 = 4  ,
  D6 = 3  ,
  D7 = 2  ;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);




uint8_t thermistorPin = A0;

float value    = 0.00; 
float maxValue = 0.00;
float V        = 0.00;
float Vmax     = 0.00;

uint16_t R    = 0;
uint16_t Rmin = 65536;

uint16_t Rprev = 0;

uint16_t RAvg  = 0;
uint32_t Rsum  = 0;
uint16_t R1    = 0;
uint16_t R2    = 0;
uint32_t count = 0;
bool  isFirstCalculation = 1;

uint16_t RdeltaThreshold = 400;
uint16_t RseriesResistor = 10000;
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

delay(500); //not sure if will work BUT supposed to wait to NOT have false positive cahnges in R delta

}




void loop() {

value = analogRead(thermistorPin);
if (value > maxValue) maxValue = value;

V = value * (5.0 / 1023.0);
if (V > Vmax) Vmax = V;

R = RseriesResistor * ((1023 / value) - 1);
if (R < Rmin) Rmin = R;




if(abs(Rprev - R) > RdeltaThreshold) { // if big change in R - assign averages

  if(count > 0) { // if it's NOT the first read  //prevents divisions by 0  //could increase this 
    if(isFirstCalculation) {
      R1 = Rsum/count;
      isFirstCalculation = false;
    }
    else {
      R2 = Rsum/count;
      isFirstCalculation = true;
    }
  }

  //reset for next stable period 
  Rsum = R;
  count = 1;

  Rprev = R;
}

else { // if stable period - accumulate for averages
  Rsum += R;
  count++;

  //RAvg = Rsum/count; //for debugging ONLY - ALL TIME average

  Rprev = R;
}




lcd.setCursor(5, 0); lcd.print(format_uint16(R1));
lcd.setCursor(5, 1); lcd.print(format_uint16(R2));
delay(50); // display frame




Serial.print(value); Serial.print(" value");
Serial.print("  ");
Serial.print(maxValue); Serial.print(" max value"); 

Serial.print("      ");

Serial.print(V); Serial.print(" V"); 
Serial.print("  "); 
Serial.print(Vmax); Serial.print(" max V"); 

Serial.print("      ");

Serial.print(R); Serial.print(" ohm");
Serial.print("  ");
Serial.print(Rmin); Serial.print(" min ohm"); 

Serial.print("      ");

Serial.print(RAvg); Serial.print(" avg ohm");




Serial.println("");
}
