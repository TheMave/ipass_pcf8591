#include <Arduino.h>
#include "knipper.h"

Knipper::Knipper(unsigned long interval_ms) :
    vorigeTijd_ms(0), interval_ms(interval_ms), ledAan(false)
{
}

void Knipper::setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void Knipper::update() 
{
    unsigned long huidigeTijd = millis();
    if (huidigeTijd - vorigeTijd_ms >= interval_ms) {
        vorigeTijd_ms = huidigeTijd;

        // Toggle LED-status
        ledAan = !ledAan;
        digitalWrite(LED_BUILTIN, ledAan ? HIGH : LOW);
    }
}
