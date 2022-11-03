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
            digitalWrite(trigger, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigger, LOW);
            int time=0;

            while (digitalRead(echo)==0){
                time=0;
            }
            while (digitalRead(echo)==1){
                delayMicroseconds(1);
                time++;
                if (time>20000)return 0;
            }
            return ((time*343)*0.0001)/2;
        }
};
int main(void){
    wiringPiSetup();

    pinMode(4, 1);
    pinMode(5,0);
    int onOff;

    //Reifen R_Rechts(int);   
    //Reifen R_Links(int);

    //Ultraschall Abstand_vorne_rechts(4, 5);
    //Ultraschall AS_vorne_links(int, int);
    //Ultraschall AS_hinten_rechts(int, int);
    //Ultraschall AS_hinten_links(int, int);
    while (1){
        // delay(5000);
        // float distanc = Abstand_vorne_rechts.get_distanz();
        // printf("%d\n",distanc);

        digitalWrite(4,1);
        onOff = digitalRead(5);
        printf("%d",onOff);
        delay(500);
        digitalWrite(4,0);
        onOff = digitalRead(5);
        printf("%d",onOff);
        delay(500);
    }
    return 0;
}