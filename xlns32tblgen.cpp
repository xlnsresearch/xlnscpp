//xlns32tblgen.cpp -- table generation
//uncomment one of the following:
//#include "xlns32F23.h"
//#include "xlns32F15.h"
#include "xlns32F13.h"
//#include "xlns32F11.h"
//#include "xlns32F9.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

inline xlns32 xlns32_sb_ideal(xlns32_signed z)
  {
	return ((xlns32) ((log(1+ pow(2.0, ((double) z) / xlns32_scale) )/log(2.0))*xlns32_scale+.5));
  }

inline xlns32 xlns32_db_ideal(xlns32_signed z)
  {
	return ((xlns32_signed) ((log( pow(2.0, ((double) z) / xlns32_scale) - 1 )/log(2.0))*xlns32_scale+.5));
  }

int main()
{
  FILE * f;
  int i;
  xlns32_signed xi;
  xlns32 sxi,dxi;

  f = fopen(xlns32_filename,"w");
  fprintf(f,"unsigned /*short*/ xlns32_sbltable[xlns32_tablesize] = {\n");
  xi = 0;
  for (i=0; i<xlns32_tablesize-1; i++)
  {
     sxi = xlns32_sb_ideal(xi); 
     fprintf(f,"   0x%04x,\n",sxi&0xFFFF);
     xi -= (xlns32_zlmask+1);
  }
  fprintf(f,"   0x0000\n");
  fprintf(f,"};\n");
  
  fprintf(f,"unsigned char xlns32_sbhtable[xlns32_tablesize] = {\n");
  xi = 0;
  for (i=0; i<xlns32_tablesize-1; i++)
  {
     sxi = xlns32_sb_ideal(xi); 
     fprintf(f,"   0x%02x,\n",sxi>>16);
     xi -= (xlns32_zlmask+1);
  }
  fprintf(f,"   0x00\n");
  fprintf(f,"};\n");

  fprintf(f,"xlns32 xlns32_db0table[xlns32_db0size] = {0,\n");
  xi = 0;
  for (i=0; i<xlns32_db0size-2; i++)
  {
     xi += xlns32_db1mask + xlns32_db2mask + 1; 
     dxi = xlns32_db_ideal(xi); 
     fprintf(f,"   0x%08x,\n",dxi);
  }
  xi += xlns32_db1mask + xlns32_db2mask + 1; 
  dxi = xlns32_db_ideal(xi); 
  fprintf(f,"   0x%08x\n",dxi);
  fprintf(f,"};\n");

  fprintf(f,"xlns32 xlns32_db1table[xlns32_db1size] = {0,\n");
  xi = 0;
  for (i=0; i<xlns32_db1size-2; i++)
  {
     xi += xlns32_db2mask + 1;
     dxi = xlns32_db_ideal(xi); 
     fprintf(f,"   0x%08x,\n",dxi);
  }
  xi += xlns32_db2mask + 1;
  dxi = xlns32_db_ideal(xi); 
  fprintf(f,"   0x%08x\n",dxi);
  fprintf(f,"};\n");

  fprintf(f,"xlns32 xlns32_db2table[xlns32_db2size] = {0,\n");
  xi = 0;
  for (i=0; i<xlns32_db2size-2; i++)
  {
     xi += 1;
     dxi = xlns32_db_ideal(xi); 
     fprintf(f,"   0x%08x,\n",dxi);
  }
  xi += 1;
  dxi = xlns32_db_ideal(xi); 
  fprintf(f,"   0x%08x\n",dxi);
  fprintf(f,"};\n");

  fclose(f);
  return 0;
}

