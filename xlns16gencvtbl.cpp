#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define xlns16_ideal
//#define xlns16_alt
#include "xlns16.cpp"

int main()
{
	FILE * f;
	long x;
	f = fopen("xlns16cvtbl.h","w");
	fprintf(f,"float xlns16cvtbl[65536] = {\n");
	for (x=0; x<65536; x++)
		fprintf(f,"%e,\n",xlns162fp((xlns16) x));
	fprintf(f,"};\n");
	fclose(f);
	return 1;
}
