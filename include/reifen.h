#include <Arduino.h>

// Variablen
//  pins zum ansteuern von den motoren

#define VOR 1
#define STOP 0

// functionen
void reifen_pinSetup();
void reifen(int seite, int mode);