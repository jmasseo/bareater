#include "bareater.h"
#include <malloc.h>
/* getmoduloindex takes an array of timestamp pointers .. this sucks hold on */
int getminutemoduloindex(time_t timestamp[], int period, int barcount)
{
#ifdef _WIN32
	errno_t result;
#else
	struct tm *result;
#endif
	struct tm rval;
	int i;
	/*rval = (struct tm*)malloc(sizeof(struct tm));
	if (rval == NULL) return -2;*/
	for (i = 0; i < barcount; i++)
	{
#ifdef _WIN32
		result = gmtime_s(rval, &timestamp[i]);
		if (result) return -3;
#else
		time_t myt = timestamp[i];
		//printf(" [%d] ts[i]: %d &ts[i]: %d myt: %d &myt: %d\n",i,timestamp[i], &timestamp[i], myt, &myt);
		result = gmtime_r(&myt, &rval);
		//printf(" result: %d rval: %d\n", result, &rval);
		if (!result) return -3;
#endif
		//printf("DEBUG: %d %d %d - %d : %d mod period %d\n",result->tm_year,result->tm_mon,result->tm_mday, result->tm_hour, result->tm_min, period);
		if (result->tm_min % period) continue;
		//printf("DEBUG: returning idx %d tm_min %d mod period %d\n",i, result->tm_min, period);
		return i;
	}
	return -1;
}
// crunch takes a bunch of args and returns the number of bars it crunched
int crunch(time_t tin[], double oin[], double hin[], double lin[], double cin[], double vin[], time_t tout[], double oout[], double hout[], double lout[], double cout[], double vout[], int barsin, int startbar, int ratio)
{
	int barsout = 0;
	int i;
	time_t tt;
	double ot, ht, lt, ct, vt;
	//int startbar = getminutemoduloindex(tin, ratio, barsin);
		//printf("DEBUG: After the getminutemodule\n");
	if (startbar < 0) return 0;
	for (i = startbar; i < barsin; )
	{
		//printf("DEBUG: Inside loop %d\n",i);
		tt = tin[i];
		ot = oin[i];
		ht = hin[i];
		lt = lin[i];
		ct = cin[i];
		vt = vin[i];
		int j;
		for (j = 1; j < ratio && j + i < barsin; j++)
		{
			int X = j + i;
			if (hin[X] > ht) ht = hin[X];
			if (lin[X] < lt) lt = lin[X];
			ct = cin[X];
			vt += vin[X];
		}
		i += j;
		//printf("Before first assignment\n");
		tout[barsout] = tt;
		oout[barsout] = ot;
		hout[barsout] = ht;
		lout[barsout] = lt;
		cout[barsout] = ct;
		vout[barsout] = vt;
		//printf("After all assignments\n");
		//printf("BO: %d TT %d OT %lf HT %lf LT %lf CT %lf VT %lf\n",barsout,tt,ot,ht,lt,ct,vt);
		barsout++;
		
		}
	return barsout;
}
