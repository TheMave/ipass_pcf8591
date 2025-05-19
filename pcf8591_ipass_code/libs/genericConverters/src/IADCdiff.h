#pragma once
#include <Arduino.h>
#include <IStreaming.h>

namespace hogeschool
{
    class IADCdiff : public IStreaming
    {
        public:
            virtual uint16_t readADCdiff (uint8_t adcDiffIndex, bool bStreaming) = 0;
            virtual int16_t readADCdiffsCyclical (uint8_t& adcDiffThatWasRead) = 0;
            virtual uint8_t getNofADCdiffs() = 0;
            virtual void endStreaming() = 0;
    };
}