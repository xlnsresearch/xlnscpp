//exhaustive test converting fp to xlns32
//check edge cases near subnormal, inf

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//uncomment for ideal sb and db:
#define xlns32_ideal
#include "xlns32.cpp"

//#define xlns32_pos_inf 0x7FFFFFFF
//#define xlns32_neg_inf 0xFFFFFFFF

//duplicate of fp2xlns32 (for debugging)
xlns32 myfp2xlns32(double x)
{
/*
	if ((x>=0.0)&&(x<2.93873587706e-39))
		return(xlns32_zero);
	else if ((x<=0.0)&&(x>-2.93873587706e-39))
		return(xlns32_zero);
*/
	if ((x>-2.938747e-39)&&(x<2.938747e-39))
		return(xlns32_zero);
        else if (x> 3.40282286e+38)
		return(xlns32_pos_inf);
        else if (x< -3.40282286e+38)
		return(xlns32_neg_inf);
	else if (x > 0.0)
		return xlns32_abs((xlns32_signed) ((log(x)/log(2.0))*xlns32_scale))
		       ^xlns32_logsignmask;
	else
		return (((xlns32_signed) ((log(fabs(x))/log(2.0))*xlns32_scale))
			  |xlns32_signmask)^xlns32_logsignmask;
}


void testcvt(double x)
{
        printf("%e converts to %08x and back to %e\n",x,fp2xlns32(x),xlns322fp(fp2xlns32(x)));
}

void test_pos_norm_floats()
{
       float x, eps;
       int ix;
       xlns32 lx;
       bool ok;

       eps = 1e-6;
       ok = 0;
       for (ix = 0x00800000; ix<=0x7F800000; ix++)
       {  if ((ix & 0x007FFFFF)==0) 
          { if (ok) 
              printf(" ok");
            ok = 1;
            printf("\nnormal %08x",ix);
          }
          x = * ((float *) (&ix));
          lx = fp2xlns32(x);
          if (fabs(x-xlns322fp(lx))/x>eps)
          { ok=0;
            printf("%08x %e %e rerr=%e\n",ix,x,xlns322fp(lx),fabs(x-xlns322fp(lx))/x);
          }
       }
       if (ok) 
         printf(" ok\n\n");
}



void test_neg_norm_floats()
{
       float x, eps;
       int ix;
       xlns32 lx;
       bool ok;

       eps = 1e-6;
       ok = 0;
       for (ix = 0x80800000; ix<=0xFF800000; ix++)
       {  if ((ix & 0x007FFFFF)==0) 
          { if (ok) 
              printf(" ok");
            ok = 1;
            printf("\nnormal %08x",ix);
          }
          x = * ((float *) (&ix));
          lx = fp2xlns32(x);
          if (fabs(x-xlns322fp(lx))/x>eps)
          { ok=0;
            printf("%08x %e %e rerr=%e\n",ix,x,xlns322fp(lx),fabs(x-xlns322fp(lx))/x);
          }
       }
       if (ok) 
         printf(" ok\n\n");
}

void test_pos_subnorm_floats()
{
       float x, eps;
       int ix;
       xlns32 lx;

       eps = 1e-5;
       printf("pos subnormals\n");
       for (ix =0x00000000; ix<=0x00800000; ix++)
       {  
          x = * ((float *) (&ix));
          lx = fp2xlns32(x);
          if (fabs(x-xlns322fp(lx))>eps)
            printf("%08x %e  l=%08x %e aerr=%e\n",ix,x,lx,xlns322fp(lx),fabs(x-xlns322fp(lx)));
       }
}


void test_neg_subnorm_floats()
{
       float x, eps;
       int ix;
       xlns32 lx;

       eps = 1e-5;
       printf("neg subnormals\n");
       for (ix =0x80000000; ix<=0x80800000; ix++)
       {  
          x = * ((float *) (&ix));
          lx = fp2xlns32(x);
          if (fabs(x-xlns322fp(lx))>eps)
            printf("%08x %e  l=%08x %e aerr=%e\n",ix,x,lx,xlns322fp(lx),fabs(x-xlns322fp(lx)));
       }
}

int main(void)
{
        test_neg_subnorm_floats();
        test_pos_subnorm_floats();
        test_pos_norm_floats();
        test_neg_norm_floats();
        testcvt(-1.0/0.0);
        testcvt(-1e100);
        testcvt(-3.40283e+38);
        testcvt(-3.40282e+38);
        testcvt(-3.402812e+38);
        testcvt(-3.402811e+38);
        testcvt(-3.4e+38);
        testcvt(-2.0);
        testcvt(-1.0);
        testcvt(-0.5);
        testcvt(-3.0e-39); 
        testcvt(-2.9388e-39); 
        testcvt(-2.938740e-39);
        testcvt(-2.938739e-39);
        testcvt(-2.938738e-39);
        testcvt(-2.938737e-39);
        testcvt(-2.938736e-39);
        testcvt(-2.938735877e-39); 
        testcvt(-2.9e-39); 
        testcvt(-1e-100);
        testcvt(-0.0);
        testcvt(0.0);
        testcvt(1e-100);
        testcvt(2.9e-39); 
        testcvt(2.938735877e-39);
        testcvt(2.938736e-39);
        testcvt(2.938737e-39);
        testcvt(2.938738e-39);
        testcvt(2.938739e-39);
        testcvt(2.938740e-39);
        testcvt(2.938741e-39);
        testcvt(2.938742e-39);
        testcvt(2.938743e-39);
        testcvt(2.938744e-39);
        testcvt(2.938745e-39);
        testcvt(2.938746e-39);
        testcvt(2.938747e-39);
        testcvt(2.9387471e-39);
        testcvt(2.9387472e-39);
        testcvt(2.9387473e-39);
        testcvt(2.9387474e-39);
        testcvt(2.9387475e-39);
        testcvt(2.9387476e-39);
        testcvt(2.9387477e-39);
        testcvt(2.9387478e-39);
        testcvt(2.9387479e-39);
        testcvt(2.938748e-39);
        testcvt(2.938750e-39);
        testcvt(2.938760e-39);
        testcvt(2.938770e-39);
        testcvt(2.938780e-39);
        testcvt(2.938790e-39);
        testcvt(2.9388e-39); 
        testcvt(3.0e-39); 
        testcvt(0.5);
        testcvt(1.0);
        testcvt(2.0);
        testcvt(3.4e+38);
        testcvt(3.402811e+38);
        testcvt(3.402812e+38);
        testcvt(3.40282e+38);
        testcvt(3.40283e+38);
        testcvt(1e100);
        testcvt(1.0/0.0);

	return 1;

}


