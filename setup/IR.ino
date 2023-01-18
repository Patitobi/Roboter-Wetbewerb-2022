#include <IRremote.hpp>

void IR_pinSetup()
{
    hexvalue = "0";
    // Recieve IR
    irrecv.begin(7, ENABLE_LED_FEEDBACK); // Fang an zu Lesen
    // Send IR
    pinMode(LED_BUILTIN, OUTPUT);
    IrSender.begin(3);
    // attachInterrupt(digitalPinToInterrupt(3), CodetoBeExecutedOnInterrupt, CHANGE); //wenn sich pin 3 ändert dann führe interruptcode aus
    Serial.begin(115200);
    // Start sync with other cars
    //reifen(0, STOP);
    Serial.println("Getting Index...");
    GetmyIndex();
}

void SendIR(long Code, int repeat, int dir)
{ // dir gibt an ob nach vorne oder nach hinten
    if (dir == 1)
    { // Send to front
        for (int i = 0; i != repeat; i++)
        {
            IrSender.sendSony(Code, 20);
            delay(600); // Muss min 5 millisek sonst erkennt der Empfänger das als ein einziges
        }
    }
    else if (dir == 0)
    { // Send to back
        for (int i = 0; i != repeat; i++)
        {
            IrSender.sendSony(Code, 20);
            delay(600); // Muss min 5 millisek sonst erkennt der Empfänger das als ein einziges
        }
    }
}
void GetIR(){
    if (irrecv.decode(&results)){           // Wenn irgendwas auf dem recvPin Gelesen wird dann
        Serial.println(results.value, HEX); // Print in den Arduino Log (HEX)
        hexvalue = String(results.value);   // Hier kurz eine variable machen um nicht in allen if statements die funktion durch zu führen (performance)
        irrecv.resume(); // Reset + es wird wieder vom Pin auf Info gewartet.
    }
}
void AmpelPing(long Code){
    // Wenn dieses Auto nicht letzter in der Reihe dann reiche die Info nach hinten weiter.
    if (NuminReihe != 5){
        // Pass IR Signal to the Cars behind
        SendIR(Code, 3, 0); // Send 3x hinter dich (0 =  nach hinten)
    }
}
void RedLineReached(){ // Muss von Farbsensor gecallt werden und kann auch nur von index 1 gecallt werden
    // Ping hinter dich das die zu dir bis auf eine bestimmte distanz auffahren sollen und dann auch stehen bleiben.
    // Dann Ping die Ampel an das die anfangen soll ihr Programm abzurfen -> (Ampel wartet 8 Sekunden und gibt dann grünes Signal via IR)
    SendIR(0x1210, 2, 0); // Stehen bleiben 2x nach hinten
    if (NuminReihe == 1) SendIR(0x1240, 2, 1); //Vorderes Auto sendet Signal zur ampel damit Ampel anfängt zu agieren
    // Warte nun auf Ampel Signal und gebe wenn  Ampel Signal da das Signal an die hinteren weiter
    while (hexvalue != String(0x1101)){
        GetIR();
    }
}
void WaitforStart()
{
    while (!synced){
        // Warte Aufs Go von der Fernbedienung und wenn Signal 0x1211 kommt Fahre los
        GetIR();
        if (hexvalue == String(0xFF30CF)){
            synced = true;
        }
    }
}
void GetmyIndex(){
    while (!synced){
        GetIR();
        if (hexvalue == String(0xFF30CF)){
            NuminReihe = 1;
            WaitforStart();
        }
        else if (hexvalue == String(/*Fernbedienung Index 2 Code*/)){
            NuminReihe = 2;
            WaitforStart();
        }
        else if (hexvalue == String(/*Fernbedienung Index 3 Code*/)){
            NuminReihe = 3;
            WaitforStart();
        }
        else if (hexvalue == String(/*Fernbedienung Index 4 Code*/)){
            NuminReihe = 4;
            WaitforStart();
        }
    }
}