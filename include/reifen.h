#include <Arduino.h>

// pins zum ansteuern von den motoren
const int L1 = 9;
const int R1 = 10;

#define VOR 1
#define STOP 0

void reifen(int seite, int mode);