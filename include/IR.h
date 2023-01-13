#include <Arduino.h>
#include <IRremote.hpp>
//#include "reifen.h"
//#include "USS.h"

// Variablen
String hexvalue;
const int RECV_PIN = 7;
const long TurnONCode = 0x1234; // Platzhalter für Fernbedienungs code um anzuschalten

IRrecv irrecv(RECV_PIN); // Empfänger Pin
decode_results results;  // erstelle Object in welches dann die Daten nach jedem scan rein wandern

int NuminReihe;
bool synced;
String NextMoveBehindMe;    // Hat das Vorhaben von dem Auto vor sich in sich
String NextMoveInfrontOfMe; // Hat das Vorhaben von dem Auto hinter sich in sich

// functionen
void IR_pinSetup();
void SendIR(long Code, int repeat, int dir);
void GetIR();
void AmpelPing(long Code);
void AmpelAnfahrt();
void RedLineReached();
