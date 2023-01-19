#include <Arduino.h>

class Reifen {
public:
  // geschwindischkeit von 0-100% für jede seite
  int speed[2] = { 0, 0 };

  bool setup = false;
  Reifen()  // setzt alle Mosfets als output
  {
    pinMode(Mos0, OUTPUT);
    pinMode(Mos1, OUTPUT);
    pinMode(Mos2, OUTPUT);
    pinMode(Mos3, OUTPUT);
    setup = true;
  }
  void stop()  // stopt sofort
  {
    speed[0] = 0;
    speed[1] = 0;
    digitalWrite(Mos0, LOW);
    digitalWrite(Mos2, LOW);

    analogWrite(Mos1, 0);
    analogWrite(Mos3, 0);

    Serial.println("STOP");
  }
  void start()  // startet auf die zufor gesetzte geschwindischkeit
  {
    digitalWrite(Mos0, RechtsRun);
    digitalWrite(Mos2, LinksRun);
    analogWrite(Mos1, map(speed, 0, 100, 0, 255));
    analogWrite(Mos3, map(speed, 0, 100, 0, 255));
  }
  void setspeed(int newspeed)  // setzt die geschwindichkeit von 0-100
  {
    if (newspeed > 100) {
      speed[0] = 100;
      speed[1] = 100;
      start();
    } else if (newspeed < 0) {
      stop();
    } else {
      speed[0] = newspeed;
      speed[1] = newspeed;
      start();
    }
    Serial.print("Set speed to");
    Serial.println(speed[0]);
  }
  void turn(int richtung) {
    if (richtung == 0) {
      setspeedLeft(0);
      setspeedRight(100);
      start();
    } else if (richtung == 1) {
      setspeedLeft(100);
      setspeedRight(0);
      start();
    }
  }

private:
  // Variablen
  //  pins zum ansteuern von den mosfets

  // ein aus schalter für rechts PIN!!
  const int Mos0 = 37;
  // geschwindichkeits steller rechts PIN!!
  const int Mos1 = 38;
  // ein aus schalter für links PIN!!
  const int Mos2 = 9;
  // geschwindichkeits steller links PIN!!
  const int Mos3 = 10;
  // an und aus schlater Links
  int LinksRun = 0;
  // an und aus schlater Rechts
  int RechtsRun = 0;

  void setspeedLeft(int newspeed) {
    if (newspeed <= 0) {
      LinksRun = 0;
    } else if (newspeed >= 100) {
      speed[0] = 100;
      LinksRun = 1;
    } else {
      speed[0] = newspeed;
      LinksRun = 1;
    }
  }
  void setspeedRight(int newspeed) {
    if (newspeed <= 0) {
      RechtsRun = 0;
    } else if (newspeed >= 100) {
      speed[1] = 100;
      RechtsRun = 1;
    } else {
      speed[1] = newspeed;
      RechtsRun = 1;
    }
  }
};