//some numeric tests of xlns16.cpp; see github.com/xlnsresearch/xlns/examples for a similar python test
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define xlns32_ideal
#include "xlns32.cpp"

//xlns32_aicasb needs to be defined for accurate 32-bit sums using xlns32_add_lpvip
#define xlns32_aicasb
#include "xlns32lpvip.cpp"

 #define xlns16_ideal
// #define xlns16_alt
// #define xlns16_altopt
 #define xlns16_table
#include "xlns16.cpp"
float rndfp[10000];
void initrndfp(float rndfp[],int limit)
{
        int i;
        for (i=0; i<limit; i++)
        {
           rndfp[i] = 4.0*((float) rand())/RAND_MAX - 1.0;
           //printf("%f\n",rndfp[i]);
        }
}
float frndtest1fp(int limit,float rndfp[])
{
	float sum;
	int i;

	sum = 0;
	for (i=1; i<=limit; i++)  //scaled down to 100
	{
		sum += rndfp[i];
	}
	//printf("rndtest1fp sum=%f\n", sum);
        return sum;
}

float frndtest1bf16(int limit,float rndfp[])
{
	float sum;
	int i;
        int bfval;

	sum = 0;
	for (i=1; i<=limit; i++)  //scaled down to 100
	{
                bfval = 0xFFFF0000&*((unsigned int *) &rndfp[i]);
		sum +=  *((float *) &bfval);
	}
	//printf("rndtest1fp sum=%f\n", sum);
        return sum;
}

float frndtest1xlns16(int limit, float rfp[])
{
	xlns16 sum;
	int i;

	sum = fp2xlns16(0.0);
	for (i=1; i<=limit; i++)  //scaled down to 100
	{
		sum = xlns16_add(sum,fp2xlns16(rndfp[i]));
	}
	//printf("rndtest1xlns16 sum=%f\n", xlns162fp(sum));
	return xlns162fp(sum);
}


float frndtest1xlns32(int limit, float rndfp[])
{
	xlns16 halfval;
        xlns32 sum;
	int i;

	sum = fp2xlns32(0.0);
	for (i=1; i<=limit; i++)  //scaled down to 100
	{
		halfval = fp2xlns16(rndfp[i]);
		sum = xlns32_add(sum,((xlns32)halfval)<<16);
	}
	//printf("rndtest1xlns16 sum=%f\n", xlns162fp(sum));
	return xlns322fp(sum);
}

float frndtest1lpvip32(int limit, float rndfp[])
{
	xlns16 halfval;
        xlns32 sum;
	int i;

	sum = fp2xlns32(0.0);
	for (i=1; i<=limit; i++)  //scaled down to 100
	{
		halfval = fp2xlns16(rndfp[i]);
		sum = xlns32_add_lpvip(sum,((xlns32)halfval)<<16);
	}
	//printf("rndtest1xlns16 sum=%f\n", xlns162fp(sum));
	return xlns322fp(sum);
}


int main()
{
      int limit;
      float rfp;
      float rlns,rerr,rlns32,rerr32,rlns32lpvip,rerr32lpvip;
      float rfp16,rerr16;
      printf("   n   fp32(exact)      bf16        rerr         xlns16     rerr       xlns32/16   rerr     lpvip32/xlns16 rerr\n");
      for (limit=1000; limit<=10000; limit+=500)
      {
        initrndfp(rndfp,limit);
	rfp = frndtest1fp(limit, rndfp);
	rlns =   frndtest1xlns16(limit, rndfp);
        rerr = fabs((rfp-rlns)/rfp);
	rfp16 = frndtest1bf16(limit, rndfp);
        rerr16 = fabs((rfp-rfp16)/rfp);
	rlns32 = frndtest1xlns32(limit, rndfp);
        rerr32 = fabs((rfp-rlns32)/rfp);
	rlns32lpvip = frndtest1lpvip32(limit, rndfp);
        rerr32lpvip = fabs((rfp-rlns32lpvip)/rfp);
        printf("%5i %12.6f   %12.6f %8.6f   %12.6f %8.6f  %12.6f %8.6f  %12.6f %8.6f\n",
              limit,rfp,rfp16,rerr16,rlns,rerr,rlns32,rerr32,rlns32lpvip,rerr32lpvip);
     }
     return 1;

}


