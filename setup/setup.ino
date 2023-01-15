#include <IRremote.hpp>
#include "C:\Users\kesse\Documents\Arduino\RP\setup\reifen.cpp"
#include "C:\Users\kesse\Documents\Arduino\RP\setup\USS.cpp"
#include "C:\Users\kesse\Documents\Arduino\RP\setup\farbSens.cpp"

// Variablen IR
String hexvalue;
const int RECV_PIN = 7;
const long TurnONCode = 0x1234; // Platzhalter für Fernbedienungs code um anzuschalten

IRrecv irrecv(RECV_PIN); // Empfänger Pin
decode_results results;  // erstelle Object in welches dann die Daten nach jedem scan rein wandern

int NuminReihe;
bool synced;
String NextMoveBehindMe;    // Hat das Vorhaben von dem Auto vor sich in sich
String NextMoveInfrontOfMe; // Hat das Vorhaben von dem Auto hinter sich in sich

Reifen Reifen; // noch nicht ganz Fertig
USS USS;
FarbSensoren FarbSensoren;

void setup()
{
    Serial.begin(9600);
    Serial.println("Test"); //wird gemacht weil der Serial beim ertsen print sonst quatch macht
    setupcheck();
}

void loop()
{

}
// checkt ob alle ojeckte ihren Constructor benutzt haben
void setupcheck(){
    if(!Reifen.setup) Serial.print("Reifen Setup False");
    if(!USS.setup) Serial.print("USS Setup False");
    if(!FarbSensoren.setup) Serial.print("FarbSensoren Setup False");
    if(FarbSensoren.setup&&USS.setup&&Reifen.setup) Serial.print("Setup done");
}
