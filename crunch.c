#include "bareater.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
void usage()
{
	fprintf(stderr,"usage: crunch [-m] [-s symbol] [-b barstoread] [-r ratio]\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	int barstoread = 1000000;
	int ratio = 5;
	FILE * outfile = NULL;
	char *sym = NULL;
	int startonmodbar = 0;
 	int c;
	opterr = 0;
	  while ((c = getopt (argc, argv, "mb:r:s:")) != -1)
	    switch (c)
	      {
	      case 'r':
	        ratio = atoi(optarg);
	        break;
	      case 'b':
	        barstoread = atoi(optarg);
	        break;
	      case 's':
	        sym = optarg;
	        break;
	      case 'm':
		startonmodbar = 1;
		break;
	      case '?':
	        if (optopt == 'c')
	          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	        else if (isprint (optopt))
       		   fprintf (stderr, "Unknown option `-%c'.\n", optopt);
       		 else
       		   fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
	           usage();
		default:
	        abort ();
	      }
	if((argc - optind ) < 2) {
		fprintf(stderr,"argc %d optind %d\n", argc,optind);
		fprintf(stderr,"usage: %s [-m] [-s symbol] [-b barstoread] [-r ratio]\n",argv[0]);
		return 0;
	}

	char * sourcefilename = argv[optind];
	        if (access(sourcefilename,R_OK) != 0) {
			fprintf(stderr, "Could not access %s\n",argv[optind]);
			usage();
		}
	outfile = fopen(argv[optind+1],"w");
	        if (!outfile) {
			fprintf(stderr, "Could not access %s\n",argv[optind+1]);
			usage();
		}

	int barstowrite = (barstoread/ratio)+1;
	time_t *outt, *intime;
	double *outo, *outh, *outl, *outc, *outv;
	double *ino, *inh, *inl, *inc, *inv;
	outt = (time_t*)malloc(barstowrite*sizeof(time_t));
	if(outt == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	intime = (time_t*)malloc(barstoread*sizeof(time_t));
	if(intime == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	ino = (double*)malloc(barstoread*sizeof(double));
	if(ino == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	outo = (double*)malloc(barstowrite*sizeof(double));
	if(outo == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	inh = (double*)malloc(barstoread*sizeof(double));
	if(inh == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	outh = (double*)malloc(barstowrite*sizeof(double));
	if(outh == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	inl = (double*)malloc(barstoread*sizeof(double));
	if(inl == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	outl = (double*)malloc(barstowrite*sizeof(double));
	if(outl == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	inc = (double*)malloc(barstoread*sizeof(double));
	if(inc == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	outc = (double*)malloc(barstowrite*sizeof(double));
	if(outc == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	inv = (double*)malloc(barstoread*sizeof(double));
	if(inv == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}
	outv = (double*)malloc(barstowrite*sizeof(double));
	if(outv == NULL)
	{
		fprintf(stderr,"Malloc failed.\n");
		exit(-1);
	}

	int barsread = readfrom_TimeSymOHLCV_CSV(sourcefilename, barstoread,intime,ino,inh,inl,inc,inv);
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
	int startbar = 0;
	if(startonmodbar){
	startbar = getminutemoduloindex(intime, ratio, barsread);
	}
	int barscrunched = crunch(intime, ino, inh, inl, inc, inv, outt, outo, outh, outl, outc, outv, barsread, startbar, ratio);
	//i=0;
	/*
	while(i < barsread)
	{
        printf("[%d] IntT %d InO: %lf InH: %lf InL: %lf InC: %lf  InV: %lf\n", i, intime[i], ino[i], inh[i],inl[i], inc[i],inv[i]);
	i++;
	}
	*/
	i=0;
	if(sym == NULL)
	while(i < barscrunched)
	{
		fprintf(outfile,"%ld,%lf,%lf,%lf,%lf,%lf\n",outt[i],outo[i],outh[i],outl[i],outc[i],outv[i]);
		i++;
	}
	else 
	while(i < barscrunched)
	{
		fprintf(outfile,"%ld,%s,%lf,%lf,%lf,%lf,%lf\n",outt[i],sym,outo[i],outh[i],outl[i],outc[i],outv[i]);
		i++;
	}
	fclose(outfile);
	printf("Bars Read: %d Bars Crunched %d\n",barsread,barscrunched);
	return 0;
}
