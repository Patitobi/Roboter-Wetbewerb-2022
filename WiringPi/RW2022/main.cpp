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
                if(anzfehler>30||micros()-startdurschnit>1000000) return -1;
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
            digitalWrite(sensPin[sensNum][0], 1);
            delayMicroseconds(10);
            digitalWrite(sensPin[sensNum][0], 0);
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
class FarbSensor{
    protected:
    public:
    int s0 =1;
    int s1 =1;
    int s2 =1;
    int s3 =1;
    int out0 =1;
    int out1 =1;
    int out2 =1;
    int outarry[3]={out0, out1, out2};
    // werte die zur umwantlung zu rgb gebraucht wird abhänig von helischkeit
    int kalValue[3][2]={{0,1},{0,1},{0,1}};

    // rgb werte von jedem sesor
    // [0-2] sensor nummer
    // [x][0] ist rot [x][1] ist gruen [x][2] ist blau
    int colorValue[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
    FarbSensor(){
        // bestimmen der out und inputs
        pinMode(s0, OUTPUT);
        pinMode(s1, OUTPUT);
        pinMode(s2, OUTPUT);
        pinMode(s3, OUTPUT);
        pinMode(out0, INPUT);
        pinMode(out1, INPUT);
        pinMode(out2, INPUT);
        // setzt output frequency auf 20% ist gaube ich für die helischkeit der LEDs
        digitalWrite(s0, HIGH);
        digitalWrite(s1, LOW);

        // noch keine function
        kalibriren();
    }
    long map(long x, long in_min, long in_max, long out_min, long out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    bool kalibriren(){
        return true;
    }
    void updateSensorS(){
        for (int sensor : outarry){
            colorValue[sensor][0]=map(rot(sensor), kalValue[sensor][0], kalValue[sensor][1], 0, 255);
            colorValue[sensor][1]=map(gruen(sensor), kalValue[sensor][0], kalValue[sensor][1], 0, 255);
            colorValue[sensor][2]=map(blau(sensor), kalValue[sensor][0], kalValue[sensor][1], 0, 255);
        }
    } 
    // die methode blau rot und gruen sind alle gleich bis auch die farb einstellung
    int blau(int sensnum){
        // einstellung ddes sensors auf die zu messene farbe
        digitalWrite(s2, LOW);
        digitalWrite(s3, HIGH);
        //im besten fall solte alles so direckt klapen gehe aber nicht davon aus
        // recordpulselength() hat vermutlich noch die falsche einstellung wird aber noch behoben in der schule
        return recordpulselength(sensnum);
    }
    int rot(int sensnum){
        digitalWrite(s2, LOW);
        digitalWrite(s3, LOW);
        return recordpulselength(sensnum);
    }
    int gruen(int sensnum){
        digitalWrite(s2, HIGH);
        digitalWrite(s3, HIGH);
        return recordpulselength(sensnum);
    }
    int recordpulselength(int sensnum){
        int starttime = micros();
        while (digitalRead(sensnum) == LOW){}
        int stoptime = micros();
        return stoptime - starttime;
    }
};
class Auto: public Ultraschall, public Reifen, public IRemfpaenger, public IRsenden{
    protected:
    // in [x][0] sind die neusten messungen in [x][1] die vorheringen und in [x][2] die differenz der beiden
        float ultraschallsave[4][3];
        float distance;
        int distanceTimeout = 100000;
    public:
        bool run =true;
    Auto(){
        if(!setup()) run=false;
    }    
    bool loop(){
        while (run){
        update();

        execude();
        }
    }
    bool setup(){
        if(!wiringPiSetupGpio()) return false;
        else return true;
    }
    void update(){
        updateUltaschall();
    }
    void execude(){

    }
    void updateUltaschall(){
        // mutipel arry mit 4 mal 3 int
        // 0-4 steht für die sensor nummer [x]0-[x]3 sind die gemessenen werte
        // 0 ist die zuletzt gemachte messung 
        // 1 die dafor und 2 die geschwindigkeit der annäherung 
        for(int i=0;i<4;i++){
            int distanceTimeStart=micros(); // zeit speichern um eine maximale dauer festzulegen
            ultraschallsave[1][i] = ultraschallsave[0][i]; // speichert den alten wert mit den neusten
            do{
                distance = get_durschnitliche_distanz(20, i); // mist die neue distanze
            }while (distance<=0||distanceTimeout<micros()-distanceTimeStart); // nicht den timout überschritten oder error dann nochmal
            ultraschallsave[0][i] = distance; // wenn distanz richtig gemessen oder timout wird der wert gespeichert ich -1 oder gemessene distanz
            if (ultraschallsave[i][0]<=0||ultraschallsave[i][1]<=0){    // wenn einer der beiden werte -1 also falsch ist dann kann keine geschwindikeit bestimmt werden
                ultraschallsave[i][2] = -1; // dan ist geschwindikeit auch -1 also falsch
            } else ultraschallsave[i][2] = ultraschallsave[i][1]-ultraschallsave[i][0]; // wenn beide richtig dann normal
            // geschwindikeit ist beher nur die defernz der beiden werte wird noch genauer ein wert zwichen 0 und 1 soll als indikator eingebaut werden
        }
    }
};
int main(void){
    Auto Roboter;
    if (!Roboter.run) return 1;
    else Roboter.loop();
    return 0;
}
