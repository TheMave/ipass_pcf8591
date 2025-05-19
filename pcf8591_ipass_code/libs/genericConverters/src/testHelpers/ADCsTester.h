#pragma once
#include "Arduino.h"
#include "IADC.h"

namespace hogeschool
{
    class ADCsTester
    {
    private:
        IADC& _adcChip;
        uint8_t _nofAdcs;
        void testApartLezenVanADCs();
        void testCyclischLezenVanADCs();
        void testGestreamedLezenVanADCs();

    public:
        ADCsTester(IADC& adcChip, uint8_t nofAdcs);
        void setup();
        void loop();
    };
}