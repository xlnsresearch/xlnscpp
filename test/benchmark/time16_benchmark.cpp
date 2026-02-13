#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef xlns16case
 #include "xlns16_testcase.h"
#else
 //#define xlns16_ideal
 #define xlns16_alt
 //#define xlns16_altopt
 #define xlns16_table
#endif
#include <xlns/xlns16.h>
  
#include "math.h"
#include <time.h>

/*
Run with xlns16_ideal and xlns16_alt without xlns16_table on macbook ubuntu on parallels:
converting to xlns_float
time=27
converting back to float
time=25
converting to xlns
time=14
summing xlns
time=45
summing xlns_float
time=53
summing float
time=3

Run xlns16_alt (not xlns16_altopt) without xlns16_ideal, xlns16_table:
converting to xlns_float
time=26
converting back to float
time=25
converting to xlns
time=14
summing xlns
time=19
summing xlns_float
time=26
summing float
time=3



Run with xlns16_table, xlns16_alt (both with and without xlns16_altopt) and xlns16_ideal:
converting to xlns_float
time=6
converting back to float
time=4
converting to xlns
time=3
summing xlns
time=44
summing xlns_float
time=51
summing float
time=3

Run with xlns16_ideal and xlns16_table without xlns16_alt:
converting to xlns_float
time=7
converting back to float
time=4
converting to xlns
time=3
summing xlns
time=33
summing xlns_float
time=36
summing float
time=3

Run with xlns16_table and xlns16_alt (with and without altopt) but without xlns16_ideal (all tables):
converting to xlns_float
time=6
converting back to float
time=4
converting to xlns
time=3
summing xlns
time=14
summing xlns_float
time=21
summing float
time=3

Above are time in sec for 1000000000 ops; can view as avg time in ns for one op
FP_sum takes 3ns/elem; FP->LNS_sum->FP takes 6+14+3=23ns/elem, about 8x slower with tables,
Fast enough to test LLMs; Much worse 27+45+14=86ns/elem without tables, about 30x slower 

*/

int main()
{
	float          f[1000000];
	xlns16_float  xf[1000000];
	xlns16      xfxf[1000000];
	xlns16       xsum;
	xlns16_float xfsum;
	float        fsum;
	time_t t1,t2;

	int i,cnt;

        #ifdef xlns16case
         printf(xlns16_whatcase);
        #endif
	for (i=0; i<1000000; i++)
		f[i] = exp(-i/10000.)*sin(i);
	printf("converting to xlns_float\n");
	time(&t1);
	for (cnt=0; cnt<1000; cnt++)
	for (i=0; i<1000000; i++)
		xf[i] = float2xlns16_(f[i]);
	time(&t2);
	printf("time=%ld\n",t2-t1);
	printf("converting back to float\n");
	time(&t1);
	for (cnt=0; cnt<1000; cnt++)
	for (i=0; i<1000000; i++)
		f[i] = xlns16_2float(xf[i]);
	time(&t2);
	printf("time=%ld\n",t2-t1);
	printf("converting to xlns\n");
	time(&t1);
	for (cnt=0; cnt<1000; cnt++)
	for (i=0; i<1000000; i++)
		xfxf[i] = fp2xlns16(f[i]);
	time(&t2);
	printf("time=%ld\n",t2-t1);
	printf("summing xlns\n");
	time(&t1);
	xsum = xlns16_zero;
	for (cnt=0; cnt<1000; cnt++)
	for (i=0; i<1000000; i++)
		xsum = xlns16_add(xsum,xfxf[i]);
	time(&t2);
	printf("time=%ld\n",t2-t1);
	printf("summing xlns_float\n");
	time(&t1);
	xfsum = 0.0;
	for (cnt=0; cnt<1000; cnt++)
	for (i=0; i<1000000; i++)
		xfsum += xf[i];
	time(&t2);
	printf("time=%ld\n",t2-t1);
	printf("summing float\n");
	time(&t1);
	fsum = 0.0;
	for (cnt=0; cnt<1000; cnt++)
	for (i=0; i<1000000; i++)
		fsum += f[i];
	time(&t2);
	printf("time=%ld\n",t2-t1);

	printf("mul xlns\n");
	time(&t1);
	xsum = fp2xlns16(3.14159);
	for (cnt=0; cnt<1000; cnt++)
	for (i=0; i<1000000; i++)
		xfxf[i] = xlns16_mul(xsum,xfxf[i]);
	time(&t2);
	printf("time=%ld\n",t2-t1);
	printf("mul float\n");
	time(&t1);
	fsum = 3.14159;
	for (cnt=0; cnt<1000; cnt++)
	for (i=0; i<1000000; i++)
		f[i] = fsum * f[i];
	time(&t2);
	printf("time=%ld\n",t2-t1);
	return 0;
}


