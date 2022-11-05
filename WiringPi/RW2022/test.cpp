#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

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
    public:
        int starttime;
        int stoptime;
        int sendpin;
        int recievepin;
        int now;
        int timeout = 12000;
        int time;
        Ultraschall(int sendpin, int recievepin){
            this->sendpin=sendpin;
            this->recievepin=recievepin;
            pinMode(sendpin, OUTPUT);
            pinMode(recievepin, INPUT);
            digitalWrite(sendpin, LOW);
            delay(50);
        }
        float get_durschnitliche_distanz(int anz){
            float distance;
            float tempdist;
            for(int i=0; i<anz; i++){
                tempdist=get_distanz();
                if (tempdist<20){
                    i--;
                }   else if(i>3||distance-40<tempdist||distance+40>tempdist){
                    continue;
                } distance+=tempdist;
            }
            return distance/=anz;
        }
        float get_distanz(){
            digitalWrite(sendpin, 1);
            delayMicroseconds(10);
            digitalWrite(sendpin, 0);

            now = micros();
            //Warte auf rückkehr des signals
            while(digitalRead(recievepin == LOW) && micros()-now<timeout){
                time = recordpulselength();
                if (time<50){
                    continue;
                } else break;
            }
            float finDistance = ((time*343)/1000)/2;
        return finDistance;
    }
    int recordpulselength(){
        starttime = micros();
        while (digitalRead(recievepin) == HIGH){}
        stoptime = micros();
        return stoptime - starttime;
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
        printf("LoS!!!\n");
        //float distanc = Abstand_vorne_rechts.get_durschnitliche_distanz(50);
        float distanc = Abstand_vorne_rechts.get_distanz();
        printf("%f\n",distanc);
        delayMicroseconds(2000);
    }
    return 0;
}
