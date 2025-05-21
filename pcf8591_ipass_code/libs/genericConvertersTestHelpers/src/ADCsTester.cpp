#include "Arduino.h"
#include "ADCsTester.h"
#include <Wire.h>
#include <pcf8591.h>
#include <serialInput.h>

namespace hogeschool
{
    ADCsTester::ADCsTester(IADC& adcChip, uint8_t nofAdcs):_adcChip(adcChip),_nofAdcs(nofAdcs){}

    void ADCsTester::testApartLezenVanADCs()
    {
        Serial.println();
        Serial.println("**************************");
        Serial.println("Test: Apart lezen van ADCs");
        Serial.println("**************************");

        while(true)  
        {
            for (int i = 0; i < _nofAdcs; i++) 
            {
                uint16_t value = _adcChip.readADC(/*adcIndex*/ i, /*bStreaming*/ false);
                        
                // Print de waarde naar de serial monitor
                Serial.print("AIN");
                Serial.print(i);
                Serial.print(": ");
                Serial.println(value);
                if((i%4)==3) Serial.println("--------");
            }

            // Wacht even, anders komt er meer data naar de serial monitor dan die
            // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
            bool bStopToetsIngedrukt = SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000);
            if(bStopToetsIngedrukt) break;
        }
    }

    void ADCsTester::testCyclischLezenVanADCs()
    {
        Serial.println();
        Serial.println("*****************************");
        Serial.println("Test: Cyclisch lezen van ADCs");
        Serial.println("*****************************");

        bool bStopToetsIngedrukt = false;
        while(true)  
        {
            uint8_t nChannel = 0;
            
            uint16_t value = _adcChip.readADCsCyclical(nChannel);
                
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
                bStopToetsIngedrukt = SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000);
            }

            if(bStopToetsIngedrukt) break;
        }
    }

    void ADCsTester::testGestreamedLezenVanADCs()
    {
        Serial.println();
        Serial.println("*******************************");
        Serial.println("Test: Gestreamed lezen van ADCs");
        Serial.println("*******************************");

        bool bStopToetsIngedrukt = false;
        while(true)  
        {
            for (int nChannel = 0; nChannel < _nofAdcs; nChannel++)
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
                    uint16_t value = _adcChip.readADC(nChannel,/*bStreaming*/true);
                        
                    // Print de waarde naar de serial monitor
                    Serial.print("AIN");
                    Serial.print(nChannel);
                    Serial.print(": ");
                    Serial.println(value);
                }
                _adcChip.endStreaming();
                
                // Wacht even, anders komt er meer data naar de serial monitor dan die
                // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
                bStopToetsIngedrukt = SerialInput::wachtEnCheckToets(/*toets*/ 'v', /*nofMilliseconden*/ 2000);
                if(bStopToetsIngedrukt) break;
            }
            if(bStopToetsIngedrukt) break;
        }
    }

    void ADCsTester::setup() 
    {
        const int serialBaudRate  = 9600;
        Serial.begin(serialBaudRate);
        Wire.begin();
    }

    void ADCsTester::loop() 
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
    }
} // end namespace hogeschool