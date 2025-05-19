# Gebruik met PlatformIO

- **VSCode** installeren
- **PlatformIO plugin** installeren
- Lokaal **platformio.ini.txt kopiëren naar platformio.ini** en de configuratie (de **COM port** van je Arduino) daar aanpassen.
- selecteer als volgt in de **terminal** wat je wilt bouwen en uploaden:
  **pio run -e knipper_snel -t upload**

### NB:

- Via **platformio.ini** worden je **.vscode bestanden** automatisch geconfigureerd.
  (soms moet je daarvoor even **F1->Developer Reload Window** activeren in VSCode)
- Als je wilt **includen uit subfolders van src**, dan kun je dat doen door die 
  subfolder in de include ervoor te zetten. Bijvoorbeeld: 
  #include <**testHelpers**/ADCsTester.h>