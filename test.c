#include "bareater.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


int main(int argc, char *argv[])
{
	time_t outt[2000], intime[10000];
	double outo[2000], outh[2000], outl[2000], outc[2000], outv[2000];
	double ino[10000], inh[10000], inl[10000], inc[10000], inv[10000];
	printf("Test harness for bareater...\nThis reads a symbol file named test.csv.\n unixtimestamp,symbol,open,high,low,close,volume\n");
	int barsread = readfrom_TimeSymOHLCV_CSV("test.csv", 10000,intime,ino,inh,inl,inc,inv);
	int i =0;
	/*
	while(i < barsread)
	{
        printf("[%d] IntT %d InO: %lf InH: %lf InL: %lf InC: %lf  InV: %lf\n", i, intime[i], ino[i], inh[i],inl[i], inc[i],inv[i]);
	i++;
	}
	*/
	if(!barsread)
	{
		printf("No bars read!\n");
		exit(-1);
	}
	int startbar = getminutemoduloindex(intime, 5, barsread);
	int barscrunched = crunch(intime, ino, inh, inl, inc, inv, outt, outo, outh, outl, outc, outv, barsread, startbar, 5);
	//i=0;
	while(i < barsread)
	{
        printf("[%d] IntT %d InO: %lf InH: %lf InL: %lf InC: %lf  InV: %lf\n", i, intime[i], ino[i], inh[i],inl[i], inc[i],inv[i]);
	i++;
	}
	i=0;
	while(i < barscrunched)
	{
        printf("[%d] OutT %d OutO: %lf OutH: %lf OutL: %lf OutC: %lf  OutV: %lf\n", i, outt[i], outo[i], outh[i],outl[i], outc[i],outv[i]);
	i++;
	}
        printf("Before exit: BR: %d BC: %d IntT[0] %d InO[0]: %lf Outt[0]: %d  OutO[0]: %lf OutC: %lf\n", barsread, barscrunched, intime[0], ino[0], outt[0],outo[0], outc[0]);
        printf("IntT[1] %d InO[1]: %lf Outt[1]: %d  OutO[1]: %lf OutC: %lf\n",  intime[1], ino[1], outt[1],outo[1], outc[1]);
	return 0;
}

int readfrom_TimeSymOHLCV_CSV(char *fname, int barstoscan, 
	time_t tin[], double oout[], double hout[], double lout[], double cout[], double vout[])
{
	FILE *f;
	int rv;
	int barsread = 0;
	f = fopen(fname, "r");
	if (f == NULL) return 0;
	char scrapstring[512]; // yes i know
	for (barsread = 0; barsread < barstoscan && !feof(f);)
	{
		rv = fscanf(f, "%ld,%[^,],%lf,%lf,%lf,%lf,%lf\n", &tin[barsread], &scrapstring,
			&oout[barsread], &hout[barsread], &lout[barsread], &cout[barsread], &vout[barsread]);
		if (rv != 7) { return 0; }
		barsread++;
	}
	fclose(f);
	return barsread;
}
