#include <Arduino.h>
#include <IRremote.hpp>

#define RECHTS  1
#define MITTE   0
#define LINKS  -1
#define VOR     1
#define STOP    0
#define ZURUECK -1

// steuerung / drei relays pro seite 1 und 2 für plus und minus 3 ist für das stopen
const int L1 = 44;

const int R1 = 46;

const int einGradeDrechen = 1; // anzahl an sekunden die wir brauchen um uns 1 grad zu drehen 

//farbsensor
const int SENSOR_S0 = 5;
const int SENSOR_S1 = 4;
const int SENSOR_S2 = 7;
const int SENSOR_S3 = 6;
const int SENSOR_OUT[3] = {36, 51, 38};

const int redmin = 18;
const int grumin = 18;
const int blumin = 6;
const int redmax = 292;
const int grumax = 307;
const int blumax = 202;

// 0 = Rot; 1 = Schwarz; 2 = Weiß; 3 = Gelbs
int wirklicheFarbe;

// gemessende werte werden hier gespeichert
int farbSensorVal[3][3];
int farbfre[3][3];

int folgeFarbe = 1; // die variable die speicher welcher farbe gefolgt werden soll in rgb angabe
//!!! wichtig hier bei ist das die angaben nicht aus dem internet sind sonder die werte die wir auf der strecke messen !!!


//USS
const int sensPins[4][2] = {{2,3}, {0, 0}, {0, 0}, {0, 0}};
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
bool Car1;
bool Car2;
bool Car3;
bool Car4;

IRrecv irrecv(RECV_PIN); //Empfänger Pin
decode_results results; //erstelle Object in welches dann die Daten nach jedem scan rein wandern

