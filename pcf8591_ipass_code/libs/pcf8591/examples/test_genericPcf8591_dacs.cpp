#include "Arduino.h"
#include <genericPcf8591.h>
#include <DACsTester.h>

// We willen geen magic constants, dus definiëren we alle constants hier.
const uint8_t   PCF8591_address = 0x48; // Mits A0, A1 en A2 met ground 
                                        // verbonden zijn.
namespace hs = hogeschool;  // shorthand
const uint8_t nA2A1A0_adresSelectorBits = 0b000;
hs::GenericPCF8591 genericPcf8591(nA2A1A0_adresSelectorBits);
const uint8_t nofDacs = 1;
const uint16_t sampleFreqKhz = 11;

// We kunnen ook direct genericPcf8591 aan dacsTester meegeven, maar om
// het even voor de studenten expliciet duidelijk te maken
// dat we alleen een generieke interface doorgeven:
hs::IDAC& dacChip = genericPcf8591;
hs::DACsTester dacsTester(dacChip, nofDacs, sampleFreqKhz);

void setup() 
{
  dacsTester.setup();
}

void loop() 
{
  dacsTester.loop();
}
