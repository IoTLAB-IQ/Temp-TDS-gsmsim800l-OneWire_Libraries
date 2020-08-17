// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>  //TX=2 ; RX=3

#include "SIM900.h"
#include "sms.h"
#include "GravityTDS.h"

#define ONE_WIRE_BUS 5 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

#define TdsSensorPin A1
GravityTDS gravityTds;
float temperature = 25,tdsValue = 0;
SMSGSM sms;
int numdata;
char smsbuffer[160];
char n[20];

void setup(void) 
{ 
 // start serial port 
 Serial.begin(9600); 
if (gsm.begin(2400)){
    Serial.println("\nstatus=READY");
   (sms.SendSMS("07811601415", " system is ready")) ;//Enable this two lines if you want to send an SMS.
  }
  else Serial.println("\nstatus=IDLE");
     
 
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference  on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
 // Start up the library 
 sensors.begin(); 
} 
void loop(void) 
{ 
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print(tdsValue,0);
    Serial.println("");
    Serial.print("TDS Value:");
    Serial.print(tdsValue,0);
    Serial.println(" PPM");
 Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.print("Temperature is: "); 
 float T = sensors.getTempCByIndex(0);
 Serial.print(T); // Why "byIndex"?  
delay(1000); 

if (tdsValue > 300.0 && T >22.0){
(sms.SendSMS("07811601415", "Worry TDS higher 300 ppm !!")) ;//Enable this two lines if you want to send an SMS.
delay(3000);
}


} 
