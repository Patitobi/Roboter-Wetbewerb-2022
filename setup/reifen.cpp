#include <Arduino.h>
class Reifen
{
private:
  // Variablen
  //  pins zum ansteuern von den mosfets
  // ein aus schalter für rechts
  const int Mos0 = 12;
  // geschwindichkeits steller rechts
  const int Mos1 = 10;
  // ein aus schalter für links
  const int Mos2 = 8;
  // geschwindichkeits steller links
  const int Mos3 = 9;
  // geschwindischkeit von 0-100%
  int speed = 0;

public:
  bool setup = false;
  Reifen() // setzt alle Mosfets als output
  {
    pinMode(Mos0, OUTPUT);
    pinMode(Mos1, OUTPUT);
    pinMode(Mos2, OUTPUT);
    pinMode(Mos3, OUTPUT);
    setup = true;
  }
  void stop() // stopt sofort
  {
    speed = 0;
    digitalWrite(Mos0, LOW);
    digitalWrite(Mos2, LOW);

    analogWrite(Mos1,0);
    analogWrite(Mos3,0);

    Serial.println("STOP");
  }
  void start() // startet auf die zufor gesetzte geschwindischkeit
  {
    if (speed <= 0)
    {
      Serial.println("Reifen:'Kein speed angegeben'");
      Serial.println(speed);
    }
    else
    {
      digitalWrite(Mos0, HIGH);
      digitalWrite(Mos2, HIGH);
      analogWrite(Mos1, map(speed, 0, 100, 0, 255));
      analogWrite(Mos3, map(speed, 0, 100, 0, 255));
    }
  }
  void setspeed(int newspeed) // setzt die geschwindichkeit von 0-100
  {
    if (newspeed > 100)
    {
      speed = 100;
      start();
    }
    else if (newspeed < 0)
    {
      stop();
    }
    else
    {
      speed = newspeed;
      start();
    }
    Serial.print("Set speed to");
    Serial.println(speed);
  }
  int getspeed() // gibt die akktuel eingestellte geschwindichkeit wieder
  {
    return speed;
  }
};
