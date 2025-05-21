// Author: Marius Versteegen
// Copyright Marius Versteegen 2025.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// @file
/// \brief
/// Contains the declaration for the GenericPCF8591 class
///

#pragma once
#include <Arduino.h>
#include <IADC.h>
#include <IADCdiff.h>
#include <IDAC.h>
#include "pcf8591.h"

/// @brief 
/// Een "unieke" namespace voor je codebase voorkomt eventuele
/// name-clashes met third party code.
namespace hogeschool 
{
    class GenericPCF8591 : public IADC, public IADCdiff, public IDAC
    {
    private:
        PCF8591 _pcf8591;
        const uint8_t nofADCs = 4;             // nof ADCs that PCF8591 has.
        const uint8_t nofADCdiffs = nofADCs/2; // nof ADC pairs that PCF8591 has.
        const uint8_t nofDACs = 1;             // nof DACs that PCF8591 has.
        const uint16_t genericBits = 16;
        const uint16_t PCF8591Bits = 8;  // bits per value on PCF8491.
        const uint8_t bitShift = genericBits-PCF8591Bits; // voor resolutie normalisatie

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
        GenericPCF8591(uint8_t bitsA2A1A0);

    /// @note De overige interfaces zijn gelijk aan die van 
    /// de abtracte base classes IADC, IADCdiff, IDAC en IStreaming. 
    /// De documentatie ervan is daar te vinden.

    /// IADC
    uint16_t readADC (uint8_t adcIndex, bool bStreaming) override;
    int16_t readADCsCyclical (uint8_t& adcThatWasRead) override;
    uint8_t getNofADCs() override;

    /// IADCdiff
    int16_t readADCdiff (uint8_t adcDiffIndex, bool bStreaming) override;
    int16_t readADCdiffsCyclical (uint8_t& adcDiffThatWasRead) override;
    uint8_t getNofADCdiffs() override;

    /// IDAC
    void writeDAC (uint8_t dacIndex, uint16_t value, bool bStreaming) override;
    void writeDACsCyclical (uint16_t& value) override;
    void disableAnalogOutput() override;
    uint8_t getNofDACs() override;

    // IStreaming
    void endStreaming() override;
    };

} // end namespace hogeschool