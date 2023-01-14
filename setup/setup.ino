#include <IRremote.hpp>

// Variablen Reifen
//  pins zum ansteuern von den motoren
const int L1 = 9;
const int R1 = 10;

#define VOR 1
#define STOP 0

// Variablen USS
const int sensPins[4][2] = {{45, 40}, {46, 40}, {47, 40}, {48, 40}}; // 1 Slot ist Output 2 Slot ist Input
int entfernung[4] = {0, 0, 0, 0};
int durschnittaus = 10;

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

// Variablen Farben
const int SENSOR_S0 = 22;
const int SENSOR_S1 = 24;
const int SENSOR_S2 = 26;
const int SENSOR_S3 = 28;
const int SENSOR_OUT = 8;

const int redmin = 28;
const int redmax = 178;
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

void update()
{
	updateSensors();
	// updatecolcor();
}

void machen()
{
	// USScheck();
}

void setup()
{
	reifen_pinSetup();
	farbSens_pinSetup();
	UUS_pinSetup();
	
	// als lestztes wegen den sync
	IR_pinSetup();
}

void loop()
{
	// update();
	// machen();
}
