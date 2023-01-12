#include <Arduino.h>
#include <IRremote.hpp>
//#include <sync.cpp>

#define RECHTS  1
#define MITTE   0
#define LINKS  -1
#define VOR     1
#define STOP    0
#define ZURUECK -1

// steuerung / drei relays pro seite 1 und 2 für plus und minus 3 ist für das stopen
const int L1 = 9;
//const int L2 = 0;
//const int L3 = 0;

const int R1 = 10;
//const int R2 = 0;
//const int R3 = 0;

const int einGradeDrechen = 1; // anzahl an sekunden die wir brauchen um uns 1 grad zu drehen 

//farbsensor
const int SENSOR_S0 = 22;
const int SENSOR_S1 = 24;
const int SENSOR_S2 = 26;
const int SENSOR_S3 = 28;
const int SENSOR_OUT = 8;

const int redmin = 28;
const int redmax= 178;
const int grumin = 26;
const int grumax = 173;
const int blumin = 18;
const int blumax = 112;

// 0 = Rot; 1 = Schwarz; 2 = Weiß; 3 = Gelbs
int FarbeUnterMir = -1;

// gemessende werte werden hier gespeichert
int farbSensorVal[3][3];

int folgeFarbe = 1; // die variable die speicher welcher farbe gefolgt werden soll in rgb angabe
//!!! wichtig hier bei ist das die angaben nicht aus dem internet sind sonder die werte die wir auf der strecke messen !!!


//USS
const int sensPins[4][2] = {{45,40}, {46, 40}, {47, 40}, {48, 40}}; //1 Slot ist Output 2 Slot ist Input
int entfernung[4] = {0,0,0,0};
int durschnittaus = 10;

//Allgemeine Variablen
int NuminReihe;
bool synced;
String NextMoveBehindMe; //Hat das Vorhaben von dem Auto vor sich in sich
String NextMoveInfrontOfMe; //Hat das Vorhaben von dem Auto hinter sich in sich

//IR
String hexvalue;
const int RECV_PIN = 7;
const long TurnONCode = 0x1234; //Platzhalter für Fernbedienungs code um anzuschalten

