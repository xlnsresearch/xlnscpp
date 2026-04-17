#define xlns32_F 23
#ifdef xlns32_db_ideal
#else
  inline xlns32 xlns32_db_ideal(xlns32_signed z)  //only for singularity
  {
	return ((xlns32_signed) ((log( pow(2.0, ((double) z) / xlns32_scale) - 1 )/log(2.0))*xlns32_scale+.5));
  }
#endif
  inline xlns32 xlns32_mitch(xlns32 z)
  {
     return (((1<<xlns32_F)+(z&((1<<xlns32_F)-1)))>>(-(z>>xlns32_F)));
  }

//need to define xlns32_aicasb if xlns32_add_lpvip is to give accurate summation

inline xlns32 xlns32_add_lpvip(xlns32 x, xlns32 y)
{
    xlns32 minxyl, maxxy, xl, yl, usedb, adjust, adjustez;
    xlns32_signed z;
    xl = x & xlns32_logmask;
    yl = y & xlns32_logmask;
    minxyl = (yl>xl) ? xl : yl;
    maxxy  = (xl>yl) ? x  : y;
    z = minxyl - (maxxy&xlns32_logmask);
    usedb = xlns32_signmask&(x^y); 
       xlns32_signed precond = (usedb==0) ? (
                                            #ifdef xlns32_aicasb
                                              (z > -(7<<(xlns32_F-1))) ? ((-z)>>3) : (7<<(xlns32_F-4)) 
                                            #else
                                              ((-z)>>3)
                                            #endif
                                            ) :          // -.125*z 
                (z < -(2<<xlns32_F)) ? 5<<(xlns32_F-3):        //  0.625
                                (z >> 2) + (9 << (xlns32_F-3));//  .25*z + 9/8
   //printf("%i %08x %08x\n",usedb,z,precond);
       xlns32_signed postcond = (z <= -(3<<xlns32_F)) ? 0: 
                            z >= -(3<<(xlns32_F-2)) ? -(1<<(xlns32_F-6)) : 
                                                      +(1<<(xlns32_F-6));
       xlns32_signed mitch = (-z >= 1<<xlns32_F)||(usedb==0) ? xlns32_mitch(z+precond) : 
                                          -xlns32_db_ideal(-z)-z; // use ideal for singularity
       adjust = usedb ? -mitch : (z==0) ? 1<<(xlns32_F) : mitch + postcond;
      adjustez = (z < -xlns32_esszer) ? 0 : adjust; 
    return ((z==0) && usedb) ? 
                     xlns32_zero :
                     xlns32_mul(maxxy, xlns32_logsignmask + adjustez);
}
