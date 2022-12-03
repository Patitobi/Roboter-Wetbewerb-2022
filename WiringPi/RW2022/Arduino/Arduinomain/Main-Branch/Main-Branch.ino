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

//Allgemeine Variablen
int NuminReihe;
bool synced;
String NextMoveBehindMe; //Hat das Vorhaben von dem Auto vor sich in sich
String NextMoveInfrontOfMe; //Hat das Vorhaben von dem Auto hinter sich in sich

//IR
String hexvalue;
const int RECV_PIN = 7;
const long TurnONCode = 0x1234; //Platzhalter für Fernbedienungs code um anzuschalten
bool Car1;
bool Car2;
bool Car3;
bool Car4;

IRrecv irrecv(RECV_PIN); //Empfänger Pin
decode_results results; //erstelle Object in welches dann die Daten nach jedem scan rein wandern

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
  //IR
  //Recieve IR
  irrecv.enableIRIn(); //Fang an zu Lesen
  irrecv.blink13(true); //Default Pin auf dem Board welcher blinkt wenn Pin was ließt
  //Send IR
  pinMode(LED_BUILTIN, OUTPUT);
  IrSender.begin(3);
  attachInterrupt(digitalPinToInterrupt(3), CodetoBeExecutedOnInterrupt, CHANGE); //wenn sich pin 3 ändert dann führe interruptcode aus
  Serial.begin(9600);
  //Start sync with other cars
  Startsync();
}

void Startsync(){
  while(!synced){ //Wartet auf Fernbedienung auf zuweisung von nummer
    GetIR();
    if(hexvalue == 0x0000){ //Platzhalter für fernbiediegungs code
      NuminReihe = 1;
      Car1 = true;
      StartGroup(NuminReihe);
    }else if(hexvalue == 0x0000){
      NuminReihe = 2;
      Car2 = true;
      StartGroup(NuminReihe);
    }
    else if(hexvalue == 0x0000){
      NuminReihe = 3;
      Car3 = true;
      StartGroup(NuminReihe);
    }
    else if(hexvalue == 0x0000){
      NuminReihe = 4;
      Car4 = true;
      StartGroup(NuminReihe);
    }
  }
}
void StartGroup(int index){
  while(!synced){
    //Send Group request
    if(index == 1){
      SendIR(0x1201, 3, 0); //hinter sich und
      SendIR(0x1201, 3, 1); //vor sich senden
    }else if(index == 2){
      SendIR(0x1202, 3, 0);
      SendIR(0x1202, 3, 1);
    }else if(index == 3){
      SendIR(0x1203, 3, 0);
      SendIR(0x1203, 3, 1);
    }else if(index == 4){
      SendIR(0x1204, 3, 0);
      SendIR(0x1204, 3, 1);
    }
    //Read Group Requests
    GetIR();
    //Wass wir hier machen ist wir warten biss alle Signale von allen Autos einmal bei einem Auto angekommen sind
    //Wir Arbeiten quasi einfach eine Check Liste ab mit allen Codes.
    if(hexvalue == 0x1201 || hexvalue == 0x1202 || hexvalue == 0x1203 || hexvalue == 0x1204){
      //Send das gelesene Signal nach vorne und hinten Weiter
      SendIR(irrecv.decode(&results), 1, 1); //1x nach vorne
      SendIR(irrecv.decode(&results), 1, 0); //1x nach hinten
      //Check das bekommene Signal ab
      if(hexvalue == 0x1201) Car1 = true;
      else if(0x1202) Car2 = true;
      else if(0x1203) Car3 = true;
      else if(0x1204) Car4 = true;
    }
    if(Car1 && Car2 && Car3 && Car4) synced = true;
  }
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
void GetIR(){
  //code der nicht unterbrochen werden darf
  if (irrecv.decode(&results)){ //Wenn irgendwas auf dem recvPin Gelesen wird dann 
      Serial.println(results.value, HEX); //Print in den Arduino Log (HEX)
      hexvalue = String(results.value, HEX); //Hier kurz eine variable machen um nicht in allen if statements die funktion durch zu führen (performance)
      if(hexvalue.indexOf("0x11") > 0){ //Schaut ob 0x11 im String irgendwo vorhanden ist. Denn 0x11 sind die ersten 2 ziffern des ampel hex codes
        AmpelPing(results.value);
      }
      irrecv.resume(); //Reset + es wird wieder vom Pin auf Info gewartet.
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
void CodetoBeExecutedOnInterrupt(){
  GetIR();
}
void AmpelPing(long Code){
  //Wenn dieses Auto erster in der Reihe dann reiche die Info nach hinten weiter.
  if(NuminReihe != 5){
    //Pass IR Signal to the Cars behind
    SendIR(Code, 3, 0); //Send 3x hinter dich (0 =  nach hinten)
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