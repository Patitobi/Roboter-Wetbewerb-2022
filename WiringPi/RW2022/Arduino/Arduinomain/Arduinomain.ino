#include <IRremote.hpp>
//#include <IRremote.h> //wenn #include <IRremote.hpp> nicht funktioniert

const int RECV_PIN = 7;
const String hexcode1 = "0x1111";
const String hexcode2 = "0x1112";
const String hexcode3 = "0x1113";

IRrecv irrecv(RECV_PIN); //Empfänger Pin
decode_results results; //erstelle Object in welches dann die Daten nach jedem scan rein wandern

void setup(){
  //Recieve IR
  irrecv.enableIRIn(); //Fang an zu Lesen
  irrecv.blink13(true); //Default Pin auf dem Board welcher blinkt wenn Pin was ließt
  //Send IR
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  IrSender.begin(3);
  attachInterrupt(digitalPinToInterrupt(2), CodetobeexecutedonInterrupt, CHANGE); //wenn sich pin 3 ändert dann führe interruptcode aus
}

void loop(){
  noInterrupts();
  //code der nicht unterbrochen werden darf
  if (irrecv.decode(&results)){ //Wenn irgendwas auf dem recvPin Gelesen wird dann 
      Serial.println(results.value, HEX); //Print in den Arduino Log (HEX)
      String hexvalue = String(results.value, HEX); //Hier kurz eine variable machen um nicht in allen if statements die funktion durch zu führen (performance)
      if(hexvalue == hexcode1){ //Code 1
        //mach was lustiges
      }else if(hexvalue == hexcode1){ //Code 2
        //mach was lustiges
      }else if(hexvalue == hexcode1){ //Code 3
        //mach was lustiges
      }
      irrecv.resume(); //Reset + es wird wieder vom Pin auf Info gewartet.
  }
  interrupts();
  //code der unterbrochen werden darf
  SendIRCode(0x42069);
}

void SendIRCode(long Code){
  IrSender.sendSony(Code, 20);
  delay(1000);
}

void CodetobeexecutedonInterrupt(){
  
}