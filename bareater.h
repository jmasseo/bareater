#pragma once
#include <time.h>

int getminutemoduloindex(time_t timestamp[], int period, int barcount);

int crunch(time_t tin[], double oin[], double hin[], double lin[], double cin[], double vin[],
	time_t tout[], double oout[], double hout[], double lout[], double cout[], double vout[], int barsin, int startbar, int ratio);

