#pragma once
#include <Arduino.h>
namespace hogeschool
{
    class SerialAssert
    {
        public:
            inline static void assert(bool bCondition, const char* errorMessage)
            {
                #ifdef DEBUG_MODE
                    if(!bCondition)
                    {
                        Serial.println(errorMessage);
                    }
                #endif
            };

            inline static void assert(bool bCondition, const char* errorMessage, int nNumber)
            {
                #ifdef DEBUG_MODE
                    if(!bCondition)
                    {
                        Serial.print(errorMessage);
                        Serial.println(nNumber);
                    }
                #endif
            }

            inline static void assert(bool bCondition, const char* errorMessage, const char* text)
            {
                #ifdef DEBUG_MODE
                    if(!bCondition)
                    {
                        Serial.print(errorMessage);
                        Serial.println(text);
                    }
                #endif
            }


            inline static void assert(bool bCondition, const char* errorMessage, float flt)
            {
                #ifdef DEBUG_MODE
                    if(!bCondition)
                    {
                        Serial.print(errorMessage);
                        Serial.println(flt);
                    }
                #endif
            }
    };
}