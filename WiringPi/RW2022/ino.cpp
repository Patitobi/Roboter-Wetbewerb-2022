#include <Arduino.h>
#include <IRremote.hpp>

const int sensPins[4][2] = {{2,3}, {0, 0}, {0, 0}, {0, 0}};
int entfernung[4] = {0,0,0,0};

void setup() {
  pinMode(sensPins[0][1], OUTPUT); 
  pinMode(sensPins[0][0], INPUT);
  pinMode(34, OUTPUT);
  pinMode(30, OUTPUT);
  //pinMode(sensPins[1][1], OUTPUT); 
  //pinMode(sensPins[1][0], INPUT); 
  Serial.begin(9600);
}
void Rreley(bool modus){//30
  if (modus){
    digitalWrite(30, HIGH);
  } else {
    digitalWrite(30, LOW);
  }
}
void Lreley(int modus){//34
  if (modus){
    digitalWrite(34, HIGH);
  } else {
    digitalWrite(34, LOW);
  }
}
// für einzelne 
void updatesensor(int sensnum){
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
  entfernung[sensnum] =  duration * 0.34 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(entfernung[sensnum]);
  Serial.println(" mm");
  Serial.print(sensnum);
}
// zum updaten von allen 
void updateSensors(){
  for(int i=0;i<2; i++){
    updatesensor(i);
  }
}
void USScheck(){
  for (int i=0; i<1;i++){
    if (entfernung[i]<=200){
      Serial.println("stop");
      Rreley(false);
      Lreley(false);
    } else {
      Rreley(true);
      Lreley(true);
    }
  }
}
void update(){
  updatesensor(0);
}
void machen(){
  USScheck();
}
void loop() {
  update();
  
  machen();
}
