# Generic Converters
## Inleiding
Deze folder bevat declaraties voor abstracte intefaces van een aantal typen AD en DA converters. Door de functionaliteit van converter-chips via deze interfaces toegankelijk te maken, kun je ze later eenvoudig vervangen door andere chips, zonder dat je de code van je applicatie hoeft aan te passen.

## Voorbeeld
```cpp
/**********************************************************************/
// Voorbeeld van applicatie met PCF8591, welke IADC en IDAC gebruikt:
/**********************************************************************/
GenericPCF8591 genericPCF8591;
IDAC& dacChip(genericPCF8591);
IADC& adcChip(genericPCF8591);

//.. gebruik de dacChip en adcChip in je applicatie, bijvoorbeeld:
int16_t adcValue = adcChip.readADC(/*adcIndex*/ 0, /*streaming*/ false);
dacChip.writeDAC(/*dacIndex*/ 0, adcValue, /*bStreaming*/ false);

/**********************************************************************/
// Als de PCF8591 is uitverkocht, of je wilt om andere redenen andere
// chips gebruiken:
/**********************************************************************/
GenericLTC2604 genericLTC2604;
GenericADS1115 genericADS1115;
IDAC& dacChip(genericLTC2604);
IADC& adcChip(genericADS1115);

//.. Je applicatie kan dan hetzelfde blijven:
int16_t adcValue = adcChip.readADC(/*adcIndex*/ 0, /*streaming*/ false);
dacChip.writeDAC(/*dacIndex*/ 0, adcValue, /*bStreaming*/ false);
```
## Testen
Je ziet het ook gedemonstreerd in de klassen ADCsTester en DACsTester. Daar staan tests in voor de functionaliteiten, zonder dat ze afhankelijk zijn van een specifieke chip.
