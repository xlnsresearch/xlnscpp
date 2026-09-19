#ifdef xlns32_arch16
  typedef unsigned long xlns32;
  typedef signed long xlns32_signed;
  #define xlns32_zero          0x000000L
  #define xlns32_scale         0x008000L
  #define xlns32_logmask       0x7fffffL
  #define xlns32_signmask      0x800000L
  #define xlns32_logsignmask   0x400000L
  #define xlns32_canonmask     0x800000L
  #define xlns32_sqrtmask      0x200000L
  #define xlns32_esszer        0x0ffa00L
  #define xlns32_canonshift   23 

  #define xlns32_one          0x400000L
  #define xlns32_neg_one      0xC00000L
  #define xlns32_two          0x408000L
  #define xlns32_neg_two      0xC08000L
  #define xlns32_half         0x3F8000L
  #define xlns32_neg_half     0xBF8000L
  #define xlns32_pos_inf      0x7FFFFFL
  #define xlns32_neg_inf      0xFFFFFFL

  #ifdef xlns32_ideal
  #else
   #define xlns32_filename      "xlns32tblF15.h"
   #define xlns32_tablesize     1024 //(4096*3+1000)//??
   #define xlns32_zhmask        0x0ffc00L
   #define xlns32_zlmask        0x0003ffL
   #define xlns32_zhshift       10

   #define xlns32_db0shift      20
   #define xlns32_db1shift      10
   #define xlns32_db0mask       0x100000L
   #define xlns32_db1mask       0x0ffc00L
   #define xlns32_db2mask       0x0003ffL
   #define xlns32_db0size       2 
   #define xlns32_db1size       1024
   #define xlns32_db2size       1024
  #endif
#else
  typedef unsigned int xlns32;
  typedef signed int xlns32_signed;
  #define xlns32_zero          0x000000
  #define xlns32_scale         0x008000
  #define xlns32_logmask       0x7fffff
  #define xlns32_signmask      0x800000
  #define xlns32_logsignmask   0x400000
  #define xlns32_canonmask     0x800000
  #define xlns32_sqrtmask      0x200000
  #define xlns32_esszer        0x0ffa00
  #define xlns32_canonshift    23

  #define xlns32_one          0x400000
  #define xlns32_neg_one      0xC00000
  #define xlns32_two          0x408000
  #define xlns32_neg_two      0xC08000
  #define xlns32_half         0x3F8000
  #define xlns32_neg_half     0xBF8000
  #define xlns32_pos_inf      0x7FFFFF
  #define xlns32_neg_inf      0xFFFFFF

  #ifdef xlns32_ideal
  #else
   #define xlns32_filename      "xlns32tblF15.h"
   #define xlns32_tablesize     1024 //(4096*3+1000)//??
   #define xlns32_zhmask        0x0ffc00
   #define xlns32_zlmask        0x0003ff
   #define xlns32_zhshift       10

   #define xlns32_db0shift      20
   #define xlns32_db1shift      10
   #define xlns32_db0mask       0x100000
   #define xlns32_db1mask       0x0ffc00
   #define xlns32_db2mask       0x0003ff
   #define xlns32_db0size       2 
   #define xlns32_db1size       1024
   #define xlns32_db2size       1024
  #endif
#endif
