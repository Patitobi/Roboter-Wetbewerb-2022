#include <Arduino.h>
#include <IRremote.hpp>

IRrecv irrecv(7); //Empfänger Pin
decode_results results;
int BlauPin = 11;
int RotPin = 13;
int GruenPin = 12;

String hexvalue;

void GetIR(){ 
  //code der nicht unterbrochen werden darf
  if (irrecv.decode(&results)){ //Wenn irgendwas auf dem recvPin Gelesen wird dann 
      Serial.println(results.value, HEX); //Print in den Arduino Log (HEX)
      hexvalue = String(results.value);
      if(hexvalue == String(0x1240)){
        GotPinged();
      }else if(hexvalue == String(0xFF22DD)){
        GotPinged();
      }
      irrecv.resume(); //Reset + es wird wieder vom Pin auf Info gewartet.
  }
}

void GotPinged(){
  //delay(5000);//Zeit bevor Grün
  analogWrite(RotPin, 150); //Rot + Grün = Gelb
  analogWrite(GruenPin, 150);
  delay(5000);//Zeit bevor IR Signal
  SendIR(0x1101, 2);
  analogWrite(RotPin, 0);
  delay(5000);  
  analogWrite(GruenPin, 0);
  analogWrite(RotPin, 150); //wieder rot
}

void SendIR(long Code, int repeat){ //dir gibt an ob nach vorne oder nach hinten
  for(int i = 0; i != repeat; i++){
    IrSender.sendSony(Code, 20);
    delay(1000); //Muss min 5 millisek sonst erkennt der Empfänger das als ein einziges
  }
}

void setup() {
    Serial.begin(9600);
    irrecv.begin(7, ENABLE_LED_FEEDBACK);
    IrSender.begin(3);
    pinMode(BlauPin, OUTPUT); // b
    pinMode(RotPin, OUTPUT);//r
    pinMode(GruenPin, OUTPUT);//g
    analogWrite(RotPin, 50);
}

void update(){
  GetIR();
}

void loop() {
  update();
}
