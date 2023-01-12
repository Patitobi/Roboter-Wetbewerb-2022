#include <Arduino.h>

// Variablen
//  pins zum ansteuern von den motoren
const int L1 = 9;
const int R1 = 10;

#define VOR 1
#define STOP 0

// functionen
void reifen_pinSetup();
void reifen(int seite, int mode);