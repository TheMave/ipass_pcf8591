#pragma once
#include "Arduino.h"
#include "IDAC.h"

namespace hogeschool
{
    class DACsTester
    {
    private:
        IDAC& _dacChip;
        uint8_t _nofDacs;
        uint16_t _sampleFreqKhz;
        void testGestreamedZendenNaarDAC();

    public:
        DACsTester(IDAC& dacChip, uint8_t nofDacs, uint16_t sampleFreqKhz);
        void setup();
        void loop();
    };
}
