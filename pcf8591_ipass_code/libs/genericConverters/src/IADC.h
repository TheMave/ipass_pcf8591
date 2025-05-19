#pragma once
#include <Arduino.h>
#include <IStreaming.h>

namespace hogeschool
{
    class IADC : public IStreaming
    {
        public:
            virtual uint16_t readADC (uint8_t adcIndex, bool bStreaming) = 0;
            virtual int16_t readADCsCyclical (uint8_t& adcThatWasRead) = 0;
            virtual uint8_t getNofADCs() = 0;
            virtual void endStreaming() = 0;
    };
}