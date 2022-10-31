#include <wiringpi.h>
//#include <studio.h>
//#include <chrono>

class IRsenden{
    private:
    public:
    IRsenden(){

    }
};
class IRemfpaenger{
    private:
    public:
    IRemfpaenger(){

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
        }
        void setmode(bool mode){
            if (mode){
                //pin auf go setzen für losfahren
                mode=true;
            } else {
                //pin auf stop setzen für stopen
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

            digitalWrite(trigger, 1);
            delay(0.1);
            digitalWrite(trigger, 0);

            while (digitalRead(echo)==0)
            {
                startTimer=0; //start time
            }
            while (digitalRead(echo)==1)
            {
                stoptimer=0; //stoptimer
            }
            return ((stoptimer-startTimer)*34300)/2;
        }

};

int main(void){
    Reifen R_Rechts(int);   
    Reifen R_Links(int); 

    return 0;
}