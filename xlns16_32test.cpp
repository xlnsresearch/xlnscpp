
//some numeric tests of xlns16.cpp vs xlns32lpvip
//xlns32_aicasb needs to be defined for accurate 32-bit sums
//xlns16_aicasb allows same hardware to compute 16-bit result
//this program proves this produces identical results
#define xlns16_aicasb
#define xlns32_aicasb

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define xlns32_ideal
#include "xlns32.cpp"
#include "xlns32lpvip.cpp"

#define xlns16_alt
#define xlns16_altopt
#include "xlns16.cpp"

void testlns16()
{
	xlns16 z,s,one;
	xlns16 s32;
        xlns32 one32;
	int i;
        one = fp2xlns16(1.0); 
        one32 = fp2xlns32(1.0); 
	for (z=0x4000-0x400; z<=0x4000; z++)  
	{
		s = xlns16_add(one,z);
                s32 = xlns32_add_lpvip(one32,z<<16)>>16;
                if (s != s32)
                {
	            printf("z=%0x %0x %0x\n",z, s, s32);
	            printf("z=%f %f %f\n", xlns162fp(z), xlns162fp(s), xlns162fp(s32));
                }

	}
}



int main()
{
     testlns16();
     return 1;

}


