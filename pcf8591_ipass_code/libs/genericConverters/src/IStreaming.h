/**
 * @file IStreaming.h
 * @brief Interface voor het reguleren van streaming.
 * @author Marius Versteegen
 * @date 2025-05-20
 */

#pragma once
namespace hogeschool
{
    class IStreaming
    {
    public:
        /**
         * @brief endStreaming beeindigt de lopende transactie op de 
         * communicatiebus (bijvoorbeeld I2C), zodat de communicatiebus
         * weer wordt vrijgegeven voor nieuwe transacties.
         */
        virtual void endStreaming() = 0;
    };
}