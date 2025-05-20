/**
 * @file IDAC.h
 * @brief Interface voor Digital-to-Analog Converters (DACs)
 * @author Marius Versteegen
 * @date 2025-05-20
 */

#pragma once
#include <Arduino.h>
#include <IStreaming.h>

namespace hogeschool
{
    /**
     * @class IDAC
     * @brief Abstracte interface voor het aansturen van DAC-kanalen.
     */
    class IDAC : public IStreaming
    {
        public:
            /**
             * @brief Schrijf een waarde naar een specifiek DAC-kanaal.
             * @param dacIndex Index van het DAC-kanaal (beginnend bij 0).
             * @param value 16-bit waarde die naar de DAC wordt gestuurd.
             * @param bStreaming 
             * Indien streaming, wordt na het schrijven van de value de
             * I2C transactie niet afgesloten, zodat eropvolgende writeDAC 
             * calls maximaal efficient kunnen zijn.
             * Je moet dan wel als je eenmaal klaar bent met de writeDAC
             * commando's, zelf endStreaming() aanroepen.
             */
            virtual void writeDAC (uint8_t dacIndex, uint16_t value, bool bStreaming) = 0;

            /**
             * @brief Schrijf cyclisch waarden naar alle DAC-kanalen.
             * Dus de eerste call naar het DAC kanaal met index 0.
             * De tweede naar het volgende DAC kanaal, etc.
             * Na schrijven naar het DAC kanaal met de hoogste index 
             * is het DAC kanaal met index 0 weer aan de beurt.
             * @note
             * Dit commando werkt streaming. Roep dus nadat je klaar bent
             * met writeDACsCyclical calls de functie endStreaming() aan.
             * @param value 16-bit waarde die naar de DAC wordt gestuurd.
             */
            virtual void writeDACsCyclical (uint16_t& value) = 0;

            /**
             * @brief Zet de analoge output van de DAC in tri-state mode,
             * bijvoorbeeld om stroom te besparen.
             */
            virtual void disableAnalogOutput() = 0;

            /**
             * @brief Retourneer het aantal DACs op de chip.
             */
            virtual uint8_t getNofDACs() = 0;

            /**
             * @brief Beeindig de huidige stream door de lopende I2C transactie
             * af te sluiten. De I2C bus is dan weer vrij voor ander gebruik.
             */
            virtual void endStreaming() = 0;
    };
}