<<<<<<< HEAD
#include "reifen.h"

void reifen_pinSetup()
{
	pinMode(L1, OUTPUT);
	pinMode(R1, OUTPUT);
}

void reifen(int seite, int mode)
{
	if (seite == 0)
	{
		if (mode == STOP)
		{
			digitalWrite(L1, LOW);
			digitalWrite(R1, LOW);
		}
		if (mode == VOR)
		{
			digitalWrite(L1, HIGH);
			digitalWrite(R1, HIGH);
		}
	}
=======
#include "reifen.h"

void reifen_pinSetup()
{
	pinMode(L1, OUTPUT);
	pinMode(R1, OUTPUT);
}

void reifen(int seite, int mode)
{
	if (seite == 0)
	{
		if (mode == STOP)
		{
			digitalWrite(L1, LOW);
			digitalWrite(R1, LOW);
		}
		if (mode == VOR)
		{
			digitalWrite(L1, HIGH);
			digitalWrite(R1, HIGH);
		}
	}
>>>>>>> 6f5105e9205dab421841d0833d582a0e853fb752
}