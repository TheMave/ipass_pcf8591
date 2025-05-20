/**
 * @file IADC.h
 * @brief Interface voor Analog-to-Digital Converters (ADCs)
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
     * @brief Abstracte interface voor het uitlezen van ADC-inputs.
     */
    class IADC : public IStreaming
    {
        public:
            /**
             * @brief Lees een waarde uit een specifieke ADC-input.
             * @param dacIndex Index van de ADC-input (beginnend bij 0).
             * @return 16-bit waarde die uit de ADC wordt gelezen.
             * @param bStreaming 
             * Indien streaming, wordt na het lezen van de value de
             * I2C transactie niet afgesloten, zodat eropvolgende readADC 
             * calls maximaal efficient kunnen zijn.
             * Je moet dan wel als je eenmaal klaar bent met de readADC
             * commando's, zelf endStreaming() aanroepen.
             */
            virtual uint16_t readADC (uint8_t adcIndex, bool bStreaming) = 0;

            /**
             * @brief Lees cyclisch waarden uit alle ADC-inputs.
             * Dus de eerste call leest uit de ADC-input met index 0.
             * De tweede leest uit de volgende, etc.
             * Na lezen uit de ADC-input met de hoogste index 
             * is de ADC input met index 0 weer aan de beurt.
             * @return 16-bit waarde die uit de ADC wordt gelezen.
             * @note
             * Dit commando werkt streaming. Roep dus nadat je klaar bent
             * met readADCsCyclical calls de functie endStreaming() aan.
             */
            virtual int16_t readADCsCyclical (uint8_t& adcThatWasRead) = 0;

            /**
             * @brief Retourneer het aantal ADC-inputs op de chip.
             */
            virtual uint8_t getNofADCs() = 0;

            /**
             * @brief Beeindig de huidige stream door de lopende I2C transactie
             * af te sluiten. De I2C bus is dan weer vrij voor ander gebruik.
             */
            virtual void endStreaming() = 0;
    };
}