#pragma once
#include <Arduino.h>

namespace hogeschool
{
    class SerialInput
    {
        public:
        inline static bool wachtEnCheckToets(char toets, int nofMilliseconden)
        {
        bool bToetsIngedrukt = false;
        
        for (int j = 0; j < (1+nofMilliseconden/100); j++)
        {
            // controleer elke 100 milliseconden of er een toets
            // is ingedrukt.
            delay(100);
            char c = ' ';
            while (Serial.available() > 0) {
            c = Serial.read();
            if (c == toets)
            {
                bToetsIngedrukt = true;
            }
            }
            if (bToetsIngedrukt) break;
        }
        
        return bToetsIngedrukt;
        }
    };
} // end namespace hogeschool