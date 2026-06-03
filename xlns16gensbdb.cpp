#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define xlns16_ideal
//#define xlns16_alt
#include "xlns16.cpp"

int main()
{
	FILE * f;
	xlns16_signed x;
	f = fopen("xlns16sbdbtbl.h","w");
	fprintf(f,"xlns16_signed xlns16sbtbl[%d] = {\n",xlns16_esszer);
	for (x=xlns16_logsignmask; x>xlns16_logsignmask-xlns16_esszer; x--)
		fprintf(f,"%d, //%d\n",(xlns16_signed) xlns16_add(xlns16_logsignmask, x)-xlns16_logsignmask,xlns16_logsignmask-x);
	fprintf(f,"};\n");
	fprintf(f,"xlns16_signed xlns16dbtbl[%d] = {\n",xlns16_esszer);
	for (x=xlns16_logsignmask; x>xlns16_logsignmask-xlns16_esszer; x--)
		fprintf(f,"%d, //%d\n",(xlns16_signed) xlns16_add(xlns16_signmask|xlns16_logsignmask, x)+xlns16_logsignmask,xlns16_logsignmask-x);
	fprintf(f,"};\n");
	fclose(f);
	return 1;
}
