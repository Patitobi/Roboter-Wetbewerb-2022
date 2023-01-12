#include <Arduino.h>
#include "USS.h"
#include "sync.h"
#include "sync.h"
#include "farbSens.h"
#include "reifen.h"

//Allgemeine Variablen
int NuminReihe;
bool synced;
String NextMoveBehindMe; //Hat das Vorhaben von dem Auto vor sich in sich
String NextMoveInfrontOfMe; //Hat das Vorhaben von dem Auto hinter sich in sich

void USScheck(){
  for (int i=0; i<1;i++){
    if (entfernung[i]<=300){
      Serial.println("stop");
      digitalWrite(30, LOW);
      digitalWrite(34, LOW);
      
      reifen(0, STOP);
    } else {
      digitalWrite(30, HIGH);
      digitalWrite(34, HIGH);
      reifen(0, VOR);
    }
  }
}
void Fahrlos(){
  reifen(0, VOR);
}
void AmpelPing(long Code){
  //Wenn dieses Auto nicht letzter in der Reihe dann reiche die Info nach hinten weiter.
  if(NuminReihe != 5){
    //Pass IR Signal to the Cars behind
    SendIR(Code, 3, 0); //Send 3x hinter dich (0 =  nach hinten)
    //Start driving forward
    Fahrlos();
  }
}
void AmpelAnfahrt(){//Wird aufgerufen um dem vordermann bis auf 5cm aufzufahren und dann zu warten bis Grünes Licht signal vom vordermann kommt.
  //Fahr so lange an bis was 7 cm vor deiner nase ist dann bleib stehen und warte einfach nur bis das "Ampel ist grün" Signal kommt.
  reifen(0, VOR);
  while(entfernung[0] > 70){
    updateSensors();
  } //Warte bis auto vorne 7 cm nah ist
  reifen(0, STOP);
}
void RedLineReached(){ //Muss von Farbsensor gecallt werden und kann auch nur von index 1 gecallt werden
  //Ping hinter dich das die zu dir bis auf eine bestimmte distanz auffahren sollen und dann auch stehen bleiben.
  //Dann Ping die Ampel an das die anfangen soll ihr Programm abzurfen -> (Ampel wartet 8 Sekunden und gibt dann grünes Signal via IR)
  SendIR(0x1210, 2, 0); //Stehen bleiben 2x nach hinten
  if(NuminReihe == 1) SendIR(0x1240, 2, 1); //If Abfrage nur zur Sicherheit. Eigentlich unnötig. //2. Vorderes Auto sendet Signal zur ampel damit Ampel anfängt zu agieren
  //Warte nun auf Ampel Signal und gebe wenn  Ampel Signal da das Signal an die hinteren weiter
  while(hexvalue != 0x1101){ //String() Fehlt kb gerade
    GetIR();
  }
  if(hexvalue == String(0x1101)){ //if zur sicherheit falls interrupt was durcheinander bringt
    //send nach hinten weiter damit jeder los fährt
    SendIR(0x1101, 3, 0);
  }
}
void WaitforStart(){
  while(!synced){
    //Warte Aufs Go von der Fernbedienung und wenn Signal 0x1211 kommt Fahre los
    GetIR();
    if(hexvalue == String(0xFF30CF)){
      synced = true;
    }
  }
}
void GetmyIndex(){
  while(!synced){
    GetIR();
    if(hexvalue == String(/*Fernbedienung Index 1 Code*/)){
      NuminReihe = 1;
      WaitforStart(); 
    }else if(hexvalue == String(/*Fernbedienung Index 2 Code*/)){
      NuminReihe = 2;
      WaitforStart(); 
    }else if(hexvalue == String(/*Fernbedienung Index 3 Code*/)){
      NuminReihe = 3;
      WaitforStart(); 
    }else if(hexvalue == String(/*Fernbedienung Index 4 Code*/)){
      NuminReihe = 4;
      WaitforStart(); 
    }
  }
}
void update(){
  updateSensors();
  //updatecolcor();
}
void machen(){
  //USScheck();
}
void setup() {
  hexvalue = "0";
  //farbsensor
  pinMode(SENSOR_S0, OUTPUT);
  pinMode(SENSOR_S1, OUTPUT);
  pinMode(SENSOR_S2, OUTPUT);
  pinMode(SENSOR_S3, OUTPUT);
  pinMode(SENSOR_OUT, INPUT);

  digitalWrite(SENSOR_S0, HIGH);
  digitalWrite(SENSOR_S1, LOW);

  // steuerung
  pinMode(L1, OUTPUT);
  //pinMode(L2, OUTPUT);
  //pinMode(L3, OUTPUT);

  pinMode(R1, OUTPUT);
  //pinMode(R2, OUTPUT);
  //pinMode(R3, OUTPUT);

  //USS
  for (int i=0; i<4; i++){
    pinMode(sensPins[i][1], OUTPUT); 
    pinMode(sensPins[i][0], INPUT);
  }
  pinMode(34, OUTPUT);  
  pinMode(30, OUTPUT);
  //IR
  //Recieve IR
  irrecv.begin(7, ENABLE_LED_FEEDBACK); //Fang an zu Lesen
  //Send IR
  pinMode(LED_BUILTIN, OUTPUT);
  IrSender.begin(3);
  //attachInterrupt(digitalPinToInterrupt(3), CodetoBeExecutedOnInterrupt, CHANGE); //wenn sich pin 3 ändert dann führe interruptcode aus
  Serial.begin(115200);
  //Start sync with other cars
  reifen(0, STOP);
  Serial.println("GO");
  GetmyIndex();
}
void loop(){
  //update();
  //machen();
}