#include "IR.h"

void SendIR(long Code, int repeat, int dir){ //dir gibt an ob nach vorne oder nach hinten
  if(dir == 1){ //Send to front
    for(int i = 0; i != repeat; i++){
      IrSender.sendSony(Code, 20);
      delay(1000); //Muss min 5 millisek sonst erkennt der Empfänger das als ein einziges
    }
  }else if(dir == 0){ //Send to back
    for(int i = 0; i != repeat; i++){
      IrSender.sendSony(Code, 20);
      delay(1000); //Muss min 5 millisek sonst erkennt der Empfänger das als ein einziges
    }
  }
}
void GetIR(){
  //code der nicht unterbrochen werden darf
  if (irrecv.decode(&results)){ //Wenn irgendwas auf dem recvPin Gelesen wird dann 
      Serial.println(results.value, HEX); //Print in den Arduino Log (HEX)
      hexvalue = String(results.value); //Hier kurz eine variable machen um nicht in allen if statements die funktion durch zu führen (performance)
      if(hexvalue == 0x1101){ //Grünes zeichen von Ampel
        AmpelPing(0x1101);
      }else if(hexvalue == 0x1210){ //Ampel Anfahrt 
        //Geb Signal nach hinten weiter
        SendIR(0x1210, 2, 0);
        AmpelAnfahrt(); //Fängt ann an den nächsten mann anzufahren (7cm) und wartet dann
      }
      irrecv.resume(); //Reset + es wird wieder vom Pin auf Info gewartet.
  }
}