#include "pcf8591.h"
#include <Wire.h>
#include <serialAssert.h>

namespace hogeschool{

// private utility functions
uint8_t PCF8591::replaceBits(uint8_t byte, uint8_t mask, uint8_t value)
{
    byte = (byte & ~mask) | (value & mask);
    return byte;
}

void PCF8591::writeControlByte(bool bStopBit)
{
    // Een write van het control byte is altijd het begin van iets.
    // Impliciet beeindigt het dus lopende transmissies.
    endStreaming(); // Stop lopende transmissie, als die er is.
    Wire.beginTransmission(_address);
    Wire.write(_controlByte);
    _lastWrittenControlByte = _controlByte;
    if(bStopBit)
    {
        Wire.endTransmission();
    }
}

void PCF8591::enableAnalogOutput()
{
    _controlByte = _controlByte | analogOutputEnableMask;
    writeControlByte(/*bStopBit*/ true);
}

uint8_t PCF8591::readByte(bool bStopBit)
{
    Wire.requestFrom(_address, static_cast<uint8_t>(1),/*stopbit=*/ static_cast<uint8_t>(bStopBit));
    return Wire.read();
}

uint8_t PCF8591::readSecondByte(bool bStopBit)
{
    // Lees 2 bytes van de PCF8591 in de cache.
    // De eerste byte is de "vorige waarde" van de ADC.
    Wire.requestFrom(_address, static_cast<uint8_t>(2), static_cast<uint8_t>(bStopBit));   // false want streaming.
    Wire.read();        // Discard de eerste byte (vorige waarde)
    return Wire.read(); // Lees de tweede byte (actuele waarde)
}

PCF8591::PCF8591(uint8_t bitsA2A1A0):
    _address(base_address|bitsA2A1A0),
    _channel(0),
    _controlByte(0),
    _bAutoIncrementFlag(false),
    _lastWrittenControlByte(0xff),
    _cyclicalChannel(0),
    _bStreaming(false)
{
    Wire.begin();
}

void PCF8591::setAinToChannelMapping(AinToChannelMapping mapping)
{
    _ainToChannelMapping = mapping;
    _controlByte = replaceBits(_controlByte, ainToChannelMappingMask, _ainToChannelMapping);
}

uint8_t PCF8591::readChannel(uint8_t channel, bool bStreaming)
{
    // Geef een foutmelding als de channel niet geldig is.
    switch (_ainToChannelMapping)
    {
        case AIN_N_TO_CHANNEL_N:
            SerialAssert::assert(channel < 4, "channel > 3 ?  channel = ", int(channel));
            break;
        case AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2:
            SerialAssert::assert(channel < 4, "channel > 2 ?  channel = ", int(channel));
            break;
        case AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2:
            SerialAssert::assert(channel < 4, "channel > 2 ?  channel = ", int(channel));
            break;
        case AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1:
            SerialAssert::assert(channel < 4, "channel > 1 ?  channel = ", int(channel));
            break;
        default:
            SerialAssert::assert(channel < 4, "invalid mapping");
    }  

    _channel = channel;
    _controlByte = replaceBits(_controlByte, channelSelectionMask, channel);
    _bStreaming = bStreaming;

    uint8_t returnValue = 0;
    if((_controlByte != _lastWrittenControlByte) || !_bStreaming)
    {
        // Start met lezen van ADC value.
        writeControlByte(/*bStopBit*/ true);
        _bStreaming = bStreaming; // zet de streaming flag weer correct.

        // Lees 2 bytes van de PCF8591 in de cache.
        // De eerste byte is de "vorige waarde" van de ADC.
        returnValue = readSecondByte(/*stopbit=*/ !_bStreaming);
    }
    else
    {
        // Continueer met lezen uit de stream.
        returnValue = readByte(/*stopbit=*/ !_bStreaming);
    }
    return returnValue;
}

uint8_t PCF8591::readCyclical(uint8_t& channelThatWasRead)
{
    uint8_t returnValue = 0;

    _controlByte = _controlByte | autoincrementMask;
    if((_controlByte != _lastWrittenControlByte)||(!_bStreaming))
    {
        writeControlByte(/*bStopBit*/ true);
        _bStreaming = true; // zet de streaming flag weer correct.

        returnValue = readSecondByte(/*bStopBit=*/false);
        _cyclicalChannel = 0; // De eerste na de vorig is altijd channel 0 (zie datasheet pg..)
    }
    else
    {
        // We zijn nog steeds herhaaldelijk aan het lezen,
        // zonder het control byte opnieuw te hebben geschreven. 
        returnValue = readByte(/*stopbit=*/ false);
        _cyclicalChannel++;

        switch(_ainToChannelMapping)
        {
            case AIN_N_TO_CHANNEL_N:
                if(_cyclicalChannel > 3)
                {
                    _cyclicalChannel = 0;
                }
                break;
            case AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2:
                if(_cyclicalChannel > 2)
                {
                    _cyclicalChannel = 0;
                }
                break;
            case AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2:
                if(_cyclicalChannel > 2)
                {
                    _cyclicalChannel = 0;
                }
                break;
            case AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1:
                if(_cyclicalChannel > 1)
                {
                    _cyclicalChannel = 0;
                }
                break;
            default:
                SerialAssert::assert(false,"Invalid Mapping"); // Invalid mapping
        }
    }

    // Cyclisch lezen gaat altijd streaming verder.
    // anders levert het geen snelheidswinst en heeft het geen zin.
    _bStreaming = true; 
                        
    channelThatWasRead = _cyclicalChannel;
    return returnValue;
}

void PCF8591::endStreaming()
{
    if(_bStreaming)
    {
        Wire.endTransmission();
        _bStreaming = false;
    }
}

void PCF8591::disableAnalogOutput()
{
    _controlByte = _controlByte & ~analogOutputEnableMask;
    writeControlByte(/*bStopBit*/ true);
}

void PCF8591::writeDAC(uint8_t value, bool bStreaming)
{
    _bStreaming = bStreaming;
    _controlByte = _controlByte | analogOutputEnableMask;
    if((_controlByte != _lastWrittenControlByte) || (!bStreaming))
    {
        // Start met schrijven van DAC value.
        enableAnalogOutput();
        writeControlByte(/*bStopBit8*/ false);
        Wire.write(value);
        if(!bStreaming)
        {
            Wire.endTransmission();
        }
    }
    else
    {
        // Continueer met stream van DAC values.
        Wire.write(value);
    }
}

} // end namespace hogeschool