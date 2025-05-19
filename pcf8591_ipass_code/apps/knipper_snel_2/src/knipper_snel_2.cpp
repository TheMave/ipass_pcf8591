// Dit is een hello_world die op de arduino uno het ledje laat knipperen

#include <Arduino.h>
#include <knipper.h> // Zorg ervoor dat je de juiste pad naar de knipper.h bestand hebt

Knipper knipper(100); // Maak een instantie van de Knipper klasse

void setup() {
  // put your setup code here, to run once:
  knipper.setup(); // Roep de setup functie van de Knipper klasse aan
}

void loop() {
  knipper.update(); // Roep de update functie van de Knipper klasse aan
}