IRrecv irrecv(RECV_PIN); //Empfänger Pin
decode_results results; //erstelle Object in welches dann die Daten nach jedem scan rein wandern

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
  // es werden die relays immer so gesetzt das wir in die angegebene richtung fahren
  // seite 0 bedeutet das beide seiten angesproche werden
  /*
  if (seite == 0||mode == STOP){
     digitalWrite(R3, LOW);
     digitalWrite(L3, LOW);

     digitalWrite(R2, LOW);
     digitalWrite(L2, LOW);

  } 
  else if(seite == 0||mode == VOR){
     digitalWrite(R3, HIGH);
     digitalWrite(R2, LOW);
     digitalWrite(R1, LOW);

     digitalWrite(L3, HIGH);
     digitalWrite(L2, LOW);
     digitalWrite(L1, LOW);
  }else{
     für links
    if (seite == -1){
      if (mode == STOP){
        digitalWrite(L3, LOW);
        digitalWrite(L2, LOW);
      } else if(mode == VOR){
        digitalWrite(L3, HIGH);
        digitalWrite(L2, LOW);
        digitalWrite(L1, LOW);
      } else if(mode == ZURUECK){
        digitalWrite(L3, LOW);
        digitalWrite(L2, HIGH);
        digitalWrite(L1, HIGH);
      }
    }
    // für rechts
    if (seite == 1){
      if (mode == STOP){
        digitalWrite(R3, LOW);
        digitalWrite(R2, LOW);
      } else if(mode == VOR){
        digitalWrite(R3, HIGH);
        digitalWrite(R2, LOW);
        digitalWrite(R1, LOW);
      } else if(mode == ZURUECK){
        digitalWrite(R3, LOW);
        digitalWrite(R2, HIGH);
        digitalWrite(R1, HIGH);
      }
    }
  }*/
}
void drehen(int richtung, int grade){
  if (richtung == RECHTS){
    reifen(LINKS, VOR);// setzt motoren so das wir uns an der stelle drehen in die angegebene richtung
    reifen(RECHTS, ZURUECK);
    delay(einGradeDrechen * grade);// delayt füe die zeit die wir brachen um uns für die bestimmte grade zahl zu drehen
  } else if (richtung == LINKS){
    reifen(LINKS, ZURUECK);// setzt motoren so das wir uns an der stelle drehen in die angegebene richtung
    reifen(RECHTS, VOR);
    delay(einGradeDrechen * grade); // delayt füe die zeit die wir brachen um uns für die bestimmte grade zahl zu drehen
  }
}

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
void getRed(int sensnum){
  int frequency;
  digitalWrite(SENSOR_S2, LOW);
  digitalWrite(SENSOR_S3, LOW);
  frequency = pulseIn(SENSOR_OUT, LOW);
  farbSensorVal[sensnum][0] = map(frequency, redmin, redmax, 255, 0);
  Serial.print("rot =  ");
  Serial.print(farbSensorVal[sensnum][0]);
  //Serial.print(frequency);
  Serial.print("      ");
}
void getGruen(int sensnum){
  int frequency;
  digitalWrite(SENSOR_S2, HIGH);
  digitalWrite(SENSOR_S3, HIGH);
  frequency = pulseIn(SENSOR_OUT, LOW);
  farbSensorVal[sensnum][1] = map(frequency, grumin, grumax, 255, 0);
  Serial.print("greun =  ");
  Serial.print(farbSensorVal[sensnum][1]);
  //Serial.print(frequency);
  Serial.print("      ");
}
void getBlue(int sensnum){
  int frequency;
  digitalWrite(SENSOR_S2, LOW);
  digitalWrite(SENSOR_S3, HIGH);
  frequency = pulseIn(SENSOR_OUT, LOW);
  farbSensorVal[sensnum][2] = map(frequency, blumin, blumax, 255, 0);
  Serial.print("blau = ");
  Serial.print(farbSensorVal[sensnum][2]);
  //Serial.print(frequency);
  Serial.println("      ");
}
void setFarbe(){
  for (int sensnum = 0; sensnum<1; sensnum++){
    if ((farbSensorVal[sensnum][0] <= 160 && farbSensorVal[sensnum][0]>= 100) && (farbSensorVal[sensnum][1]<=70 && farbSensorVal[sensnum][1 ]>= 10) && (farbSensorVal[sensnum][2] <= 70 && farbSensorVal[sensnum][2] >= 20)){
      FarbeUnterMir = 0;
    }
    else if ((farbSensorVal[sensnum][0] <= 21) && (farbSensorVal[sensnum][1]<=25) && (farbSensorVal[sensnum][2] <= 40)){
      FarbeUnterMir = 1;
    }
    else if ((farbSensorVal[sensnum][0]>= 225) && (farbSensorVal[sensnum][1] >= 225) && (farbSensorVal[sensnum][2] >= 225)){
      FarbeUnterMir = 2;
    }
    else if ((farbSensorVal[sensnum][0] <= 140 && farbSensorVal[sensnum][0]>= 114) && (farbSensorVal[sensnum][1]<=112 && farbSensorVal[sensnum][1]>= 98) && (farbSensorVal[sensnum][2] <= 74 && farbSensorVal[sensnum][2] >= 49)){
      FarbeUnterMir = 3;
    }
    else {
      FarbeUnterMir = -1;
    }
  }
  Serial.println(FarbeUnterMir);
}
void updatecolcor(){
  for (int i=0; i<1; i++){
    getRed(i);
    getGruen(i);
    getBlue(i);
  }
  setFarbe();
  switch (FarbeUnterMir)
  {
  case 0: // wenn rot
    Serial.println("rot");
    reifen(0, STOP);
    break;
  case 1: // wenn Schwarß
    Serial.println("Schwarz");
    reifen(0, VOR);
    break;
  case 2: // wenn Weiß
    Serial.println("Weiß");
    break;
  case 3: // wenn Gelb
    Serial.println("Gelb");
    break;
  default: // wenn nicht erkennbar
    Serial.println("NULL");
    break;
  }
}
void SendIR(long Code, int repeat, int dir){ //dir gibt an ob nach vorne oder nach hinten
  if(dir == 1){ //Send to front
    for(int i = 0; i != repeat; i++){
      IrSender.sendSony(Code, 20);
      delay(1000); //Muss min 5 millisek sonst erkennt der Empfänger das als ein einziges
    }
  }else if(dir == 0){ //Send to back
    for(int i = 0; i != repeat; i++){
      IrSender.sendSony(Code, 20);
      delay(1000); //Muss min 5 millisek sonst erkennt der Empfänger das als ein einziges
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
void GetIR(){
  //code der nicht unterbrochen werden darf
  if (irrecv.decode(&results)){ //Wenn irgendwas auf dem recvPin Gelesen wird dann 
      Serial.println(results.value, HEX); //Print in den Arduino Log (HEX)
      hexvalue = String(results.value); //Hier kurz eine variable machen um nicht in allen if statements die funktion durch zu führen (performance)
      if(hexvalue == 0x1101){ //Grünes zeichen von Ampel
        AmpelPing(0x1101);
      }else if(hexvalue == 0x1210){ //Ampel Anfahrt 
        //Geb Signal nach hinten weiter
        SendIR(0x1210, 2, 0);
        AmpelAnfahrt(); //Fängt ann an den nächsten mann anzufahren (7cm) und wartet dann
      }
      irrecv.resume(); //Reset + es wird wieder vom Pin auf Info gewartet.
  }
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
