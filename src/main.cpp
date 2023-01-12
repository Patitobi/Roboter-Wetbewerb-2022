<<<<<<< HEAD
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
=======
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
>>>>>>> 6f5105e9205dab421841d0833d582a0e853fb752
}