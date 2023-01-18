#include <Arduino.h>

class FarbSensoren {
public:
  const int anzSensoren = 1;

  bool setup = false;
  FarbSensoren() {
    pinMode(SENSOR_S0, OUTPUT);
    pinMode(SENSOR_S1, OUTPUT);
    pinMode(SENSOR_S2, OUTPUT);
    pinMode(SENSOR_S3, OUTPUT);
    pinMode(SENSOR_OUT[0], INPUT);
    pinMode(SENSOR_OUT[1], INPUT);
    pinMode(SENSOR_OUT[2], INPUT);

    digitalWrite(SENSOR_S0, HIGH);
    digitalWrite(SENSOR_S1, LOW);

    setup = true;
  }
  /* Farben die von den Sensoren gemessen wurden
    Rot=0; Schwartz=1; Weiss=2*/
  int farben[3] = { -1, -1, -1 };

  bool Rot = false;

  void debug()  // gibt die akktuel gemessenden daten aus !!ohne clacFarben!!
  {
    for (int sensor = 0; sensor < anzSensoren; sensor++) {
      Serial.print("Sensor");
      Serial.println(sensor);

      Serial.print("Rot = ");
      getRed(sensor);
      Serial.println(farbSensorVal[sensor][0]);

      Serial.print("Gruen = ");
      getGruen(sensor);
      Serial.println(farbSensorVal[sensor][1]);

      Serial.print("Blau = ");
      getBlue(sensor);
      Serial.println(farbSensorVal[sensor][2]);
    }
  }
  void clacdebug()  // gibt die akktuel gemessenden daten aus !!mit clacFarben!!
  {
    updateFarben();

    Serial.print("Sensor 0  ");
    Serial.println(farben[0]);

    Serial.print("Sensor 1  ");
    Serial.println(farben[1]);

    Serial.print("Sensor 2  ");
    Serial.println(farben[2]);
  }
  void updateFarben() {
    for (int i = 0; i < anzSensoren; i++) {
      getRed(i);
      getGruen(i);
      getBlue(i);

      calcFarbe(i);
    }
    redCheck();
  }

private:
  // Variablen
  const int SENSOR_S0 = 6;
  const int SENSOR_S1 = 7;

  const int SENSOR_S2 = 4;
  const int SENSOR_S3 = 3;
  // pin andem das singnal zurück kommt
  const int SENSOR_OUT[3] = { 5, 0, 0 };

  // gemessende werte werden hier gespeichert
  int farbSensorVal[3][3];  // [x][] ist der sensor 0-3; [][x] ist der wert in rgb reifolge 0-3

  // berechnent die Farbe des Sensors anhant dem Farbverhältnisse
  void calcFarbe(int sensnum) {
    int prozWert;
    int summe;
    int prozWerte[3];
    int farbVote[3];
    for (int i = 0; i < 3; i++) {
      summe += farbSensorVal[sensnum][i];
    }
    if (summe >= 100)
      farben[sensnum] = 1;
    else if (summe <= 45)
      farben[sensnum] = 2;
    prozWert = summe / 100;
    for (int i = 0; i < 3; i++) {
      prozWerte[i] = farbSensorVal[sensnum][i] * prozWert;
    }
    if (prozWerte[0] < prozWerte[1] && prozWerte[0] < prozWerte[3]) {
      farben[sensnum] = 0;
    }
  }
  void redCheck() {
    int count;
    for (int i = 0; i < 3; i++) {
      if (farben[i] == 0)
        count++;
    }
    if (count >= 2) {
      Rot = true;
    }
  }
  void getRed(int sensnum) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(SENSOR_S2, LOW);
      digitalWrite(SENSOR_S3, LOW);
      farbSensorVal[sensnum][0] = pulseIn(SENSOR_OUT[sensnum], LOW);
    }
    farbSensorVal[sensnum][0] /= 2;
  }
  void getGruen(int sensnum) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(SENSOR_S2, HIGH);
      digitalWrite(SENSOR_S3, HIGH);
      farbSensorVal[sensnum][1] = pulseIn(SENSOR_OUT[sensnum], LOW);
    }
    farbSensorVal[sensnum][1] /= 2;
  }
  void getBlue(int sensnum) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(SENSOR_S2, LOW);
      digitalWrite(SENSOR_S3, HIGH);
      farbSensorVal[sensnum][2] = pulseIn(SENSOR_OUT[sensnum], LOW);
    }
    farbSensorVal[sensnum][2] /= 2;
  }
};
