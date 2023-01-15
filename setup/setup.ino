#include <IRremote.hpp>
#include "reifen.cpp" //Datei Name reicht da es in der selben Directory ist
#include "USS.cpp"
#include "farbSens.cpp"
//IR Wird automatisch eingebunden da es eine .ino Datei in der Selber Directory ist (Glaube ich)

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

Reifen reifen; // noch nicht ganz Fertig !(variabel name muss anders sein als der der Klasse)! <- Schon gefixxt
USS uss; //--Falls du hier Errors bekommst dann versuch mal die Objekte ganz oben im Setup zu erstellen.--
FarbSensoren farbsensoren;

void setup()
{
    Serial.begin(9600);
    Serial.println("Test"); //wird gemacht weil der Serial beim ertsen print sonst quatch macht
    reifen.stop(); //Fahr erstmal nicht sondern warte auf Sync
    IR_pinSetup();//Wichtig!! Muss als letztes gecalled werden da ab hier auf Sync gewartet wird
    setupcheck();
}

void loop()
{

}
// checkt ob alle ojeckte ihren Constructor benutzt haben
void setupcheck(){
    if(!reifen.setup) Serial.println("Reifen Setup Failed");
    if(!uss.setup) Serial.println("USS Setup Failed");
    if(!farbsensoren.setup) Serial.println("FarbSensoren Setup Failed");
    if(farbsensoren.setup && uss.setup && reifen.setup) Serial.println("Setup done");
}
