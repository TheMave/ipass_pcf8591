#pragma once
#include <Arduino.h>

class Knipper {
private:
  unsigned long vorigeTijd_ms;   // Tijdstip van laatste toggle
  unsigned long interval_ms;     // Interval tussen toggles (in ms)
  bool ledAan;                   // Huidige LED-status

public:
  Knipper(unsigned long interval_ms = 1000); 
  void setup();
  void update();
};
