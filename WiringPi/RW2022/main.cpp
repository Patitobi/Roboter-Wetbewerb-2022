#include <wiringPi.h>
#include <stdio.h>

class IRsenden{
    public:
        int senderPin[3]= {1,1,1};
        IRsenden(){
            
        }
        void sendinfo(){

        }
};
class IRemfpaenger{
    public:
        int resiverPin[3] = {1,1,1};
        IRemfpaenger(){
        }
        int getinfo(){
            return 0;
        }
};
class Reifen{
    public:
        int steuerpin[2] = {1, 1};
        bool mode;
        Reifen(){
            mode=false;

            pinMode(steuerpin[0], OUTPUT);
            pinMode(steuerpin[1], OUTPUT);
        }
        void setmode(bool mode, int seite){
            if (mode){
                //pin auf go setzen für losfahren
                digitalWrite(steuerpin[seite], HIGH);
                mode=true;
            } else {
                //pin auf stop setzen für stopen
                digitalWrite(steuerpin[seite], LOW);
                mode=false;
            }
        }
        bool getmode(){
            return mode;
        }
};
class Ultraschall{
    public:
        int sensPin[4][2] = {{23,24},{1,1},{1,1},{1,1}};
        int starttime;
        int stoptime;
        int sendpin;
        int recievepin;
        int now;
        int timeout = 12000;
        int time;
        int anzfehler;
        int startdurschnit;
        Ultraschall(){
            for (int i=0; i<4;i++){
                pinMode(sensPin[i][0], OUTPUT);
                pinMode(sensPin[i][1], INPUT);
                digitalWrite(sensPin[i][0], LOW);
                delay(50);
            }
            
        }
        float get_durschnitliche_distanz(int anz, int sensNum){
            anzfehler=0;
            //speichern der zeit beim begin
            startdurschnit = micros();
            // anz ist die anzahl an messungen aus der der duschnitt genommen werden soll
            float distance;
            float tempdist;
            float durschnitt;
            //loop bis anzahl an richtigen ergebniss ereicht ist
            for(int i=0; i<anz; i++){
                //wenn zu viele fehler sind oder die zeitableut wird 0 zurückgegeben
                //damit sich das Program nicht aufhängt
                if(anzfehler>30||micros()-startdurschnit>1000000) return 0;
                //gibt den abstand in mM zurück
                tempdist=get_distanz(sensNum);
                //um den durschnitt wehrend jedem durschlauf zu haben
                if (i>anz/4){
                    durschnitt=distance/i;
                }
                if (tempdist<20){
                    //wenn messung immernoch falsch dann wird die messung nochmal durschgefürt
                    //indem der schleifen index -1 gerechnet wird
                    i--;
                    anzfehler++;
                    continue;
                }   else if(i>anz/4&&(durschnitt-200>tempdist||durschnitt+200<tempdist)){
                    //ab einem fiertel der durschleufe durschleufen werden die ergebnisse mit dem bisheringen duschnitt verglichen wenn abstand zu groß den wird neu gemessen
                    i--;
                    anzfehler++;
                    continue;
                } distance+=tempdist;
            }
            return distance/anz;
        }
        float get_distanz(int sensNum){
            //es wird ein singnal von 10qs an den sensor gesendet
            digitalWrite(sendpin, 1);
            delayMicroseconds(10);
            digitalWrite(sendpin, 0);
            //die zeit vor dem auslesenen der rückgabe wird gespeichet
            now = micros();
            //wenn kein rückgabe singnal ankommt und nicht der tiout überschritten wird 
            while(digitalRead(recievepin == LOW) && micros()-now<timeout){
                //die länge der singnals wird gemessen 
                time = recordpulselength();
                //wenn die länge des singnal zu klein ist, ist die messung vermutlich falsch
                //weil wenn kein sinal ankommt das programm trotzdem 2qs braucht zum ausfüren das würde als ergebnis zählen was aber falsch ist
                if (time<50){
                    continue;
                } else break;
            }
            //formal zum umrechnen von qS in mM
            float finDistance = ((time*343)/1000)/2;
        return finDistance;
        }
        int recordpulselength(){
            //speicher der zeit bevor der recievepin aus 1 geht
            starttime = micros();
            while (digitalRead(recievepin) == HIGH){}
            //speicher der zeit nachdem der recievepin aus 1 war
            stoptime = micros();
            //die differenz zwichen start und stop wird ausgegeben
            return stoptime - starttime;
        }
};
class Auto: public Ultraschall, public Reifen, public IRemfpaenger, public IRsenden{
    protected:
    public:
        bool run =true;
    Auto(){
        if(!setup()) run=false;
        
        while (run){
            
        }
    }
    bool setup(){
        if(wiringPiSetupGpio()) return true;
        else return false;
    }
    void update(){

    }
    void execude(){

    }
};
int main(void){
    Auto Roboter;
    if (!Roboter.run) return 1;
    else return 0;
}
