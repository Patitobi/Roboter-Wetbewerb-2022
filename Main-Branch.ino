#include <Arduino.h>
#include <IRremote.hpp>

#define RECHTS  1
#define MITTE   0
#define LINKS  -1
#define VOR     1
#define STOP    0
#define ZURUECK-1

// steuerung / drei relays pro seite 1 und 2 für plus und minus 3 ist für das stopen
const int L1 = 0;
const int L2 = 0;
const int L3 = 0;

const int R1 = 0;
const int R2 = 0;
const int R3 = 0;

//farbsensor
const int SENSOR_S0 = 5;
const int SENSOR_S1 = 4;
const int SENSOR_S2 = 7;
const int SENSOR_S3 = 6;
const int SENSOR_OUT =8;

const int redmin = 22;
const int redmax= 381;
const int grumin = 22;
const int grumax = 406;
const int blumin = 15;
const int blumax = 254;
// gemessende werte werden hier gespeichert
int farbSensorVal[3][3];

//Allgemeine Variablen
// definirt in welche 
int fahrRichtung = 0;

int folgeFarbe[3]; // die variable die speicher welcher farbe gefolgt werden soll in rgb angabe
//!!! wichtig hier bei ist das die angaben nicht aus dem internet sind sonder die werte die wir auf der strecke messen !!!


//USS
const int sensPins[4][2] = {{2,3}, {0, 0}, {0, 0}, {0, 0}};
int entfernung[4] = {0,0,0,0};

//Allgemeine Variablen
int NuminReihe;
String NextMoveBehindMe; //Hat das Vorhaben von dem Auto vor sich in sich
String NextMoveInfrontOfMe; //Hat das Vorhaben von dem Auto hinter sich in sich

//IR
const int RECV_PIN = 7;
const String hexcode1 = "0x1111";
const String hexcode2 = "0x1112";
const String hexcode3 = "0x1113";

IRrecv irrecv(RECV_PIN); //Empfänger Pin
decode_results results; //erstelle Object in welches dann die Daten nach jedem scan rein wandern

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
void AmpelPing(long Code){
  //Wenn dieses Auto erster in der Reihe dann reiche die Info nach hinten weiter.
  if(NuminReihe != 5){
    //Pass IR Signal to the Cars behind
    SendIR(Code, 3, 0); //Send 3x hinter dich (0 =  nach hinten)
  }
}
void GetIR(){
  //code der nicht unterbrochen werden darf
  if (irrecv.decode(&results)){ //Wenn irgendwas auf dem recvPin Gelesen wird dann 
      Serial.println(results.value, HEX); //Print in den Arduino Log (HEX)
      String hexvalue = String(results.value, HEX); //Hier kurz eine variable machen um nicht in allen if statements die funktion durch zu führen (performance)
      if(hexvalue.indexOf("0x11") > 0){ //Schaut ob 0x11 im String irgendwo vorhanden ist. Denn 0x11 sind die ersten 2 ziffern des ampel hex codes
        AmpelPing(results.value);
      }
      irrecv.resume(); //Reset + es wird wieder vom Pin auf Info gewartet.
  }
}
void CodetobeexecutedonInterrupt(){
  GetIR();
}

void setup() {
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
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);

  //USS
  for (int i=0; i<4; i++){
    pinMode(sensPins[i][1], OUTPUT); 
    pinMode(sensPins[i][0], INPUT);
  }
  pinMode(34, OUTPUT);
  pinMode(30, OUTPUT);
  //IR
  //Recieve IR
  irrecv.enableIRIn(); //Fang an zu Lesen
  irrecv.blink13(true); //Default Pin auf dem Board welcher blinkt wenn Pin was ließt
  //Send IR
  pinMode(LED_BUILTIN, OUTPUT);
  IrSender.begin(3);
  attachInterrupt(digitalPinToInterrupt(3), CodetobeexecutedonInterrupt, CHANGE); //wenn sich pin 3 ändert dann führe interruptcode aus
  Serial.begin(9600);
}
void reifen(int seite, int mode){
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
void getRed(int sensnum){
  int frequency;
  digitalWrite(SENSOR_S2, LOW);
  digitalWrite(SENSOR_S3, LOW);
  frequency = pulseIn(SENSOR_OUT, LOW);
  farbSensorVal[sensnum][0] = map(frequency, redmin, redmax, 255, 0);
}
void getGruen(int sensnum){
  int frequency;
  digitalWrite(SENSOR_S2, HIGH);
  digitalWrite(SENSOR_S3, HIGH);
  frequency = pulseIn(SENSOR_OUT, LOW);
  farbSensorVal[sensnum][1] = map(frequency, grumin, grumax, 255, 0);
}
void getBlue(int sensnum){
  int frequency;
  digitalWrite(SENSOR_S2, LOW);
  digitalWrite(SENSOR_S3, HIGH);
  frequency = pulseIn(SENSOR_OUT, LOW);
  farbSensorVal[sensnum][2] = map(frequency, blumin, blumax, 255, 0);
}
void updatecolcor(){
  for (int i=0; i<3; i++){
    getRed(i);
    getGruen(i);
    getBlue(i);
  }
}
void farbcheck(){
  //der in fahrtrichtung linke sender ist IMMER 0 mitte 1 rechts 2
  for (int sensor=0; sensor<3; sensor++){
    for (int rgb=0; rgb<3; rgb++)
      if (farbSensorVal[sensor][rgb]+10!=folgeFarbe[0]||farbSensorVal[sensor][rgb]-10!=folgeFarbe[0]){ // guckt ob der wehrt dem gesuchten wert entspricht
        // tollerranz von 10 einheiten abweichung zum gesuchten wert
        if (sensor == 0){
          // nach rechts korigiren
        }
        if (sensor == 1){
          // erstam nix
        }
        if (sensor == 2){
          // nach links korigiren
        }
      }
  }
}
void USScheck(){
  for (int i=0; i<1;i++){
    if (entfernung[i]<=200){
      Serial.println("stop");
      reifen(0, STOP);
    } else {
      reifen(0, VOR);
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
