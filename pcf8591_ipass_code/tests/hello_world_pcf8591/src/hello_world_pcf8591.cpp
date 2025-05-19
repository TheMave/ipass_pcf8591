#include "Arduino.h"
#include <Wire.h>

// We willen geen magic constants, dus definiëren we alle constants hier.
const uint8_t   PCF8591_address = 0x48;
const int       serialBaudRate  = 9600;
const int       maxNofChannels  = 4; // A0 is de enige analoge pin op de Arduino Uno

void setup() 
{
  Serial.begin(serialBaudRate);
  Wire.begin();
}

void loop() 
{
  for (int i = 0; i < maxNofChannels; i++) {
    // Kies single-ended input mode en een channel
    // door de juiste bits in het control register te schrijven.
    Wire.beginTransmission(PCF8591_address);
    // De laagste 2 bits selecteren de channel.
    // De overige bits staan op 0. Dat betekent:
    // - De DAC output is uitgeschakeld.
    // - Analoge inputs 0..3 worden 1 op 1 op channel 0..3 gemapt.
    // - De autoincrement flag is uitgeschakeld.
    Wire.write(0x00 | i);
    Wire.endTransmission();

    // Lees 2 bytes: de eerste is de "vorige waarde" van de ADC.
    // De tweede is de waarde die we willen hebben.

    // Lees 2 bytes van de PCF8591 in de cache.
    Wire.requestFrom(PCF8591_address, 2);

    // Lees de eerste byte uit de cache.
    Wire.read();
    // Lees de tweede byte uit de cache.
    int value = Wire.read();

    // Print de waarde naar de serial monitor
    Serial.print("AIN");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(value);
    if((i%4)==3) Serial.println("--------");
  }

  // Wacht even, anders komt er meer data naar de serial monitor dan die
  // kan verwerken. (bovendien vliegt het dan te snel voorbij om te kunnen lezen)
  delay(2000);
}
