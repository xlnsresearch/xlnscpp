//some numeric tests of xlns16.cpp; see github.com/xlnsresearch/xlns/examples for a similar python test
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef xlns16case
 #include "xlns16_testcase.h"
#else
 //#define xlns16_ideal
 #define xlns16_alt
 #define xlns16_altopt
 //#define xlns16_table
#endif
  

#include <xlns/xlns16.h>



void test1fp()
{
	float odd,sum;
	int i;

	odd = 1;
	sum = 0;
	for (i=1; i<=100; i++)  //scaled down to 100
	{
		sum += odd;
		odd += 2.0;
	}
	printf("test1fp odd=%f sum=%f\n", odd,sum);
}

void test1xlns16()
{
	xlns16 odd,sum,two;
	int i;

	odd = fp2xlns16(1.0);
	sum = fp2xlns16(0.0);
	two = fp2xlns16(2.0);
	for (i=1; i<=100; i++)  //scaled down to 100
	{
		sum = xlns16_add(sum,odd);
		odd = xlns16_add(odd,two);
	}
	printf("test1xlns16 odd=%f sum=%f\n", xlns162fp(odd),xlns162fp(sum));
}



void test2fp()
{
	float num,fact,sum;
	int i;

	num = 1.0;
	fact = 1.0;
	sum = 0.0;
	for (i=1; i<=8; i++)
	{
		sum = sum + 1.0/fact;
		fact = fact * num;
		num = num + 1.0;
	}
	printf("test2fp1 num=%f fact=%e sum=%f\n", num,fact,sum);
}


void test2xlns16()
{
	xlns16 num,fact,sum,one;
	int i;

	one = fp2xlns16(1.0);
	num = one;
	fact = one;
	sum = fp2xlns16(0.0);
	for (i=1; i<=8; i++)
	{
		sum = xlns16_add(sum, xlns16_recip(fact));
		fact = xlns16_mul(fact, num);
		num = xlns16_add(num, one);
	}
	printf("test2xlns161 num=%f fact=%e sum=%f\n",
		 xlns162fp(num),xlns162fp(fact),xlns162fp(sum));
}




void test3fp()
{
	double num,fact,sum;
	int i;

	num = 1.0;
	fact = 1.0;
	sum = 0.0;
	for (i=1; i<=10; i++)
	{
		sum = sum + 1.0/fact;
		fact = -fact * num * (num + 1.0);
		num = num + 2.0;
	}
	printf("test3fp1 num=%f fact=%e sum=%f\n", num,fact,sum);
}



void test3xlns16()
{
	xlns16 num,fact,sum,one,two;
	int i;

	one = fp2xlns16(1.0);
	num = one;
	fact = one;
	sum = fp2xlns16(0.0);
	two = fp2xlns16(2.0);
	for (i=1; i<=10; i++)
	{
		sum = xlns16_add(sum, xlns16_recip(fact));
		fact = xlns16_neg(xlns16_mul(fact, xlns16_mul(num,xlns16_add(num,one))));
		num = xlns16_add(num, two);
	}
	printf("test3xlns161 num=%f fact= sum=%f\n",
		 xlns162fp(num),/*xlns162fp(fact),*/xlns162fp(sum));
}

/* compute pi the hard way*/

void test5xlns16_float()
{
	xlns16_float num,val,sum;
	long i;

	num = 1.0;
	sum = 0.0;
	val = num;
	for (i=1; i<=10/*00*/; i++)
	{
		sum = sum + val/num;
		val = -val;
		//std::cout << "test5xlns16_float num=" << num << " 4*sum=" << 4*sum << "\n";
		num = num + 2.0;
	}
	std::cout << "test5xlns16_float num=" << num << " 4*sum=" << 4*sum << "\n";
}

void test5fp()
{
	float num,val,sum;
	long i;

	num = 1.0;
	sum = 0.0;
	val = num;
	for (i=1; i<=10/*00*/; i++)
	{
		sum = sum + val/num;
		val = -val;
		num = num + 2.0;
	}
	printf("test5fp num=%f 4*sum=%f\n", num,4*sum);
}

void test5xlns16()
{
	xlns16 num,val,sum,two;
	long i;

	two = fp2xlns16(2.0);
	num = fp2xlns16(1.0);
	sum = fp2xlns16(0.0);
	val = num;
	for (i=1; i<=10/*00*/; i++)
	{
		sum = xlns16_add(sum, xlns16_div(val, num));
		val = xlns16_neg(val);
		num = xlns16_add(num, two);
	}
	printf("test5xlns16 num=%f val=%e 4*sum=%f\n",
		 xlns162fp(num),xlns162fp(val),xlns162fp(xlns16_mul(fp2xlns16(4.0),sum)));
}

