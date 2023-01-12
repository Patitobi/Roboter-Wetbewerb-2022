#include <Arduino.h>
#include "USS.h"
#include "sync.h"
#include "sync.h"
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
}

void loop()
{
	// update();
	// machen();
}