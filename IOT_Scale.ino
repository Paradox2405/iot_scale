//#include "HX711.h"
#include <ESP8266WiFi.h>;
#include <HX711_ADC.h>
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>

//const int LOADCELL_DOUT_PIN = 14;
//const int LOADCELL_SCK_PIN = 12;
//LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "akila"; //Your Network SSID
const char* password = "abcd1234"; //Your Network Password

WiFiClient client;
 
unsigned long myChannelNumber = 1382053;            //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "YK27OP1RWOH78AZE";   //Your Write API Key

HX711_ADC LoadCell(5,4);

float calibration_factor = -90000; // this calibration factor is adjusted according to my load cell
float units;
//float ounces;

void setup() 
{
  Serial.begin(9600);
    //scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    LoadCell.begin();
  //Wire.begin(D2, D1);
  //lcd.begin();
  //lcd.print("Circuit Digest");
  //lcd.setCursor(0,1);
  //lcd.print("NodeMCU Wg Scale");
  
  delay(2000);

  //lcd.clear();

  //lcd.print("Connecting Wifi");
 // lcd.setCursor(0,1);
  //lcd.print("And Thingsspeak");
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  
  delay(3000);
  
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
LoadCell.start(stabilisingtime);
  //scale.set_scale();
  LoadCell.setCalFactor(calibration_factor); // user set calibration factor (float)
  //scale.tare();  //Reset the scale to 0

  //long zero_factor = scale.read_average(); //Get a baseline reading
  //Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
 // Serial.println(zero_factor);
}

void loop() 

{
  LoadCell.update();
 // scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  //lcd.clear();
    
  units = LoadCell.getData();
      Serial.print("HX711 reading: ");
    Serial.println(units);
  if (units < 0)
  {
    units = 0;
  }
 
  if (units == 0)
  {
  // units=34;
       //lcd.print("Weight :");
       //lcd.print(units);
       //lcd.print("  g");
       ThingSpeak.writeField(myChannelNumber, 1,units, myWriteAPIKey); //Update in ThingSpeak 
       Serial.print(units);
       Serial.print(" grams"); 
       Serial.print(" calibration_factor: ");
       Serial.print(calibration_factor);
       Serial.println();
       delay(15000);
   } 
  
  if (units > 0)
  {
       //lcd.print("Weight :");
       //lcd.print(units);
       //lcd.print("  g");
       ThingSpeak.writeField(myChannelNumber, 1,units, myWriteAPIKey); //Update in ThingSpeak 
       Serial.print(units);
       Serial.print(" grams"); 
       Serial.print(" calibration_factor: ");
       Serial.print(calibration_factor);
       Serial.println();
       delay(15000);
  } 

 
//  if(Serial.available())
//  {
//    char temp = Serial.read();
//    if(temp == '+' || temp == 'a')
//      calibration_factor += 1;
//    else if(temp == '-' || temp == 'z')
//      calibration_factor -= 1;
//  }
}
