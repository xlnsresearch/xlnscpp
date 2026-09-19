#ifdef xlns32_arch16
  typedef unsigned long xlns32;
  typedef signed long xlns32_signed;
  #define xlns32_zero          0x000000L
  #define xlns32_scale         0x002000L
  #define xlns32_logmask       0x1fffffL
  #define xlns32_signmask      0x200000L
  #define xlns32_logsignmask   0x100000L
  #define xlns32_canonmask     0x200000L
  #define xlns32_sqrtmask      0x080000L
  #define xlns32_esszer        0x03fa00L
  #define xlns32_canonshift    21

  #define xlns32_one          0x100000L
  #define xlns32_neg_one      0x300000L
  #define xlns32_two          0x102000L
  #define xlns32_neg_two      0x302000L
  #define xlns32_half         0x0FE000L
  #define xlns32_neg_half     0x2FE000L
  #define xlns32_pos_inf      0x1FFFFFL
  #define xlns32_neg_inf      0x3FFFFFL

  #ifdef xlns32_ideal
  #else
   #define xlns32_filename      "xlns32tblF13.h"
   #define xlns32_tablesize     512//(4096*3+1000)//??
   #define xlns32_zhmask        0x03fe00L
   #define xlns32_zlmask        0x0001ffL
   #define xlns32_zhshift       9

   #define xlns32_db0shift      18
   #define xlns32_db1shift      9
   #define xlns32_db0mask       0x040000L
   #define xlns32_db1mask       0x03fe00L
   #define xlns32_db2mask       0x0001ffL
   #define xlns32_db0size       2 
   #define xlns32_db1size       512 
   #define xlns32_db2size       512
  #endif
#else
  typedef unsigned int xlns32;
  typedef signed int xlns32_signed;
  #define xlns32_zero          0x000000
  #define xlns32_scale         0x002000
  #define xlns32_logmask       0x1fffff
  #define xlns32_signmask      0x200000
  #define xlns32_logsignmask   0x100000
  #define xlns32_canonmask     0x200000
  #define xlns32_sqrtmask      0x200000
  #define xlns32_esszer        0x03fa00
  #define xlns32_canonshift    21

  #define xlns32_one          0x100000
  #define xlns32_neg_one      0x300000
  #define xlns32_two          0x102000
  #define xlns32_neg_two      0x302000
  #define xlns32_half         0x0FE000
  #define xlns32_neg_half     0x2FE000
  #define xlns32_pos_inf      0x1FFFFF
  #define xlns32_neg_inf      0x3FFFFF

  #ifdef xlns32_ideal
  #else
   #define xlns32_filename      "xlns32tblF13.h"
   #define xlns32_tablesize     512//(4096*3+1000)//??
   #define xlns32_zhmask        0x03fe00
   #define xlns32_zlmask        0x0001ff
   #define xlns32_zhshift       9

   #define xlns32_db0shift      18
   #define xlns32_db1shift      9
   #define xlns32_db0mask       0x040000
   #define xlns32_db1mask       0x03fe00
   #define xlns32_db2mask       0x0001ff
   #define xlns32_db0size       2 
   #define xlns32_db1size       512 
   #define xlns32_db2size       512
  #endif
#endif
