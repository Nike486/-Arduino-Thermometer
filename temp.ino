#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "GyverButton.h"

LiquidCrystal_I2C myLCD (0x27, 16, 2);
DHT myDHT(3, DHT22);
GButton button(2);

void CheckBacklight(bool &status) noexcept
{
  if(!status)
  {
    status = true;
    myLCD.backlight();  
  }
  else if(status)
  {
    status = false;
    myLCD.noBacklight();    
  }
}

void PrintLCD () noexcept
{
  float t = myDHT.readTemperature();
  float h = myDHT.readHumidity();

  if (isnan(t) || isnan(h)) 
  {
    myLCD.setCursor(0, 0);
    myLCD.print("Failed");
    return;
  }

  myLCD.setCursor(0, 0);
  String strT = "t: " + (String)t;
  myLCD.print(strT);
  myLCD.setCursor(8, 0);
  myLCD.write(0);
  myLCD.print("C");

  myLCD.setCursor(0, 1);
  String strH = "h: " + (String)h + "%";
  myLCD.print(strH); 
} 

void setup() 
{
  constexpr const byte charDegree[] = 
  {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };  

  myLCD.begin(16,2); 
  myLCD.noBacklight();
  myLCD.createChar(0, charDegree);  

  myDHT.begin(); 

  button.setDebounce(50);

  PrintLCD(); 
}

void loop() 
{
  button.tick();
  static bool backlightStatus = false;
  bool buttonStatus = button.isPress();

  if(buttonStatus) { CheckBacklight(backlightStatus); }  

  auto currentTime = millis();
  static unsigned long lastTime = 0;

  if((currentTime - lastTime) >= 2000)
  {
    lastTime = currentTime;
    PrintLCD();    
  }  
}