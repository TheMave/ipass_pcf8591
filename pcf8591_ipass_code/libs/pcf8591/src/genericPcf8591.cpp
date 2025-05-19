#include <Arduino.h>
#include <genericPcf8591.h>

namespace hogeschool
{
    GenericPCF8591::GenericPCF8591(uint8_t bitsA2A1A0)
    : _pcf8591(bitsA2A1A0)
    {
    }

    // IADC
    uint16_t GenericPCF8591::readADC (uint8_t adcIndex, bool bStreaming)
    {
        _pcf8591.setAinToChannelMapping(PCF8591::AinToChannelMapping::AIN_N_TO_CHANNEL_N);
        uint16_t value = _pcf8591.readChannel(adcIndex, bStreaming);
        return value<<bitShift;
    }

    int16_t GenericPCF8591::readADCsCyclical (uint8_t& adcThatWasRead)
    {
        _pcf8591.setAinToChannelMapping(PCF8591::AinToChannelMapping::AIN_N_TO_CHANNEL_N);
        uint16_t value = _pcf8591.readCyclical(adcThatWasRead);
        return value<<bitShift;
    }

    uint8_t GenericPCF8591::getNofADCs()
    {
        return nofADCs;
    }

    // IADCdiff
    uint16_t GenericPCF8591::readADCdiff (uint8_t adcDiffIndex, bool bStreaming)
    {
        _pcf8591.setAinToChannelMapping(PCF8591::AinToChannelMapping::AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2);
        int16_t value = _pcf8591.readChannel(adcDiffIndex, bStreaming);
        return value<<bitShift;
    }

    int16_t GenericPCF8591::readADCdiffsCyclical (uint8_t& adcDiffThatWasRead)
    {
        _pcf8591.setAinToChannelMapping(PCF8591::AinToChannelMapping::AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2);
        int16_t value = _pcf8591.readCyclical(adcDiffThatWasRead);
        return value<<bitShift;
    }

    uint8_t GenericPCF8591::getNofADCdiffs()
    {
        return nofADCdiffs;
    }

    // IDAC
    void GenericPCF8591::writeDAC (uint8_t dacIndex, uint16_t value, bool bStreaming)
    {
        _pcf8591.writeDAC(value>>bitShift, bStreaming);
    }

    void GenericPCF8591::writeDACsCyclical (uint16_t& value)
    {
        // write to the one and only dac of the pcf8591
        _pcf8591.writeDAC(value>>bitShift, /*bStreaming*/ true);
    }

    void GenericPCF8591::disableAnalogOutput()
    {
        _pcf8591.disableAnalogOutput();
    }

    uint8_t GenericPCF8591::getNofDACs()
    {
        return nofDACs;
    }

    // IStreaming
    void GenericPCF8591::endStreaming()
    {
        _pcf8591.endStreaming();
    }
}