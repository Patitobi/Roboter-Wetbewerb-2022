#include <Arduino.h>

class Reifen
{
public:
  // geschwindischkeit von 0-100% für jede seite
  int speedL = 0;
  int speedR = 0;
  bool whiteLine = false;

  bool setup = false;
  Reifen() // setzt alle Mosfets als output
  {
    pinMode(MosR, OUTPUT);
    pinMode(MosL, OUTPUT);
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
    speedL = 0;
    speedR = 0;
    analogWrite(MosR, 0);
    analogWrite(MosL, 0);
    
    Serial.println("STOP");
  }
  void start() // startet auf die zufor gesetzte geschwindischkeit
  {
    // analogWrite(MosR, 255);
    // analogWrite(MosL, 255);
    analogWrite(MosL, speedL);
    analogWrite(MosR, speedR);
  }
  void setspeed(int newspeed) // setzt die geschwindichkeit von 0-100
  {
    if (newspeed > 100)
    {
      speedL = 255;
      speedR = 255;
      start();
    }
    else if (newspeed < 0)
    {
      stop();
    }
    else
    {
      speedL = newspeed + 150;
      speedR = newspeed + 150;
      start();
    }
    Serial.print("Set speed to");
    Serial.println(speedL);
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
  const int MosR = 9;
  // geschwindichkeits steller links PIN!!
  const int MosL = 10;
  // richtung in die wir fahren MÜSSEN wird 0 1 2
  int drive = 1;
  // time andem das erste mal korigirt wurde
  unsigned long starttimer;
  // ob wir gerade am korrigere sind (für timer wichtig)
  bool korrigere = false;
  // in welchen abständen wir korigiren sollen (gerade 1sec)
  unsigned long korrekturinterval = 300;

  void fahren(){ // wenn auto wieder richtig auf kurs ist
    if (drive == 1 && speedL != speedR){
      setspeed(100);
    }
    else if (drive = 0){
      if (millis() - starttimer + korrekturinterval < 0){
        setspeedLeft(0);
        setspeedRight(100);
      }
      else if (millis() - starttimer + korrekturinterval > 0){
        setspeedLeft(50);
        setspeedRight(100);
      }
      else if (millis() - starttimer + korrekturinterval * 2 > 0){
        setspeedLeft(75);
        setspeedRight(100);
        korrigere = false;
      }
      start();
    }
    else if (drive = 2){
      if (millis() - starttimer + korrekturinterval < 0){
        setspeedLeft(100);
        setspeedRight(0);
      }
      else if (millis() - starttimer + korrekturinterval > 0){
        setspeedLeft(100);
        setspeedRight(50);
      }
      else if (millis() - starttimer + korrekturinterval * 2 > 0){
        setspeedLeft(100);
        setspeedRight(75);
        korrigere = false;
      }
      start();
    }
    if (drive != 1 && !korrigere){
      starttimer = millis();
      korrigere = true;
    }
  }
  void setspeedLeft(int newspeed)
  {
    if (newspeed <= 0){
      speedL = 0;
    }
    else if (newspeed >= 100){
      speedL = 255;
    }
    else{
      speedL = newspeed + 150;
    }
  }
  void setspeedRight(int newspeed)
  {
    if (newspeed <= 0){
      speedR = 0;
    }
    else if (newspeed >= 100){
      speedR = 255;
    }
    else{
      speedR = newspeed + 150;
    }
  }
};
