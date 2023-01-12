#include "reifen.h"

void reifen(int seite, int mode){
  if (seite == 0){
    if (mode==STOP){
      digitalWrite(L1, LOW);
      digitalWrite(R1, LOW);
    }
    if(mode==VOR){
      digitalWrite(L1, HIGH);
      digitalWrite(R1, HIGH);
    }
  }
}