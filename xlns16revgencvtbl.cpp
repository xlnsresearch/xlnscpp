
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define xlns16_ideal
#define xlns16_alt
#include "xlns16.cpp"

int main()
{
	unsigned fi;
	float fl;
	xlns16 xf;
	FILE * f;

	int n = 17;
	f = fopen("xlns16revcvtbl.h","w");
	fprintf(f,"xlns16 xlns16revcvtbl[%d] = {\n",1<<n);
	for (fi=0; fi!=-1<<(32-n); fi+=1<<(32-n))
	{
		fl = *(float *)&fi;		
		xf = fp2xlns16(fl);
		fprintf(f,"%u, // %04x\n", xf,fi>>(32-n));
		//printf("%u %u=%e %e\n",fi,xf,xlns162fp(xf),fl);
	}
	fprintf(f,"};\n");
	fclose(f);
	return 1;
}
