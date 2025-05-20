/**
 * @file ADCsTester.h
 * @brief Het testen van ADC uitleesfunctionaliteit. 
 * @author Marius Versteegen
 * @date 2025-05-20
 */

#pragma once
#include "Arduino.h"
#include "IADC.h"

namespace hogeschool
{
    /**
     * @class ADCsTester
     * @brief Hulpklasse voor het testen van ADC uitleesfunctionaliteit 
     * van een klasse die de interface IADC implementeert.
     */
    class ADCsTester
    {
    private:
        IADC& _adcChip;
        uint8_t _nofAdcs;
        void testApartLezenVanADCs();
        void testCyclischLezenVanADCs();
        void testGestreamedLezenVanADCs();

    public:
        /**
         * @brief Constructor
         * @param adcChip Reference naar een adcChip object, welke 
         * de interface IADC implementeert.
         * @param nofAdcs Specificeert het aantal adc input pinnen welke
         * aanwezig zijn op de adc chip.
         */
        ADCsTester(IADC& adcChip, uint8_t nofAdcs);

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