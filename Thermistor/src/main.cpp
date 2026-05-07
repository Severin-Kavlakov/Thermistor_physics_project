#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

uint8_t buttonPin = A4;
uint8_t tempSensorPin = 9;
uint8_t thermistorPin = A0;

const uint8_t 
  RS = 12, 
  E  = 11,
  D4 = 5,
  D5 = 4,
  D6 = 3, 
  D7 = 2;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

OneWire oneWire(tempSensorPin);      // oneWire instance - communicate with any OneWire device - not just Maxim/Dallas temperature ICs
DallasTemperature sensors(&oneWire); // oneWire reference to Dallas Temperature lib




float value = 0.00; 
float V     = 0.00;

uint16_t RseriesResistor = 10000;
uint16_t R     = 0;
uint32_t RSum  = 0;
uint32_t count = 0;
float    RAvg  = 0;

bool buttonState     = false;
bool buttonStatePrev = false;
uint16_t R1 = 0;
uint16_t R2 = 0;
bool isR1 = true;

float tempC1      = 0.00;
float tempKelvin1 = 0.00;
float tempC2      = 0.00;
float tempKelvin2 = 0.00;


char bufferFloat[5];
char* float_out(float f) {
  dtostrf(f, sizeof(bufferFloat), 2, bufferFloat); //decimals
  return bufferFloat;
}

char bufferUint16[5];
char* uint16_t_out(uint16_t i) {
  sprintf(bufferUint16, "%5d", i);
  return bufferUint16;
}




void setup() {

  Serial.begin(9600);
  pinMode(thermistorPin, INPUT);
  lcd.begin(16, 2);

  lcd.setCursor(0, 0); lcd.print("R1 ");
  lcd.setCursor(0, 1); lcd.print("R2 ");

  sensors.begin(); // Start temperature sensor lib

  delay(1000); //debigging

}




void loop() {
  value = analogRead(thermistorPin);
  V = value * (5.0 / 1023.0);
  R = RseriesResistor * ((1023.0 / value) - 1);

  




  if (analogRead(buttonPin) > 700) { buttonState = true; Serial.println("BUTTON PRESS!  BUTTON PRESS!  BUTTON PRESS!  "); }
  else                               buttonState = false;

  if (buttonState == false && buttonStatePrev == true) { // button pressed - print current avg

    if (isR1) { // if FIRST period 
      R1 = RAvg;
      lcd.setCursor(3, 0); lcd.print(uint16_t_out(R1));

      sensors.requestTemperatures();
      tempC1 = sensors.getTempCByIndex(0);
      tempKelvin1 = tempC1 + 273.15;

      lcd.setCursor(10, 0); lcd.print(tempKelvin1);

      RSum = 0;
      count = 0;
      RAvg = R; // recalculate average from now on

      isR1 = false;
    }

    else { // if SECOND period
      R2 = RAvg;
      lcd.setCursor(3, 1); lcd.print(uint16_t_out(R2));

      sensors.requestTemperatures();
      tempC2 = sensors.getTempCByIndex(0);
      tempKelvin2 = tempC2 + 273.15;

      lcd.setCursor(10, 1); lcd.print(tempKelvin2);

      RSum = 0;
      count = 0;
      RAvg = R; // recalculate average from now on

      isR1 = true;
    }
  }

  else { // button not pressed - calculate average
    RSum += R;
    count++;
    RAvg = RSum/count;
  }

  buttonStatePrev = buttonState; // 2nd sample of button to check if pressed 


  delay(50); // display frame









  Serial.print(float_out(value)); Serial.print(" analog");
  Serial.print("  ");
  Serial.print(float_out(V)); Serial.print(" V"); 

  Serial.print("      ");

  Serial.print(uint16_t_out(R)); Serial.print(" R");
  Serial.print("  ");
  Serial.print(uint16_t_out(RAvg)); Serial.print(" avg R");

  Serial.print("      ");

  Serial.print(uint16_t_out(R1)); Serial.print(" R1");
  Serial.print("  ");
  Serial.print(uint16_t_out(R2)); Serial.print(" R2");

  Serial.print("      ");

  Serial.print(float_out(tempKelvin1)); Serial.print(" K1");
  Serial.print("  ");
  Serial.print(float_out(tempKelvin2)); Serial.print(" K2");





  
  Serial.println("");
}
