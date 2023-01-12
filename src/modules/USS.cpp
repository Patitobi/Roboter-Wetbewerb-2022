#include "USS.h"

// für einzelne 
int updatesensor(int sensnum){
  long duration;
  // Clears the trigPin condition
  digitalWrite(sensPins[sensnum][1], LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(sensPins[sensnum][1], HIGH);
  delayMicroseconds(10);
  digitalWrite(sensPins[sensnum][1], LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(sensPins[sensnum][0], HIGH);
  // Calculating the distance
  return duration * 0.34 / 2; // Speed of sound wave divided by 2 (go and back)
}
// zum updaten von allen 
void updateSensors(){
  int temp;
  for(int i=0;i<1; i++){
    entfernung[i] = 0;
    temp = 0;
    for (int x=0; x<durschnittaus; x++){
      temp = updatesensor(i);
      if (temp > 1200||temp<0){
        temp = updatesensor(i);
      }
      entfernung[i]+=temp;
    }
    entfernung[i] /= durschnittaus;
    Serial.print("entfernung");
    Serial.println(entfernung[i]);
  }
}