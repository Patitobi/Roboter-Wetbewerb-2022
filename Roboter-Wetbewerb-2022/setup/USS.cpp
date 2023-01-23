#include <Arduino.h>

class USS
{
public:
  bool setup = false;
  USS()
  {
    for (int i = 0; i < 4; i++)
    {
      pinMode(triggePin, OUTPUT);
      pinMode(sensPins[i], INPUT);
    }
    setup = true;
  }

  double entfernung[4] = {0, 0, 0, 0};
  // zum updaten von allen USS
  void updateSensors()
  {
    int temp;
    for (int i = 0; i < 4; i++)
    {
      entfernung[i] = 0;
      for (int x = 0; x < durschnittaus; x++)
      {
        entfernung[i] += updatesensor(i);
      }
      entfernung[i] /= durschnittaus;
      // Serial.print("entfernung");
      // Serial.println(entfernung[i]);
    }
  }
  // gibt alle werte die gemessen werden aus mit sensoren nummer
  void debug()
  {
    Serial.println("Senssor:      Wert:");
    Serial.print("1           ");
    Serial.println(String(updatesensor(0)));
    Serial.print("2           ");
    Serial.println(String(updatesensor(1)));
    Serial.print("3           ");
    Serial.println(String(updatesensor(2)));
    Serial.print("4           ");
    Serial.println(String(updatesensor(3)));
  }

private:
  int triggePin = 40;
  int sensPins[4] = {45, 46, 47, 48}; //0 Slot ist Input(Echo) 1 ist Output(Trigger)
  int durschnittaus = 10;

  double updatesensor(int sensnum)
  {
    long duration;
    // Clears the trigPin condition
    digitalWrite(triggePin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(triggePin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggePin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    
    duration = pulseIn(sensPins[sensnum], HIGH);
    //duration = pulseIn(sensPins[sensnum], LOW);
    
    // Calculating the distance
    //Serial.println(String(duration));
    return duration * 0.34 / 2; // Speed of sound wave divided by 2 (go and back)
  }
};
