#include <Arduino.h>
#include "USS.h"
#include "IR.h"
#include "farbSens.h"
#include "reifen.h"

void update()
{
	updateSensors();
	// updatecolcor();
}

void machen()
{
	// USScheck();
}

void setup()
{
	reifen_pinSetup();
	farbSens_pinSetup();
	UUS_pinSetup();
	
	// als lestztes wegen den sync
	IR_pinSetup();
}

void loop()
{
	// update();
	// machen();
}