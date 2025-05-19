#include "Arduino.h"
#include <Wire.h>
#include <pcf8591.h>
#include <serialInput.h>
#include <testHelpers/DACsTester.h>

namespace hogeschool
{
  DACsTester::DACsTester(IDAC& dacChip, uint8_t nofDacs, uint16_t sampleFreqKhz)
  : _dacChip(dacChip),_nofDacs(nofDacs),_sampleFreqKhz(sampleFreqKhz) {}

  void DACsTester::testGestreamedZendenNaarDAC()
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
      if(SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000))
      {
        break;
      }
    }
    bStopToetsIngedrukt = false;

    // We willen een 1kHz zaagtand maken, dus dan geldt:
    int aantalSamplesPerCyclus = _sampleFreqKhz; 
    uint16_t maxValue = 0xFFFF; // Want de generic DAC gaat uit van 16 bits. 
    uint8_t stepSize  = maxValue/aantalSamplesPerCyclus;
    int n = 0;

    while(true)  
    {
      for(int indexDAC = 0; indexDAC < _nofDacs; indexDAC++)
      {
        Serial.print("Zaagtand voor DAC ");
        Serial.println(indexDAC);
        
        // 1000 1kHz zaagtanden gedurende 1 seconde.
        for(int i = 0; i < 2000; i++)
        {
          for(uint8_t value = 0; value < 255; value+= stepSize)
          {
            _dacChip.writeDAC(indexDAC, value, /*bStreaming*/ true);  
          }
          n++;
        }
        _dacChip.endStreaming();
      }

      Serial.print("Alweer ");
      Serial.print(n);
      Serial.println(" zaagtanden gemaakt");
      // Wacht even, anders komt er meer data naar de serial monitor dan die
      // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
      bStopToetsIngedrukt = SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 1000);
      if(bStopToetsIngedrukt) break;
    }
  }

  void DACsTester::setup() 
  {
    const int serialBaudRate  = 9600;
    Serial.begin(serialBaudRate);
    Wire.begin();
  }

  void DACsTester::loop() 
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
    
    testGestreamedZendenNaarDAC();
  }
}// end namespace hogeschool