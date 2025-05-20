/**
 * @file DACsTester.h
 * @brief Het testen van DAC functionaliteit. 
 * @author Marius Versteegen
 * @date 2025-05-20
 */

#pragma once
#include "Arduino.h"
#include "IDAC.h"

namespace hogeschool
{
    /**
     * @class DACsTester
     * @brief Hulpklasse voor het testen van DAC functionaliteit 
     * van een klasse die de interface IDAC implementeert.
     */
    class DACsTester
    {
    private:
        IDAC& _dacChip;
        uint8_t _nofDacs;
        uint16_t _sampleFreqKhz;
        void testGestreamedZendenNaarDAC();

    public:
        /**
         * @brief Constructor
         * @param dacChip Reference naar een dacChip object, welke 
         * de interface IDAC implementeert.
         * @param nofDacs Specificeert het aantal dac output pinnen welke
         * aanwezig zijn op de dac chip.
         * @param sampleFreqKhz Specificeert de sample frequentie in kilohertz.
         */
        DACsTester(IDAC& dacChip, uint8_t nofDacs, uint16_t sampleFreqKhz);

        /**
         * @brief setup initialiseert de tests.
         */
        void setup();

        /**
         * @brief loop voert de tests uit.
         */
        void loop();
    };
}
