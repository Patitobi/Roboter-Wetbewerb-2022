#include <Arduino.h>

class Reifen {
private:
  // Variablen
  //  pins zum ansteuern von den mosfets
  const int Mos0 = 9;
  const int Mos1 = 10;
  const int Mos2 = 0;
  const int Mos3 = 0;
  // geschwindischkeit von 0-100%
  int speed = 0;

public:
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
    speed = 0;
    for (int mos = 0; mos < 4; mos++) {
      analogWrite(mos, 0);
    }
    Serial.println("STOP");
  }
  void start()  // startet auf die zufor gesetzte geschwindischkeit
  {
    if (speed <= 0) {
      Serial.println("Reifen:'Kein speed angegeben'");
    }
    for (int mos = 0; mos < 4; mos++) {
      analogWrite(mos, map(0, 100, 0, 255, speed));
    }
  }
  void setspeed(int newspeed) // setzt die geschwindichkeit von 0-100
   {
    if (newspeed > 100)
      speed = 100;
    else if (newspeed < 0)
      stop();
  }
  int getspeed() // gibt die akktuel eingestellte geschwindichkeit wieder 
  {
    return speed;
  }
};
