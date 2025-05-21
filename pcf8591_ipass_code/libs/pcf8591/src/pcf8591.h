/**
 * @file pcf8591.h
 * @brief driver voor de pcf8591 AD/DA converter chip.
 * @author Marius Versteegen
 * @date 2025-05-20
 */

/// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <Arduino.h>
#include <Wire.h>

/// @brief 
/// Een "unieke" namespace voor je codebase voorkomt eventuele
/// name-clashes met third party code.
namespace hogeschool 
{
    /**
     * @class PCF8591
     * @brief Driver die de functionaliteit van de pcf8591 chip ontsluit.
     */
    class PCF8591
    {
    public:
        // Bits 4 en 5 van het control byte bepalen de mapping van de analoge inputs
        // naar de kanalen.
    /**
     * @enum AinToChannelMapping
     * @brief Keuze voor het mappen van AIN-signalen naar outputkanalen.
     *
     * Dit enum definieert verschillende manieren om de vier analoge inputs
     * (AIN0–AIN3) om te zetten naar maximaal vier kanalen.
     *
     * @details
     * Mogelijke waarden:
     * - **AIN_N_TO_CHANNEL_N**  
     *   Elke AIN<sub>i</sub> → kanaal <i>.
     * - **AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2**  
     *   (AIN0, AIN1, AIN2) − AIN3 → kanalen 0, 1, 2.
     * - **AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2**  
     *   AIN0 → kanaal 0, AIN1 → kanaal 1, (AIN2 − AIN3) → kanaal 2.
     * - **AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1**  
     *   (AIN1 − AIN0) → kanaal 0, (AIN2 − AIN3) → kanaal 1.
     *
     * @var AinToChannelMapping::AIN_N_TO_CHANNEL_N
     * 1:1 mapping van elke AIN naar hetzelfde kanaal (bits 5–4 = `00`).
     *
     * @var AinToChannelMapping::AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2
     * Mappt de verschillen (AIN0–AIN3, AIN1–AIN3, AIN2–AIN3) naar kanalen 0–2 (bits 5–4 = `01`).
     *
     * @var AinToChannelMapping::AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2
     * 1:1 mapping voor AIN0/1 naar kanaal 0/1, en (AIN2-AIN3) naar kanaal 2 (bits 5–4 = `10`).
     *
     * @var AinToChannelMapping::AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1
     * (AIN1-AIN0) → kanaal 0 en (AIN2-AIN3) → kanaal 1 (bits 5–4 = `11`).
     */
    enum AinToChannelMapping : uint8_t
    {
        AIN_N_TO_CHANNEL_N                             = 0b00 << 4,
        AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2           = 0b01 << 4,
        AIN0_1_TO_CHANNEL_0_1__AIN2_MINUS_AIN3_TO_CHANNEL_2 = 0b10 << 4,
        AIN1_MINUS_AIN0_TO_CHANNEL_0__AIN2_MINUS_AIN3__TO_CHANNEL_1 = 0b11 << 4
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
         * Via deze enum parameter kan de mapping van analoge input (pin)s naar
         * kanalen worden gespecificeerd (zie de enum documentatie).
         */
        void    setAinToChannelMapping(AinToChannelMapping mapping);

        /**
         * @brief Lees een waarde uit een kanaal.
         * @param channel de index het kanaal (beginnend bij 0).
         * @return 16-bit waarde die uit de ADC wordt gelezen.
         * @param bStreaming 
         * Indien streaming, wordt na het lezen van de value de
         * I2C transactie niet afgesloten, zodat eropvolgende readChannel 
         * calls maximaal efficient kunnen zijn.
         * Je moet dan wel als je eenmaal klaar bent met de readChannel
         * commando's, zelf endStreaming() aanroepen.
         */
        uint8_t readChannel(uint8_t channel, bool bStreaming = false);

        /**
         * @brief Lees cyclisch waarden uit alle kanalen.
         * Dus de eerste call leest uit het kanaal met index 0.
         * De tweede leest uit de volgende, etc.
         * Na lezen uit het kanaal met de hoogste index 
         * is de het kanaal met index 0 weer aan de beurt.
         * @return 16-bit waarde die uit het kanaal wordt gelezen.
         * @note
         * Dit commando werkt streaming. Roep dus nadat je klaar bent
         * met readCyclical calls de functie endStreaming() aan.
         * @note
         * Als het betreffende kanaal voortkomt uit een differentiele
         * mapping van analoge inputs, zoals AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2,
         * dan moet je de uint_8 eerst nog een reinterpret_cast geven naar int8_t.
         */
        uint8_t readCyclical(uint8_t& channelThatWasRead);

        /**
         * @brief Schrijf een waarde naar de DAC.
         * @param value 16-bit waarde die naar de DAC wordt gestuurd.
         * @param bStreaming 
         * Indien streaming, wordt na het schrijven van de value de
         * I2C transactie niet afgesloten, zodat eropvolgende writeDAC 
         * calls maximaal efficient kunnen zijn.
         * Je moet dan wel als je eenmaal klaar bent met de writeDAC
         * commando's, zelf endStreaming() aanroepen.
         * @note
         * Als het betreffende kanaal voortkomt uit een differentiele
         * mapping van analoge inputs, zoals AIN0_1_2_MINUS_AIN3_TO_CHANNEL_0_1_2,
         * dan moet je de uint_8 eerst nog een reinterpret_cast geven naar int8_t.
         */
        void    writeDAC(uint8_t value, bool bStreaming = false);

        /**
         * @brief Zet de analoge output van de DAC in tri-state mode,
         * bijvoorbeeld om stroom te besparen.
         */
        void    disableAnalogOutput();

        /**
         * @brief endStreaming beeindigt de lopende transactie op de 
         * communicatiebus (bijvoorbeeld I2C), zodat de communicatiebus
         * weer wordt vrijgegeven voor nieuwe transacties.
         */
        void    endStreaming();
    }; // end class PCF8591
} // end namespace hogeschool