// Author: Marius Versteegen
// Copyright Marius Versteegen 2025.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// @file
/// \brief
/// Contains the declaration for the PCF8591 class
///

#pragma once
#include <Arduino.h>
#include <Wire.h>

/// @brief 
/// Een "unieke" namespace voor je codebase voorkomt eventuele
/// name-clashes met third party code.
namespace hogeschool 
{
    class PCF8591
    {
    public:
        // Bits 4 en 5 van het control byte bepalen de mapping van de analoge inputs
        // naar de kanalen.

    /** 
     * @brief AIN to channel mapping selection enum
     * @details
     * AIN_N_TO_CHANNEL_N 
     * -> Elke Analoge input wordt 1 op 1 gemapt naar één kanaal.
     * AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2 
     * -> De mate waarin AIN0, AIN1 en AIN2 groter zijn dan AIN3 
     *    wordt gemapt naar kanaal 0, 1 en 2.
     * AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2
     * -> Analoge inputs 0 en 1 worden 1 op 1 gemapt naar kanaal 0 en 1.
     * AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1
     * -> Het verschil tussen AIN0 en AIN1 en het verschil tussen AIN2 en AIN3
     *    wordt gemapt naar kanaal 0 en 1
    */
        enum AinToChannelMapping  : uint8_t
        {
            // Elke Analoge input wordt 1 op 1 gemapt naar een kanaal:
            AIN_N_TO_CHANNEL_N = 0b00<<4,

            // De mate waarin AIN0, AIN1 en AIN2 groter zijn dan AIN3 
            // wordt gemapt naar kanaal 0, 1 en 2:
            AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2 = 0b01<<4,

            // Analoge inputs 0 en 1 worden 1 op 1 gemapt naar kanaal 0 en 1:
            AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2 = 0b10<<4,

            // Het verschil tussen AIN0 en AIN1 en het verschil tussen AIN2 en AIN3
            // wordt gemapt naar kanaal 0 en 1:
            AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1 = 0b11<<4
        };

        private:
        // Definieer maskers voor de verschillende bits in het control byte,
        // zodat die makkelijk gemanipuleerd kunnen worden.
        const uint8_t channelSelectionMask      = 0b11;
        const uint8_t autoincrementMask         = 0b01<<2;
        const uint8_t ainToChannelMappingMask   = 0b11<<4;
        const uint8_t analogOutputEnableMask    = 0b01<<6;
        const uint8_t base_address = 0x48; // Het adres als A0, A1 en A2 met ground verbonden zijn.

        uint8_t _address;               // Het huidige I2C adres van de PCF8591 (inclusief A0, A1, A2)
        uint8_t _channel;               // Het huidige kanaal dat we willen lezen.
        uint8_t _controlByte;           // De huidige control byte van de PCF8591
        uint8_t _ainToChannelMapping;   // De huidige mapping van de analoge inputs naar de kanalen
        bool _bAutoIncrementFlag;       // De huidige status van de autoincrement flag

        uint8_t _lastWrittenControlByte; // De laatst geschreven control byte
        uint8_t _cyclicalChannel;        // Het laatst gelezen cyclische channel

        bool _bStreaming;                // In streaming mode wordt de I2C bus niet vrijgegeven
                                        // voor andere I2C devices middels een Stop Bit.
                                        // Dat kan dan expliciet gedaan worden met endStream().
        
                                        // Utility functie om bits in een byte te vervangen.
        uint8_t replaceBits(uint8_t byte, uint8_t mask, uint8_t value);
        void writeControlByte(bool bStreaming);
        uint8_t readByte(bool bStreaming);
        uint8_t readSecondByte(bool bStreaming);
        void enableAnalogOutput();

    public:
    /**
     * @brief Constructor
     * @param bitsA2A1A0
     * De pinnen A2, A1 en A0 van de chip kunnen gebruikt worden om 
     * een uniek I2C adres in te stellen.
     * @details 
     * Als bijvoorbeeld A2 en A1 zijn verbonden met supply en A0 met ground,
     * dient bitsA2A1A0=0xb110 worden meegegeven.
    */
        PCF8591(uint8_t bitsA2A1A0);

    /**
     * @brief AIN pin naar channel mapping
     * @param AinToChannelMapping
     * Via deze parameter kan de mapping worden gespecificeerd.
     */
        void    setAinToChannelMapping(AinToChannelMapping mapping);
        uint8_t readChannel(uint8_t channel, bool bStreaming = false);
        uint8_t readCyclical(uint8_t& channelThatWasRead);
        void    writeDAC(uint8_t value, bool bStreaming = false);
        void    disableAnalogOutput();
        void    endStreaming();
    }; // end class PCF8591
} // end namespace hogeschool