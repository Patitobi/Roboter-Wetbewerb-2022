#include <Arduino.h>
#include "reifen.h"

// Variablen
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

// functionen
void farbSens_pinSetup();
void setFarbe();
void updatecolcor();