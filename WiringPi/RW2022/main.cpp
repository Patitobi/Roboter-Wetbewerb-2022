#include <wiringPi.h>
#include <stdio.h>

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
        return 0;
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
            this->trigger=trigger;
            this->echo=echo;
            pinMode(trigger, OUTPUT);
            pinMode(echo, INPUT);
            digitalWrite(trigger, LOW);
            delay(500);
        }
        float get_durschnitliche_distanz(int anz){
            float distanz;
            for(int i=0; i<anz; i++){
                distanz+=get_distanz();
            }
            return distanz/=anz;
        }
        double get_distanz(){
        delay(10);

        digitalWrite(trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigger, LOW);

        now=micros();

        while (digitalRead(echo) == LOW && micros()-now<timeout);
            recordPulseLength();

        travelTimeUsec = endTimeUsec - startTimeUsec;
        distanceMeters = 100*((travelTimeUsec/1000000.0)*340.29)/2;

        return distanceMeters;
    }
    recordPulseLength(){
        startTimeUsec = micros();
        while ( digitalRead(echo) == HIGH );
        endTimeUsec = micros();
    }
};
int main(void){
    wiringPiSetupGpio();
    //Reifen R_Rechts(int);   
    //Reifen R_Links(int);

    Ultraschall Abstand_vorne_rechts(23, 24);
    //Ultraschall AS_vorne_links(int, int);
    //Ultraschall AS_hinten_rechts(int, int);
    //Ultraschall AS_hinten_links(int, int);
    while (1){
        double distanc = Abstand_vorne_rechts.get_distanz();
        printf("%d\n",distanc);
        delay(5000);
    }
    return 0;
}
