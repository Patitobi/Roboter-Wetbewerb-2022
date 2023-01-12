#include <Arduino.h>
#include <IRremote.hpp>

//IR
String hexvalue;
const int RECV_PIN = 7;
const long TurnONCode = 0x1234; //Platzhalter für Fernbedienungs code um anzuschalten

IRrecv irrecv(RECV_PIN); //Empfänger Pin
decode_results results; //erstelle Object in welches dann die Daten nach jedem scan rein wandern

void SendIR(long Code, int repeat, int dir);
void GetIR();