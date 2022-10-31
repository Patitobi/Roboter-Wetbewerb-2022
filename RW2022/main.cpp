#include <wiringpi.h>
//#include <studio.h>

class IRsenden{
    private:
    int senderPin;

    public:
    IRsenden(int pin){
        senderPin=pin;
        
    }
    void sendinfo(){

    }
};
class IRemfpaenger{
    private:
        int resiverPin;
    public:
    IRemfpaenger(int resiver){
        resiverPin=resiver;
    }
    int getinfo(){

    }
};
class Reifen{
    private:
        int steuerpin;
        bool mode;
    public:
        Reifen(int pin){
            steuerpin=pin;
            mode=false;

            pinMode(steuerpin, OUTPUT);
        }
        void setmode(bool mode){
            if (mode){
                //pin auf go setzen für losfahren
                digitalWrite(steuerpin, HIGH);
                mode=true;
            } else {
                //pin auf stop setzen für stopen
                digitalWrite(steuerpin, LOW);
                mode=false;
            }
        }
        bool getmode(){
            return mode;
        }
};
class Ultraschall{
    private:
        int trigger;
        int echo;
    public:
        Ultraschall(int trigger, int echo){
            trigger=trigger;
            echo=echo;

            pinMode(trigger, OUTPUT);
            pinMode(echo, INPUT);
        }
        float get_durschnitliche_distanz(int anz){
            float distanz;
            for(int i=0; i<anz; i++){
                distanz+=get_distanz();
            }
            return distanz/=anz;
        }
        float get_distanz(){
            float startTimer;
            float stoptimer;

            digitalWrite(trigger, HIGH);
            delayMicroseconds(0.01);
            digitalWrite(trigger, LOW);

            while (digitalRead(echo)==0)
            {
                startTimer=micros(); //start time
            }
            while (digitalRead(echo)==1)
            {
                stoptimer=micros(); //stoptimer
            }
            return ((stoptimer-startTimer)*34300)/2;
        }
};

int main(void){
    wiringPiSetupGpio();

    Reifen R_Rechts(int);   
    Reifen R_Links(int);

    Ultraschall AS_vorne_rechts(int, int);
    Ultraschall AS_vorne_links(int, int);
    Ultraschall AS_hinten_rechts(int, int);
    Ultraschall AS_hinten_links(int, int);

    return 0;
}