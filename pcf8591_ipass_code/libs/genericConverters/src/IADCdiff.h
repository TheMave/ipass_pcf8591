/**
 * @file IADCdiff.h
 * @brief Interface voor differential input Analog-to-Digital Converters (ADCs)
 * @author Marius Versteegen
 * @date 2025-05-20
 */

#pragma once
#include <Arduino.h>
#include <IStreaming.h>

namespace hogeschool
{
    /**
     * @class IADC
     * @brief Abstracte interface voor het uitlezen van differentiele ADC-inputs.
     * @note Een differentiele ADC-input bestaat uit een paar van 2 ADC inputs.
     * De eerste tussen pinnen AIN0 en AIN1, de volgende tussen AIN2 en AIN3, etc.
     * De uitgelezen waarde is het verschil tussen de eerste en de tweede 
     * input van het paar. Dat verschil kan negatief zijn.
     */
    class IADCdiff : public IStreaming
    {
    public:
        /**
         * @brief Lees de verschilwaarde uit een specifieke ADC-input-paar.
         * @param dacIndex Index van het ADC-input-paar (beginnend bij 0).
         * @return 16-bit verschilwaarde die wordt gelezen.
         * @param bStreaming 
         * Indien streaming, wordt na het lezen van de value de
         * I2C transactie niet afgesloten, zodat eropvolgende readADCdiff 
         * calls maximaal efficient kunnen zijn.
         * Je moet dan wel als je eenmaal klaar bent met de readADCdiff
         * commando's, zelf endStreaming() aanroepen.
         */
        virtual uint16_t readADCdiff (uint8_t adcDiffIndex, bool bStreaming) = 0;

        /**
         * @brief Lees cyclisch de verschilwaarden uit alle ADC-input-paartjes.
         * Dus de eerste call leest uit het ADC-input-paar met index 0.
         * Dat paar zit op pinnen AIN0 en AIN1.
         * De tweede leest uit het volgende paar, etc.
         * Na lezen uit het ADC-input-paar met de hoogste index 
         * is het ADC-input-paar met index 0 weer aan de beurt.
         * @return 16-bit verschilwaarde die wordt gelezen.
         * @note
         * Dit commando werkt streaming. Roep dus nadat je klaar bent
         * met readADCdiffsCyclical calls de functie endStreaming() aan.
         */
        virtual int16_t readADCdiffsCyclical (uint8_t& adcDiffThatWasRead) = 0;

        /**
         * @brief Retourneer het aantal ADC-input-paartjes op de chip.
         * Dus het aantal Analoge inputs gedeeld door 2.
         */
        virtual uint8_t getNofADCdiffs() = 0;

        /**
         * @brief Beeindig de huidige stream door de lopende I2C transactie
         * af te sluiten. De I2C bus is dan weer vrij voor ander gebruik.
         */
        virtual void endStreaming() = 0;
    };
}