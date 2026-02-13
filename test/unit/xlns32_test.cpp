//some numeric tests of xlns32.cpp; see github.com/xlnsresearch/xlns/examples for a similar python test
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//uncomment for ideal sb and db:
//#define xlns32_ideal
#include <xlns/xlns32.h>

void test1fp()
{
	float odd,sum;
	int i;

	odd = 1;
	sum = 0;
	for (i=1; i<=10000; i++)
	{
		sum += odd;
		odd += 2.0;
	}
	printf("test1fp odd=%f sum=%f\n", odd,sum);
}

void test1xlns32()
{
	xlns32 odd,sum,two;
	int i;

	odd = fp2xlns32(1.0);
	sum = fp2xlns32(0.0);
	two = fp2xlns32(2.0);
	for (i=1; i<=10000; i++)
	{
		sum = xlns32_add(sum,odd);
		odd = xlns32_add(odd,two);
	}
	printf("test1xlns32 odd=%f sum=%f\n", xlns322fp(odd),xlns322fp(sum));
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


void test2xlns32()
{
	xlns32 num,fact,sum,one;
	int i;

	one = fp2xlns32(1.0);
	num = one;
	fact = one;
	sum = fp2xlns32(0.0);
	for (i=1; i<=8; i++)
	{
		sum = xlns32_add(sum, xlns32_recip(fact));
		fact = xlns32_mul(fact, num);
		num = xlns32_add(num, one);
	}
	printf("test2xlns321 num=%f fact=%e sum=%f\n",
		 xlns322fp(num),xlns322fp(fact),xlns322fp(sum));
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



void test3xlns32()
{
	xlns32 num,fact,sum,one,two;
	int i;

	one = fp2xlns32(1.0);
	num = one;
	fact = one;
	sum = fp2xlns32(0.0);
	two = fp2xlns32(2.0);
	for (i=1; i<=10; i++)
	{
		sum = xlns32_add(sum, xlns32_recip(fact));
		fact = xlns32_neg(xlns32_mul(fact, xlns32_mul(num,xlns32_add(num,one))));
		num = xlns32_add(num, two);
	}
	printf("test3xlns321 num=%f fact= sum=%f\n",
		 xlns322fp(num),/*xlns322fp(fact),*/xlns322fp(sum));
}

/* compute pi the hard way*/

void test5xlns32_float()
{
	xlns32_float num,val,sum;
	long i;

	num = 1.0;
	sum = 0.0;
	val = num;
	for (i=1; i<=1000; i++)
	{
		sum = sum + val/num;
		val = -val;
		num = num + 2.0;
	}
	std::cout << "test5xlns32_float num=" << num << " 4*sum=" << 4*sum << "\n";
}

void test5fp()
{
	float num,val,sum;
	long i;

	num = 1.0;
	sum = 0.0;
	val = num;
	for (i=1; i<=1000; i++)
	{
		sum = sum + val/num;
		val = -val;
		num = num + 2.0;
	}
	printf("test5fp num=%f 4*sum=%f\n", num,4*sum);
}

void test5xlns32()
{
	xlns32 num,val,sum,two;
	long i;

	two = fp2xlns32(2.0);
	num = fp2xlns32(1.0);
	sum = fp2xlns32(0.0);
	val = num;
	for (i=1; i<=1000; i++)
	{
		sum = xlns32_add(sum, xlns32_div(val, num));
		val = xlns32_neg(val);
		num = xlns32_add(num, two);
	}
	printf("test5xlns32 num=%f val=%e 4*sum=%f\n",
		 xlns322fp(num),xlns322fp(val),xlns322fp(xlns32_mul(fp2xlns32(4.0),sum)));
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

/*Mandelbrot set xlns32*/


void test4xlns32(int iter)
{
	time_t t1,t2;
	char ch;
	int ix,iy,count;
	xlns32 x1,y1,x,y,xnew,ynew,mone,two,four,xscale,yscale,temp;

	mone = fp2xlns32(-1.0);
	two = fp2xlns32(2.0);
	four = fp2xlns32(4.0);
	yscale = fp2xlns32(12.0);
	xscale = fp2xlns32(24.0);

	time(&t1);
	for (iy = 11; iy >= -11; iy--)
	{
		for (ix=-40; ix <= 38; ix++)
		{
                        y = xlns32_div(fp2xlns32((float)iy), yscale);
                        x = xlns32_div(fp2xlns32((float)ix), xscale);
			x1 = x;
			y1 = y;
			count = 0;
			while (((xlns32_add(xlns32_mul(x,x),xlns32_mul(y,y))) < (four)) && (count<iter))
			{
				//xnew = xlns32_add(xlns32_sub(xlns32_mul(x,x),xlns32_mul(y,y)), x1);
				//ynew = xlns32_add(xlns32_mul(two, xlns32_mul(x,y)) ,y1);
                           temp = xlns32_neg(xlns32_mul(y,y));
                           xnew = xlns32_add(xlns32_add(
                                     xlns32_mul(x,x), temp ), x1);
                           temp = xlns32_mul(x,y);
                           ynew = xlns32_add(xlns32_mul(two, temp),y1);

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
	printf("test4xlns32 new %d sec ",((int)(t2-t1)));
	scanf("%c",&ch);
}

void test4xlns32_float(int iter)
{
	time_t t1,t2;
	char ch;
	int ix,iy,count;
	xlns32_float x1,y1,x,y,xnew,ynew,mone,two,four,xscale,yscale;

	xlns32_hits=0;
	xlns32_misses=0;
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
	printf("hits=%ld misses=%ld\n",xlns32_hits,xlns32_misses);
	printf("testxlns32_float new %d sec ",((int)(t2-t1)));
	scanf("%c",&ch);
}




void testops()
{
	xlns32 x,y;
	float fx,fy;

	printf("test ops xlns32: enter 0 0 to quit\n");
	do
	{
	scanf("%f %f",&fx,&fy);
	printf("fx=%e fy=%e\n",fx,fy);
	x=fp2xlns32(fx);
	y=fp2xlns32(fy);
	printf("x=%08x %e\n",x, xlns322fp(x));
	printf("y=%08x %e\n",y, xlns322fp(y));
	printf("-fx     =%+e  -x     =%+e   %08x  %d\n",
			-(fx),   xlns322fp(xlns32_neg(x)),xlns32_neg(x),fp2xlns32(-fx)-xlns32_neg(x));
	printf("sqrt(fx)=%+e  sqrt(x)=%+e   %08x  %d\n",
			sqrt(fx),xlns322fp(xlns32_sqrt(x)),xlns32_sqrt(x),fp2xlns32(sqrt(fx))-xlns32_sqrt(x));
	printf("abs(fx) =%+e  abs(x) =%+e   %08x  %d\n",
			fabs(fx),xlns322fp(xlns32_abs(x)),xlns32_abs(x),fp2xlns32(fabs(fx))-xlns32_abs(x));
	if (fx!=0.0)
		printf("1/fx    =%+e  1/x    =%+e   %08x  %d\n",
			1/fx,    xlns322fp(xlns32_recip(x)),xlns32_recip(x),fp2xlns32((float)(1.0/fx))-xlns32_recip(x));
	printf("fx+fy   =%+e  x+y    =%+e   %08x  %d\n",
			fx+fy,   xlns322fp(xlns32_add(x,y)),xlns32_add(x,y),fp2xlns32(fx+fy)-xlns32_add(x,y));
	printf("fx-fy   =%+e  x-y    =%+e   %08x  %d\n",
			fx-fy,   xlns322fp(xlns32_sub(x,y)),xlns32_sub(x,y),fp2xlns32(fx-fy)-xlns32_sub(x,y));
	printf("fx*fy   =%+e  x*y    =%+e   %08x  %d\n",
			fx*fy,   xlns322fp(xlns32_mul(x,y)),xlns32_mul(x,y),fp2xlns32(fx*fy)-xlns32_mul(x,y));
	if (fx!=0.0)
		printf("fx/fy   =%+e  x/y    =%+e   %08x  %d\n",
			fx/fy,   xlns322fp(xlns32_div(x,y)),xlns32_div(x,y),fp2xlns32((float)(fx/fy))-xlns32_div(x,y));

	}while(fx!=0.0);
}

void test1xlns32_float()
{
	xlns32_float odd,sum,two;
	int i;

	odd = 1.0;
	sum = 0.0;
	two = 2.0;
	for (i=1; i<=10000; i++)
	{
		sum = sum+odd;
		odd = odd+two;
	}
	printf("test1xlns32_float odd=%f sum=%f\n", xlns32_2float(odd),xlns32_2float(sum));
	std::cout << "test1xlns32_float cout odd=" << odd << " sum=" << sum << "\n";
}
void test2xlns32_float()
{
	xlns32_float num,fact,sum;
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
	std::cout << "test2xlns32_float num=" << num << " fact=" << fact << " sum=" << sum << "\n";
}


void test3xlns32_float()
{
	xlns32_float num,fact,sum;
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
	std::cout << "test3xlns32_float num=" << num << " fact=" << fact << "sum=" << sum << "\n";
}


void testcompare()
{
	xlns32_float x[4];
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
	printf("xlns32 C++ (32-bit like float) %ld\n",sizeof(xlns32));

	testcompare();
	test5fp();
	test5xlns32();
	test5xlns32_float();
	test1fp();
	test1xlns32();
	test1xlns32_float();
	test2fp();
	test2xlns32();
	test2xlns32_float();
	test3fp();
	test3xlns32();
	test3xlns32_float();

	scanf("%c",&ch);
	test4fp(2000);
	scanf("%c",&ch);
	test4xlns32(2000);
	scanf("%c",&ch);
	test4xlns32_float(2000);

	testops();

	return 1;

}


