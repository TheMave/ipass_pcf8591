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

void testApartLezenVanADCs()
{
  Serial.println();
  Serial.println("**************************");
  Serial.println("Test: Apart lezen van ADCs");
  Serial.println("**************************");

  pcf8591.setAinToChannelMapping(hs::PCF8591::AinToChannelMapping::AIN_N_TO_CHANNEL_N);

  while(true)  
  {
    for (int i = 0; i < maxNofChannels; i++) 
    {
      uint8_t value = pcf8591.readChannel(i);
            
      // Print de waarde naar de serial monitor
      Serial.print("AIN");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(value);
      if((i%4)==3) Serial.println("--------");
    }

    // Wacht even, anders komt er meer data naar de serial monitor dan die
    // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
    bool bStopToetsIngedrukt = hs::SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000);
    if(bStopToetsIngedrukt) break;
  }
}

void testCyclischLezenVanADCs()
{
  Serial.println();
  Serial.println("*****************************");
  Serial.println("Test: Cyclisch lezen van ADCs");
  Serial.println("*****************************");

  pcf8591.setAinToChannelMapping(hs::PCF8591::AinToChannelMapping::AIN_N_TO_CHANNEL_N);

  bool bStopToetsIngedrukt = false;
  while(true)  
  {
    uint8_t nChannel=0;
    
    uint8_t value = pcf8591.readCyclical(nChannel);
          
    // Print de waarde naar de serial monitor
    Serial.print("AIN");
    Serial.print(nChannel);
    Serial.print(": ");
    Serial.println(value);

    if(nChannel==3) 
    {
      Serial.println("--------");
      // Wacht even, anders komt er meer data naar de serial monitor dan die
      // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
      bStopToetsIngedrukt = hs::SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000);
    }

    if(bStopToetsIngedrukt) break;
  }
}

void testGestreamedLezenVanADCs()
{
  Serial.println();
  Serial.println("*******************************");
  Serial.println("Test: Gestreamed lezen van ADCs");
  Serial.println("*******************************");

  pcf8591.setAinToChannelMapping(hs::PCF8591::AinToChannelMapping::AIN_N_TO_CHANNEL_N);

  bool bStopToetsIngedrukt = false;
  while(true)  
  {
    for (int nChannel = 0; nChannel < maxNofChannels; nChannel++)
    {
      // Laten we steeds een 5x het betreffende kanaal gestreamed uitlezen.
      Serial.println("----------------------------");
      Serial.print("5x AN");
      Serial.print(nChannel);
      Serial.println(" gestreamed lezen:");
      for (int j = 0; j < 5; j++) 
      {
        // Bij de huidige mapping is de index van het kanaal gelijk aan die
        // van de Analog input pin.
        uint8_t value = pcf8591.readChannel(nChannel,/*bStreaming*/true);
              
        // Print de waarde naar de serial monitor
        Serial.print("AIN");
        Serial.print(nChannel);
        Serial.print(": ");
        Serial.println(value);
      }
      pcf8591.endStreaming();
      
      // Wacht even, anders komt er meer data naar de serial monitor dan die
      // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
      bStopToetsIngedrukt = hs::SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000);
      if(bStopToetsIngedrukt) break;
    }
    if(bStopToetsIngedrukt) break;
  }
}

// signedChannels is een bitmasker dat aangeeft welke channels een diff resultaat leveren
// en dus potentieel negatief. 
// signedChannels = 0b101 betekent dus: channel 0 en channel 2 retourneren 2's complement (signed) integers.
void test_mapping(hs::PCF8591::AinToChannelMapping testMapping, const char* strMapping, int nofChannels, uint8_t signedChannels)
{
  Serial.println();
  Serial.println("****************************************************");
  Serial.print("Test: lezen met ");
  Serial.println(strMapping);
  Serial.println("****************************************************");

  while(true)  
  {
    Serial.println("Eerst lezen we de kanalen apart, dus nog met");
    Serial.println("AIN_N_TO_CHANNEL_N mapping.");
    
    pcf8591.setAinToChannelMapping(hs::PCF8591::AinToChannelMapping::AIN_N_TO_CHANNEL_N);
    
    for (int i = 0; i < maxNofChannels; i++) 
    {
      uint8_t value = pcf8591.readChannel(i);
            
      // Print de waarde naar de serial monitor
      Serial.print("AIN");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(value);
    }

    pcf8591.setAinToChannelMapping(testMapping);
    
    Serial.print("Dit worden dan de kanalen met mapping ");
    Serial.println(strMapping);
    for (int i = 0; i < nofChannels; i++) 
    {
      // nb kanaal is nu diff ingesteld. kan dus negatief worden.
      uint8_t value = pcf8591.readChannel(i);

      // Print de waarde naar de serial monitor
      Serial.print("Kanaal");
      Serial.print(i);
      Serial.print(": ");

      if((1<<i) & signedChannels)
      {
        Serial.println(int8_t(value));
      }
      else
      {
        Serial.println(value);
      }
    }
    
    Serial.println("--------");

    // Wacht even, anders komt er meer data naar de serial monitor dan die
    // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
    bool bStopToetsIngedrukt = hs::SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000);
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

  testApartLezenVanADCs();
  testGestreamedLezenVanADCs();
  testCyclischLezenVanADCs();
  test_mapping(hs::PCF8591::AinToChannelMapping::AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2,"AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2",/*nofChannels*/3,/*signedChannels*/0b111);
  test_mapping(hs::PCF8591::AinToChannelMapping::AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1,"AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1",/*nofChannels*/2,/*signedChannels*/0b11);
  test_mapping(hs::PCF8591::AinToChannelMapping::AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2,"AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2",/*nofChannels*/3,/*signedChannels*/0b100);
}
