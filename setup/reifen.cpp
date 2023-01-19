#include <Arduino.h>

class Reifen
{
public:
  // geschwindischkeit von 0-100% für jede seite
  int speed[2] = {0, 0};
  bool whiteLine = false;

  bool setup = false;
  Reifen() // setzt alle Mosfets als output
  {
    pinMode(Mos1, OUTPUT);
    pinMode(Mos3, OUTPUT);
    setup = true;
  }
  void update(int farben[3]) // bestimmt die richtung in die gefaren wird basirens auf dem farbsensor
  {
    if (farben[0] == 2 || farben[2] == 2)
    {
      if (farben[0] == 2 && farben[1] == 2 && farben[2] == 2)
      {
        whiteLine = true;
      }
      else if (farben[0] == 2 && (farben[1] == 2 || farben[1] == 1))
      {
        drive = 0;
      }
      else if (farben[2] == 2 && (farben[1] == 2 || farben[1] == 1))
      {
        drive = 2;
      }
    }
    else
    {
      drive = 1;
    }
    fahren();
  }
  void stop() // stopt sofort
  {
    speed[0] = 0;
    speed[1] = 0;
    analogWrite(Mos1, 0);
    analogWrite(Mos3, 0);

    Serial.println("STOP");
  }
  void start() // startet auf die zufor gesetzte geschwindischkeit
  {
    analogWrite(Mos1, map(speed, 0, 100, 0, 255));
    analogWrite(Mos3, map(speed, 0, 100, 0, 255));
  }
  void setspeed(int newspeed) // setzt die geschwindichkeit von 0-100
  {
    if (newspeed > 100)
    {
      speed[0] = 100;
      speed[1] = 100;
      start();
    }
    else if (newspeed < 0)
    {
      stop();
    }
    else
    {
      speed[0] = newspeed;
      speed[1] = newspeed;
      start();
    }
    Serial.print("Set speed to");
    Serial.println(speed[0]);
  }
  void turn90(int richtung)
  {
    if (richtung == 0)
    {
      setspeedLeft(0);
      setspeedRight(100);
      start();
    }
    else if (richtung == 1)
    {
      setspeedLeft(100);
      setspeedRight(0);
      start();
    }
  }

private:
  //  pins zum ansteuern von den mosfets

  // geschwindichkeits steller rechts PIN!!
  const int Mos1 = 38;
  // geschwindichkeits steller links PIN!!
  const int Mos3 = 10;
  // richtung in die wir fahren MÜSSEN wird 0 1 2
  int drive = 1;
  // time andem das erste mal korigirt wurde
  unsigned long starttimer;
  // ob wir gerade am korrigere sind (für timer wichtig)
  bool korrigere = false;
  // in welchen abständen wir korigiren sollen (gerade 1sec)
  unsigned long korrekturinterval = 1000;

  void fahren()
  { // wenn auto wieder richtig auf kurs ist
    if (drive == 1 && speed[0] != speed[1])
    {
      setspeed(100);
    }
    else if (drive = 0)
    {
      if (millis() - starttimer + korrekturinterval < 0)
      {
        setspeedLeft(0);
        setspeedRight(100);
      }
      else if (millis() - starttimer + korrekturinterval > 0)
      {
        setspeedLeft(50);
        setspeedRight(100);
      }
      else if (millis() - starttimer + korrekturinterval * 2 > 0)
      {
        setspeedLeft(75);
        setspeedRight(100);
        korrigere = false;
      }
      start();
    }
    else if (drive = 2)
    {
      if (millis() - starttimer + korrekturinterval < 0)
      {
        setspeedLeft(100);
        setspeedRight(0);
      }
      else if (millis() - starttimer + korrekturinterval > 0)
      {
        setspeedLeft(100);
        setspeedRight(50);
      }
      else if (millis() - starttimer + korrekturinterval * 2 > 0)
      {
        setspeedLeft(100);
        setspeedRight(75);
        korrigere = false;
      }
      start();
    }
    if (drive != 1 && !korrigere)
    {
      starttimer = millis();
      korrigere = true;
    }
  }
  void setspeedLeft(int newspeed)
  {
    if (newspeed <= 0)
    {
      speed[0] = 0;
    }
    else if (newspeed >= 100)
    {
      speed[0] = 100;
    }
    else
    {
      speed[0] = newspeed;
    }
  }
  void setspeedRight(int newspeed)
  {
    if (newspeed <= 0)
    {
      speed[1] = 0;
    }
    else if (newspeed >= 100)
    {
      speed[1] = 100;
    }
    else
    {
      speed[1] = newspeed;
    }
  }
};