#include "farbSens.h"

void farbSens_pinSetup()
{
    pinMode(SENSOR_S0, OUTPUT);
    pinMode(SENSOR_S1, OUTPUT);
    pinMode(SENSOR_S2, OUTPUT);
    pinMode(SENSOR_S3, OUTPUT);
    pinMode(SENSOR_OUT, INPUT);

    digitalWrite(SENSOR_S0, HIGH);
    digitalWrite(SENSOR_S1, LOW);
}

void getRed(int sensnum)
{
    int frequency;
    digitalWrite(SENSOR_S2, LOW);
    digitalWrite(SENSOR_S3, LOW);
    frequency = pulseIn(SENSOR_OUT, LOW);
    farbSensorVal[sensnum][0] = map(frequency, redmin, redmax, 255, 0);
    Serial.print("rot =  ");
    Serial.print(farbSensorVal[sensnum][0]);
    // Serial.print(frequency);
    Serial.print("      ");
}
void getGruen(int sensnum)
{
    int frequency;
    digitalWrite(SENSOR_S2, HIGH);
    digitalWrite(SENSOR_S3, HIGH);
    frequency = pulseIn(SENSOR_OUT, LOW);
    farbSensorVal[sensnum][1] = map(frequency, grumin, grumax, 255, 0);
    Serial.print("greun =  ");
    Serial.print(farbSensorVal[sensnum][1]);
    // Serial.print(frequency);
    Serial.print("      ");
}
void getBlue(int sensnum)
{
    int frequency;
    digitalWrite(SENSOR_S2, LOW);
    digitalWrite(SENSOR_S3, HIGH);
    frequency = pulseIn(SENSOR_OUT, LOW);
    farbSensorVal[sensnum][2] = map(frequency, blumin, blumax, 255, 0);
    Serial.print("blau = ");
    Serial.print(farbSensorVal[sensnum][2]);
    // Serial.print(frequency);
    Serial.println("      ");
}

void setFarbe()
{
    for (int sensnum = 0; sensnum < 1; sensnum++)
    {
        if ((farbSensorVal[sensnum][0] <= 160 && farbSensorVal[sensnum][0] >= 100) && (farbSensorVal[sensnum][1] <= 70 && farbSensorVal[sensnum][1] >= 10) && (farbSensorVal[sensnum][2] <= 70 && farbSensorVal[sensnum][2] >= 20))
        {
            FarbeUnterMir = 0;
        }
        else if ((farbSensorVal[sensnum][0] <= 21) && (farbSensorVal[sensnum][1] <= 25) && (farbSensorVal[sensnum][2] <= 40))
        {
            FarbeUnterMir = 1;
        }
        else if ((farbSensorVal[sensnum][0] >= 225) && (farbSensorVal[sensnum][1] >= 225) && (farbSensorVal[sensnum][2] >= 225))
        {
            FarbeUnterMir = 2;
        }
        else if ((farbSensorVal[sensnum][0] <= 140 && farbSensorVal[sensnum][0] >= 114) && (farbSensorVal[sensnum][1] <= 112 && farbSensorVal[sensnum][1] >= 98) && (farbSensorVal[sensnum][2] <= 74 && farbSensorVal[sensnum][2] >= 49))
        {
            FarbeUnterMir = 3;
        }
        else
        {
            FarbeUnterMir = -1;
        }
    }
    Serial.println(FarbeUnterMir);
}
void updatecolcor()
{
    for (int i = 0; i < 1; i++)
    {
        getRed(i);
        getGruen(i);
        getBlue(i);
    }
    setFarbe();
    switch (FarbeUnterMir)
    {
    case 0: // wenn rot
        Serial.println("rot");
        reifen(0, STOP);
        break;
    case 1: // wenn Schwarß
        Serial.println("Schwarz");
        reifen(0, VOR);
        break;
    case 2: // wenn Weiß
        Serial.println("Weiß");
        break;
    case 3: // wenn Gelb
        Serial.println("Gelb");
        break;
    default: // wenn nicht erkennbar
        Serial.println("NULL");
        break;
    }
}