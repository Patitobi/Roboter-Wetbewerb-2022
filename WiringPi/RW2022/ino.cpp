#include <Arduino.h>
#include <IRremote.hpp>
//farbsensor
#define SENSOR_S0 5
#define SENSOR_S1 4
#define SENSOR_S2 7
#define SENSOR_S3 6
#define SENSOR_OUT 8

const int redmin = 22;
const int redmax= 381;
const int grumin = 22;
const int grumax = 406;
const int blumin = 15;
const int blumax = 254;

//USS
const int sensPins[4][2] = {{2,3}, {0, 0}, {0, 0}, {0, 0}};
int entfernung[4] = {0,0,0,0};

void setup() {
  //farbsensor
  digitalWrite(SENSOR_S0, HIGH);
  digitalWrite(SENSOR_S1, LOW);

  pinMode(SENSOR_S0, OUTPUT);
  pinMode(SENSOR_S1, OUTPUT);
  pinMode(SENSOR_S2, OUTPUT);
  pinMode(SENSOR_S3, OUTPUT);
  pinMode(SENSOR_OUT, INPUT);
  //USS
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
void getRed(){
  int frequency;
  digitalWrite(SENSOR_S2, LOW);
  digitalWrite(SENSOR_S3, LOW);
  frequency = pulseIn(SENSOR_OUT, LOW);
  map(frequency, redmin, redmax, 255, 0);
}
void getGruen(){
  int frequency;
  digitalWrite(SENSOR_S2, HIGH);
  digitalWrite(SENSOR_S3, HIGH);
  frequency = pulseIn(SENSOR_OUT, LOW);
  map(frequency, grumin, grumax, 255, 0);
}
void getBlue(){
  int frequency;
  digitalWrite(SENSOR_S2, LOW);
  digitalWrite(SENSOR_S3, HIGH);
  frequency = pulseIn(SENSOR_OUT, LOW);
  map(frequency, blumin, blumax, 255, 0);
}
void updatecolcor(){
  getRed();
  getGruen();
  getBlue();
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
