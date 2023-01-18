#include <IRremote.hpp>
#include "reifen.cpp"  //Datei Name reicht da es in der selben Directory ist
#include "USS.cpp"
#include "farbSens.cpp"
//IR Wird automatisch eingebunden da es eine .ino Datei in der Selber Directory ist (Glaube ich)

// Variablen IR
String hexvalue;
const int RECV_PIN = 7;
const long TurnONCode = 0x1234;  // Platzhalter für Fernbedienungs code um anzuschalten

IRrecv irrecv(RECV_PIN);  // Empfänger Pin
decode_results results;   // erstelle Object in welches dann die Daten nach jedem scan rein wandern

int NuminReihe;
bool synced;
String NextTurn;  // Hat die Nächste Abbiegung von dem Vorderman in sich
bool IgnoreNextRedLine;  // Wenn das Auto vor dir gerade eine rote Linie erreicht hat und du noch nicht dran bist dann ignoriere die rote Linie
int RedLineCount;

Reifen reifen;  // noch nicht ganz Fertig
USS uss;
FarbSensoren farbsensoren;

void setup() {
  Serial.begin(9600);
  Serial.println("Test");  //wird gemacht weil der Serial beim ertsen print sonst quatch macht
  reifen.stop();           //Fahr erstmal nicht sondern warte auf Sync
  IR_pinSetup();           //Wichtig!! Muss als letztes gecalled werden da ab hier auf Sync gewartet wird
  setupcheck();
}

void loop(){
  update();
  machen();
}
void update(){
  uss.updateSensors();
  farbsensoren.updateFarben();
  GetIR();
}
void machen(){
  //IR Hexvalue Check
  if(hexvalue == 0x1101){ // Grünes zeichen von Ampel
    AmpelPing(0x1101);
  }else if(hexvalue == String(0x1210)){
    //Rote Linie erreicht, Einreihen. (Wird von index 1 gecallt und wird an alle anderen nach hinten weiter gereicht)
  }else if(hexvalue == String(0x1230)){ //Nächste Kreuzung rechts. Kommt vom Vordermann
    NextTurn = "right"; //Nächste Abbiegung rechts
  }else if(hexvalue == String(0x1231)){
    NextTurn = "left"; //Nächste Abbiegung links
  }else if(hexvalue == String(0x1220)){
    SendIR(0x1220, 2, 0); //Weiter Fahren nach hinten weiter geben 
    reifen.start(); //Und los fahren
  }
  //Farbsensor Check
  if(NuminReihe == 1 && farbsensoren.Rot && !IgnoreNextRedLine){ //Wenn Rote Linie erreicht
    RedLineReached();
    IgnoreNextRedLine = true;
  }
  //!!Wichtig!! IgnoreNextRedLine muss bei Kurven Erkennung von FarbsenS.cpp auf false gesetzt werden
}
// checkt ob alle ojeckte ihren Constructor benutzt haben
void setupcheck() {
  if (!reifen.setup) Serial.println("Reifen Setup Failed");
  if (!uss.setup) Serial.println("USS Setup Failed");
  if (!farbsensoren.setup) Serial.println("FarbSensoren Setup Failed");
  if (farbsensoren.setup && uss.setup && reifen.setup) Serial.println("Setup done");
}