#include <time.h>

/*Mandelbrot set fpI*/

void test4fp(int iter)
{
	time_t t1,t2;
	char ch;
	int ix,iy,count;
	float x1,y1,x,y,xnew,ynew,mone,two,four,xscale,yscale;

	mone = -1.0;
	two = 2.0;
	four = 4.0;
	yscale = 12.0;
	xscale = 24.0;
	time(&t1);
	for (iy = 11; iy >= -11; iy--)
	{
		for (ix=-40; ix <= 38; ix++)
		{
			y = iy/yscale;
			x = ix/xscale;
			x1 = x;
			y1 = y;
			count = 0;
			while ((x*x+y*y < four) && (count<iter))
			{
				xnew = x*x - y*y + x1;
				ynew = x*y*two + y1;
				count++;
				x = xnew;
				y = ynew;
			}
			if (count < iter)
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}
	time(&t2);
	printf("test4fp %d sec ",((int)(t2-t1)));
	scanf("%c",&ch);
}

/*Mandelbrot set xlns16*/


void test4xlns16(int iter)
{
	time_t t1,t2;
	char ch;
	int ix,iy,count;
	xlns16 x1,y1,x,y,xnew,ynew,mone,two,four,xscale,yscale,temp;

	mone = fp2xlns16(-1.0);
	two = fp2xlns16(2.0);
	four = fp2xlns16(4.0);
	yscale = fp2xlns16(12.0);
	xscale = fp2xlns16(24.0);

	time(&t1);
	for (iy = 11; iy >= -11; iy--)
	{
		for (ix=-40; ix <= 38; ix++)
		{
                        y = xlns16_div(fp2xlns16((float)iy), yscale);
                        x = xlns16_div(fp2xlns16((float)ix), xscale);
			x1 = x;
			y1 = y;
			count = 0;
			while (((xlns16_add(xlns16_mul(x,x),xlns16_mul(y,y))) < (four)) && (count<iter))
			{
				//xnew = xlns16_add(xlns16_sub(xlns16_mul(x,x),xlns16_mul(y,y)), x1);
				//ynew = xlns16_add(xlns16_mul(two, xlns16_mul(x,y)) ,y1);
                           temp = xlns16_neg(xlns16_mul(y,y));
                           xnew = xlns16_add(xlns16_add(
                                     xlns16_mul(x,x), temp ), x1);
                           temp = xlns16_mul(x,y);
                           ynew = xlns16_add(xlns16_mul(two, temp),y1);

			   count++;
			   x = xnew;
			   y = ynew;
			}
			if (count < iter)
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}
	time(&t2);
	printf("test4xlns16 new %d sec ",((int)(t2-t1)));
	scanf("%c",&ch);
}

void test4xlns16_float(int iter)
{
	time_t t1,t2;
	char ch;
	int ix,iy,count;
	xlns16_float x1,y1,x,y,xnew,ynew,mone,two,four,xscale,yscale;

	xlns16_hits=0;
	xlns16_misses=0;
	mone = -1.0;
	two = 2.0;
	four = 4.0;
	yscale = 12.0;
	xscale = 24.0;
	time(&t1);
	for (iy = 11; iy >= -11; iy--)
	{
		for (ix=-40; ix <= 38; ix++)
		{
			y1 = iy/yscale;
			x1= ix/xscale;
			x = x1;
			y = y1;
			count = 0;
			while ((x*x+y*y < 4.0) && (count<iter))
			{
				xnew = x*x - y*y + x1;
				ynew = x*y*2.0 + y1;
				count++;
				x = xnew;
				y = ynew;
			}
			if (count < iter)
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}
	time(&t2);
	printf("hits=%ld misses=%ld\n",xlns16_hits,xlns16_misses);
	printf("testxlns16_float new %d sec ",((int)(t2-t1)));
	scanf("%c",&ch);
}




void testops()
{
	xlns16 x,y;
	float fx,fy;

	printf("test ops xlns16:  enter 0 0 to quit\n");
	do
	{
	scanf("%f %f",&fx,&fy);
	printf("fx=%e fy=%e\n",fx,fy);
	x=fp2xlns16(fx);
	y=fp2xlns16(fy);
	printf("x=%04x %e\n",x, xlns162fp(x));
	printf("y=%04x %e\n",y, xlns162fp(y));
	printf("-fx     =%+e  -x     =%+e   %04x  %d\n",
			-(fx),   xlns162fp(xlns16_neg(x)),xlns16_neg(x),fp2xlns16(-fx)-xlns16_neg(x));
	printf("sqrt(fx)=%+e  sqrt(x)=%+e   %04x  %d\n",
			sqrt(fx),xlns162fp(xlns16_sqrt(x)),xlns16_sqrt(x),fp2xlns16(sqrt(fx))-xlns16_sqrt(x));
	printf("abs(fx) =%+e  abs(x) =%+e   %04x  %d\n",
			fabs(fx),xlns162fp(xlns16_abs(x)),xlns16_abs(x),fp2xlns16(fabs(fx))-xlns16_abs(x));
	if (fx!=0.0)
		printf("1/fx    =%+e  1/x    =%+e   %04x  %d\n",
			1/fx,    xlns162fp(xlns16_recip(x)),xlns16_recip(x),fp2xlns16((float)(1.0/fx))-xlns16_recip(x));
	printf("fx+fy   =%+e  x+y    =%+e   %04x  %d\n",
			fx+fy,   xlns162fp(xlns16_add(x,y)),xlns16_add(x,y),fp2xlns16(fx+fy)-xlns16_add(x,y));
	printf("fx-fy   =%+e  x-y    =%+e   %04x  %d\n",
			fx-fy,   xlns162fp(xlns16_sub(x,y)),xlns16_sub(x,y),fp2xlns16(fx-fy)-xlns16_sub(x,y));
	printf("fx*fy   =%+e  x*y    =%+e   %04x  %d\n",
			fx*fy,   xlns162fp(xlns16_mul(x,y)),xlns16_mul(x,y),fp2xlns16(fx*fy)-xlns16_mul(x,y));
	if (fx!=0.0)
		printf("fx/fy   =%+e  x/y    =%+e   %04x  %d\n",
			fx/fy,   xlns162fp(xlns16_div(x,y)),xlns16_div(x,y),fp2xlns16((float)(fx/fy))-xlns16_div(x,y));
	}while(fx!=0.0);
}


void test1xlns16_float()
{
	xlns16_float odd,sum,two;
	int i;

	odd = 1.0;
	sum = 0.0;
	two = 2.0;
	for (i=1; i<=100; i++)  //scaled down to 100
	{
		sum = sum+odd;
		odd = odd+two;
	}
	printf("test1xlns16_float odd=%f sum=%f\n", xlns16_2float(odd),xlns16_2float(sum));
	std::cout << "test1xlns16_float cout odd=" << odd << " sum=" << sum << "\n";
}
void test2xlns16_float()
{
	xlns16_float num,fact,sum;
	int i;

	num = 1.0;
	fact = 1.0;
	sum = 0.0;
	for (i=1; i<=8; i++)
	{
		sum = sum + 1.0/fact;
		fact = fact * num;
		num = num + 1.0;
	}
	std::cout << "test2xlns16_float num=" << num << " fact=" << fact << " sum=" << sum << "\n";
}


void test3xlns16_float()
{
	xlns16_float num,fact,sum;
	int i;

	num = 1.0;
	fact = 1.0;
	sum = 0.0;
	for (i=1; i<=10; i++)
	{
		sum = sum + 1.0/fact;
		fact = -fact * num * (num + 1.0);
		num = num + 2.0;
	}
	std::cout << "test3xlns16_float num=" << num << " fact=" << fact << "sum=" << sum << "\n";
}


void testcompare()
{
	xlns16_float x[4];
	float f[4];
	int i,j;
	f[0] = -2.0;
	f[1] = -0.5;
	f[2] = 0.5;
	f[3] = 2.0;
	x[0] = -2.0;
	x[1] = -0.5;
	x[2] = 0.5;
	x[3] = 2.0;

	for(i=0; i<=3; i++)
	{
	  for (j=0; j<=3; j++)
	    {
	      printf("%d ",f[i]<f[j]);
	    }
	    printf("    ");
	  for (j=0; j<=3; j++)
	    {
	      printf("%d ",x[i]<x[j]);
	    }
	    printf("\n");
	}
}


int main(void)
{
	char ch;
	printf("xlns16 C++ (16-bit like bfloat) %ld\n",sizeof(xlns16));
	#ifdef xlns16case
           printf("xlns16_float: %s",xlns16_whatcase);
    	#endif
	testcompare();
	test5fp();
	test5xlns16();
	test5xlns16_float();
	test1fp();
	test1xlns16();
	test1xlns16_float();
	test2fp();
	test2xlns16();
	test2xlns16_float();
	test3fp();
	test3xlns16();
	test3xlns16_float();

	#ifdef xlns16case
	   	exit(0);
	#endif

	scanf("%c",&ch);
	test4fp(2000);
	scanf("%c",&ch);
	test4xlns16(2000);
	scanf("%c",&ch);
	test4xlns16_float(2000);

	testops();

	return 1;

}


