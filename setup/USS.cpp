#include <Arduino.h>

class USS
{
public:
  bool setup = false;
  USS()
  {
    for (int i = 0; i < 4; i++)
    {
      pinMode(sensPins[i][0], OUTPUT);
      pinMode(sensPins[i][1], INPUT);
    }
    setup = true;
  }

  int entfernung[4] = {0, 0, 0, 0};
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
      // Serial.print("entfernung");
      // Serial.println(entfernung[i]);
    }
  }
  // gibt alle werte die gemessen werden aus mit sensoren nummer
void USSDebug(){
    Serial.println("entfernung 1" + " " + String(entfernung[0]));
    Serial.println("entfernung 2" + " " + String(entfernung[1]));
    Serial.println("entfernung 3" + " " + String(entfernung[2]));
    Serial.println("entfernung 4" + " " + String(entfernung[3]));
    Serial.println(" ");
}

private:
  const int sensPins[4][2] = {{45, 40}, {46, 40}, {47, 40}, {48, 40}}; // 0 ist Output(Trigger) 1 Slot ist Input(Echo)
  int durschnittaus = 10;

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
};