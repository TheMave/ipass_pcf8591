#pragma once
#include <Arduino.h>
#include <IStreaming.h>

namespace hogeschool
{
    class IDAC : public IStreaming
    {
        public:
            virtual void writeDAC (uint8_t dacIndex, uint16_t value, bool bStreaming) = 0;
            virtual void writeDACsCyclical (uint16_t& value) = 0;
            virtual void disableAnalogOutput() = 0;
            virtual uint8_t getNofDACs() = 0;
            virtual void endStreaming() = 0;
    };
}