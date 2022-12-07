#include <Arduino.h>
#include <IRremote.hpp>

IRrecv irrecv(7); //Empfänger Pin
decode_results results;

String hexvalue;

void GetIR(){ 
  //code der nicht unterbrochen werden darf
  if (irrecv.decode(&results)){ //Wenn irgendwas auf dem recvPin Gelesen wird dann 
      Serial.println(results.value, HEX); //Print in den Arduino Log (HEX)
      hexvalue = String(results.value);
      if(hexvalue == String(0x1240)){
        GotPinged();
      }
      irrecv.resume(); //Reset + es wird wieder vom Pin auf Info gewartet.
  }
}

void GotPinged(){
  delay(10000);//Zeit bevor Grün
  digitalWrite(6, HIGH);
  SendIR(0x1101, 1);
  delay(500);
  digitalWrite(6, LOW);
}

void SendIR(long Code, int repeat){ //dir gibt an ob nach vorne oder nach hinten
  for(int i = 0; i != repeat; i++){
    IrSender.sendSony(Code, 20);
    delay(1000); //Muss min 5 millisek sonst erkennt der Empfänger das als ein einziges
  }
}

void setup() {
    Serial.begin(115200);
    irrecv.begin(7, ENABLE_LED_FEEDBACK);
    IrSender.begin(3);
    pinMode(6, OUTPUT);
}

void update(){
  GetIR();
}

void loop() {
  update();
}
