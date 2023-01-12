#include <Arduino.h>

//USS
const int sensPins[4][2] = {{45,40}, {46, 40}, {47, 40}, {48, 40}}; //1 Slot ist Output 2 Slot ist Input
int entfernung[4] = {0,0,0,0};
int durschnittaus = 10;