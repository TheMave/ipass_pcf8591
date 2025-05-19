#include "Arduino.h"
#include <Wire.h>
#include <pcf8591.h>
#include <serialInput.h>

// We willen geen magic constants, dus definiëren we alle constants hier.
const uint8_t   PCF8591_address = 0x48; // Mits A0, A1 en A2 met ground 
                                        // verbonden zijn.
const int       serialBaudRate  = 9600;
const int       maxNofChannels  = 4;    // Dit is het aantal kanalen van de 

namespace hs = hogeschool;  // shorthand
const uint8_t nA2A1A0_adresSelectorBits = 0b000;
hs::PCF8591 pcf8591(nA2A1A0_adresSelectorBits);

void testDACnaarADCloopback()
{
  Serial.println();
  Serial.println("*********************************************");
  Serial.println("Test: DAC naar ADC loopback");
  Serial.println("Verbind AIN0 met AOUT en type 'v' voor verder");
  Serial.println("*********************************************");

  pcf8591.setAinToChannelMapping(hs::PCF8591::AinToChannelMapping::AIN_N_TO_CHANNEL_N);

  bool bStopToetsIngedrukt = false;
  while(true)
  {
    if(hs::SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000))
    {
      break;
    }
  }
  bStopToetsIngedrukt = false;

  while(true)  
  {
    Serial.println();
    Serial.println("Starting from 0:");
    for(uint8_t value = 0; value < 255; value+= 10)
    {
      pcf8591.writeDAC(value, /*bStreaming*/ false);
      uint8_t valueRead = pcf8591.readChannel(0);
            
      // Print de waarde naar de serial monitor
      Serial.print("Value sent to DAC: ");
      Serial.println(value);
      Serial.print("Value read from AIN0: ");
      Serial.println(valueRead);
      
      if(abs(int(value)-int(valueRead)) >= 6)
      {
        Serial.print("Het verschil is te groot, er is iets mis!");
      }
      Serial.println("--------");

      // Wacht even, anders komt er meer data naar de serial monitor dan die
      // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
      bStopToetsIngedrukt = hs::SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000);
      if(bStopToetsIngedrukt) break;
    }
    if(bStopToetsIngedrukt) break;
  }
}

void testGestreamedZendenNaarDAC()
{
  Serial.println();
  Serial.println("********************************");
  Serial.println("Test: Gestreamed zenden naar DAC");
  Serial.println("********************************");

  Serial.println("");
  Serial.println("Hang een oscilloscoop aan AOUT.");
  Serial.println("Als het goed is, is daar straks periodiek");
  Serial.println("een zaagtand van ongeveer 1kHz te meten.");
  Serial.println("druk op 'v' de zaagtand te starten.");

  bool bStopToetsIngedrukt = false;
  while(true)
  {
    if(hs::SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000))
    {
      break;
    }
  }
  bStopToetsIngedrukt = false;

  // fsample = 11.1kHz, dus zijn 11 samples per periode van 1kHz mogelijk.
  int aantalSamplesPerCyclus = 11; 
  int stepSize = 255/aantalSamplesPerCyclus;
  int n = 0;

  while(true)  
  {
    // 1000 1kHz zaagtanden in 1 seconde.
    for(int i = 0; i < 2000; i++)
    {
      for(uint8_t value = 0; value < 255; value+= stepSize)
      {
        pcf8591.writeDAC(value, /*bStreaming*/ true);  
      }
      n++;
    }
    pcf8591.endStreaming();

    Serial.print("Alweer ");
    Serial.print(n);
    Serial.println(" zaagtanden gemaakt");
    // Wacht even, anders komt er meer data naar de serial monitor dan die
    // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
    bStopToetsIngedrukt = hs::SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 1000);
    if(bStopToetsIngedrukt) break;
  }
}

void setup() 
{
  Serial.begin(serialBaudRate);
  Wire.begin();
}

void loop() 
{
  Serial.println("");
  Serial.println("******************");
  Serial.println("******************");
  Serial.println("Start van de Tests");
  Serial.println("******************");
  Serial.println("******************");
  Serial.println("");
  Serial.println("(geef steeds invoer 'v' voor de volgende test)");
  Serial.println("");
  
  testDACnaarADCloopback();
  testGestreamedZendenNaarDAC();
}