void reifen(int seite, int mode)
{
  if (canDrive)
  {
    if (seite == 0)
    {
      if (mode == VOR)
      {
        pinMode(L1, LOW);
        pinMode(R1, LOW);
      }
      else if (mode == STOP)
      {
        pinMode(L1, HIGH);
        pinMode(R1, HIGH);
      }
    }
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
void getRed(int sensnum)
{
  int frequency;
  digitalWrite(SENSOR_S2, LOW);
  digitalWrite(SENSOR_S3, LOW);
  frequency = pulseIn(SENSOR_OUT[sensnum], LOW);
  farbfre[sensnum][0] = frequency;
  farbSensorVal[sensnum][0] = map(frequency, redmin, redmax, 255, 0);
  // Serial.print("rot =  ");
  // Serial.print(farbSensorVal[sensnum][0]);
  // // Serial.print(frequency);
  // Serial.print("      ");
}
void getGruen(int sensnum)
{
  int frequency;
  digitalWrite(SENSOR_S2, HIGH);
  digitalWrite(SENSOR_S3, HIGH);
  frequency = pulseIn(SENSOR_OUT[sensnum], LOW);
  farbfre[sensnum][1] = frequency;
  farbSensorVal[sensnum][1] = map(frequency, grumin, grumax, 255, 0);
  // Serial.print("greun =  ");
  // Serial.print(farbSensorVal[sensnum][1]);
  // // Serial.print(frequency);
  // Serial.print("      ");
}
void getBlue(int sensnum)
{
  int frequency;
  digitalWrite(SENSOR_S2, LOW);
  digitalWrite(SENSOR_S3, HIGH);
  frequency = pulseIn(SENSOR_OUT[sensnum], LOW);
  farbfre[sensnum][2] = frequency;
  farbSensorVal[sensnum][2] = map(frequency, blumin, blumax, 255, 0);
  // Serial.print("blau = ");
  // Serial.print(farbSensorVal[sensnum][2]);
  // //  Serial.print(frequency);
  //  Serial.println("      ");
}
void setFarbe(){
  int FarbeUnterMir[4] {0,0,0,0};
  for(int sensors=0; sensors<3; sensors++){
    if ((farbSensorVal[sensors][0] >180 && farbSensorVal[sensors][0] <230)&& (farbSensorVal[sensors][1] < 170 && farbSensorVal[sensors][1] >130) && (farbSensorVal[sensors][2] <160 && farbSensorVal[sensors][2] > 130)){
      FarbeUnterMir[0] += 1;
    }
    else if (farbSensorVal[sensors][0] < 150 && farbSensorVal[sensors][1] <150 && farbSensorVal[sensors][2] <150){
      FarbeUnterMir[1] += 1;
    }
    else if (farbSensorVal[sensors][0] > 200 && farbSensorVal[sensors][1] > 200 && farbSensorVal[sensors][2] >200){
      FarbeUnterMir[2] += 1;
    }
    else {
      FarbeUnterMir[3] += 1;
    }
  }
  int temp = 0;
// Serial.println(FarbeUnterMir[0]);
// Serial.println(FarbeUnterMir[1]);
// Serial.println(FarbeUnterMir[2]);
// Serial.println(FarbeUnterMir[3]);

  for (int x =0; x<4; x++){
    if(temp<FarbeUnterMir[x]){
      temp = FarbeUnterMir[x];
      wirklicheFarbe = x;
    }
  }
}
void updatecolcor()
{
  for (int i = 0; i < 3; i++)
  {
    getRed(i);
    getGruen(i);
    getBlue(i);
  }
  setFarbe();
  switch (wirklicheFarbe)
  {
  case 0: // wenn rot
    Serial.println("rot");
    reifen(0, STOP);  
    canDrive = false;
    break;
  case 1: // wenn Schwarz
    Serial.println("Schwarz");
    //reifen(0, STOP);
    break;
  case 2: // wenn Weiß
    Serial.println("Weiß");
    //reifen(0, VOR);
    break;
  // case 3: // wenn Gelb
  //   Serial.println("Gelb");
  //   reifen(0, VOR);
  //   break;
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
  //Warte nun auf Ampel Signal und gebe wenn Ampel Signal da das Signal an die hinteren weiter
  while(hexvalue != 0x1101){
    GetIR();
  }
  if(hexvalue == 0x1101){ //if zur sicherheit falls interrupt was durcheinander bringt
    //send nach hinten weiter damit jeder los fährt
    SendIR(0x1101, 3, 0);
  }
}
void StartGroup(int index){
  while(!synced){
    //Send Group request
    if(index == 1){
      SendIR(0x1201, 1, 0); //hinter sich und
      //SendIR(0x1201, 3, 1); //vor sich senden
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
    delay(100);
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
    Serial.println("Waiting for Other Cars to pair as ");
    Serial.print(NuminReihe);
  }
}
void Startsync(){
  hexvalue = String(0x0123);
  while(!synced){ //Wartet auf Fernbedienung auf zuweisung von nummer
    for(int i = 0; i != 100;i++){
      GetIR();
    }
    if(hexvalue == String(0xFF30CF)){ //Platzhalter für fernbiediegungs code
      NuminReihe = 1;
      Car1 = true;
      StartGroup(NuminReihe);
    }else if(hexvalue == String(0xFF18E7)){
      NuminReihe = 2;
      Car2 = true;
      StartGroup(NuminReihe);
    }
    else if(hexvalue == String(0xFF7A85)){
      NuminReihe = 3;
      Car3 = true;
      StartGroup(NuminReihe);
    }
    else if(hexvalue == String(0xFF10EF)){
      NuminReihe = 4;
      Car4 = true;
      StartGroup(NuminReihe);
    }
    //Serial.println("Waiting for Remote");
  }
}
void update(){
  //updateSensors();
  updatecolcor();
}
void machen(){
  USScheck();
}
void kal()
{
  int val[2][3] = {{1000, 1000, 1000}, {0, 0, 0}};
  int fre[2][3] = {{1000, 1000, 1000}, {0, 0, 0}};
  for (int x = 0; x < 5000; x++)
  {

    for (int i = 0; i < 3; i++)
    {
      getRed(i);
      getGruen(i);
      getBlue(i);

      if (farbSensorVal[i][0] < val[0][0])
        val[0][0] = farbSensorVal[i][0];
      if (farbSensorVal[i][1] < val[0][1])
        val[0][1] = farbSensorVal[i][1];
      if (farbSensorVal[i][2] < val[0][2])
        val[0][2] = farbSensorVal[i][2];

      if (farbSensorVal[i][0] > val[1][0])
        val[1][0] = farbSensorVal[i][0];
      if (farbSensorVal[i][1] > val[1][1])
        val[1][1] = farbSensorVal[i][1];
      if (farbSensorVal[i][2] > val[1][2])
        val[1][2] = farbSensorVal[i][2];

      if (farbfre[i][0] < fre[0][0])
        fre[0][0] = farbfre[i][0];
      if (farbfre[i][1] < fre[0][1])
        fre[0][1] = farbfre[i][1];
      if (farbfre[i][2] < fre[0][2])
        fre[0][2] = farbfre[i][2];

      if (farbfre[i][0] > fre[1][0])
        fre[1][0] = farbfre[i][0];
      if (farbfre[i][1] > fre[1][1])
        fre[1][1] = farbfre[i][1];
      if (farbfre[i][2] > fre[1][2])
        fre[1][2] = farbfre[i][2];
    }
  }
  Serial.println("--------------------------");

  Serial.print("const int redmin = ");
  Serial.println(val[0][0]);
  Serial.print("const int grumin = ");
  Serial.println(val[0][1]);
  Serial.print("const int blumin = ");
  Serial.println(val[0][2]);
  Serial.print("const int redmax = ");
  Serial.println(val[1][0]);
  Serial.print("const int grumax = ");
  Serial.println(val[1][1]);
  Serial.print("const int blumax = ");
  Serial.println(val[1][2]);

  Serial.println("--------------------------");

  Serial.print("const int redmin = ");
  Serial.println(fre[0][0]);
  Serial.print("const int grumin = ");
  Serial.println(fre[0][1]);
  Serial.print("const int blumin = ");
  Serial.println(fre[0][2]);
  Serial.print("const int redmax = ");
  Serial.println(fre[1][0]);
  Serial.print("const int grumax = ");
  Serial.println(fre[1][1]);
  Serial.print("const int blumax = ");
  Serial.println(fre[1][2]);

  Serial.println("--------------------------");
}
void setup() {
  hexvalue = "0";
  //farbsensor
  pinMode(SENSOR_S0, OUTPUT);
  pinMode(SENSOR_S1, OUTPUT);
  pinMode(SENSOR_S2, OUTPUT);
  pinMode(SENSOR_S3, OUTPUT);
  pinMode(SENSOR_OUT[0], INPUT);
  pinMode(SENSOR_OUT[1], INPUT);
  pinMode(SENSOR_OUT[2], INPUT);

  digitalWrite(SENSOR_S0, HIGH);
  digitalWrite(SENSOR_S1, LOW);

  // steuerung
  pinMode(L1, OUTPUT);

  pinMode(R1, OUTPUT);

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
  Serial.println("GO");
  Startsync();
}
void loop(){
  update();
  delay(200);
  //machen();
}
