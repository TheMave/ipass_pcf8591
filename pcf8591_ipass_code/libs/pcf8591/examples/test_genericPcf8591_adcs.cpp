#include "Arduino.h"
#include <genericPcf8591.h>
#include <testHelpers/ADCsTester.h>

// We willen geen magic constants, dus definiëren we alle constants hier.
const uint8_t   PCF8591_address = 0x48; // Mits A0, A1 en A2 met ground 
                                        // verbonden zijn.
namespace hs = hogeschool;  // shorthand
const uint8_t nA2A1A0_adresSelectorBits = 0b000;
hs::GenericPCF8591 genericPcf8591(nA2A1A0_adresSelectorBits);

const uint8_t nofAdcs = 4;

// We kunnen ook direct genericPcf8591 aan adcsTester meegeven, maar om
// het even voor de studenten expliciet duidelijk te maken
// dat we alleen een generieke interface doorgeven:
hs::IADC& adcChip = genericPcf8591;
hs::ADCsTester adcsTester(adcChip, nofAdcs);

void setup() 
{
  adcsTester.setup();
}

void loop() 
{
  adcsTester.loop();
}
