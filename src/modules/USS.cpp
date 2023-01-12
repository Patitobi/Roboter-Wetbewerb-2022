<<<<<<< HEAD
#include "USS.h"

void UUS_pinSetup()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(sensPins[i][1], OUTPUT);
        pinMode(sensPins[i][0], INPUT);
    }
    pinMode(34, OUTPUT);
    pinMode(30, OUTPUT);
}

// für einzelne
int updatesensor(int sensnum)
{
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
    return duration * 0.34 / 2; // Speed of sound wave divided by 2 (go and back)
}
// zum updaten von allen USS
void updateSensors()
{
    int temp;
    for (int i = 0; i < 4; i++)
    {
        entfernung[i] = 0;
        temp = 0;
        for (int x = 0; x < durschnittaus; x++)
        {
            temp = updatesensor(i);
            if (temp > 1200 || temp < 0)
            {
                temp = updatesensor(i);
            }
            entfernung[i] += temp;
        }
        entfernung[i] /= durschnittaus;
        Serial.print("entfernung");
        Serial.println(entfernung[i]);
    }
}
void USScheck()
{
    for (int i = 0; i < 1; i++)
    {
        if (entfernung[i] <= 300)
        {
            Serial.println("stop");
            digitalWrite(30, LOW);
            digitalWrite(34, LOW);

            reifen(0, STOP);
        }
        else
        {
            digitalWrite(30, HIGH);
            digitalWrite(34, HIGH);
            reifen(0, VOR);
        }
    }
=======
#include "USS.h"

void UUS_pinSetup()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(sensPins[i][1], OUTPUT);
        pinMode(sensPins[i][0], INPUT);
    }
    pinMode(34, OUTPUT);
    pinMode(30, OUTPUT);
}

// für einzelne
int updatesensor(int sensnum)
{
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
    return duration * 0.34 / 2; // Speed of sound wave divided by 2 (go and back)
}
// zum updaten von allen USS
void updateSensors()
{
    int temp;
    for (int i = 0; i < 4; i++)
    {
        entfernung[i] = 0;
        temp = 0;
        for (int x = 0; x < durschnittaus; x++)
        {
            temp = updatesensor(i);
            if (temp > 1200 || temp < 0)
            {
                temp = updatesensor(i);
            }
            entfernung[i] += temp;
        }
        entfernung[i] /= durschnittaus;
        Serial.print("entfernung");
        Serial.println(entfernung[i]);
    }
}
void USScheck()
{
    for (int i = 0; i < 1; i++)
    {
        if (entfernung[i] <= 300)
        {
            Serial.println("stop");
            digitalWrite(30, LOW);
            digitalWrite(34, LOW);

            reifen(0, STOP);
        }
        else
        {
            digitalWrite(30, HIGH);
            digitalWrite(34, HIGH);
            reifen(0, VOR);
        }
    }
>>>>>>> 6f5105e9205dab421841d0833d582a0e853fb752
}