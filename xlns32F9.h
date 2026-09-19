#ifdef xlns32_arch16
  typedef unsigned long xlns32;
  typedef signed long xlns32_signed;
  #define xlns32_zero          0x00000L
  #define xlns32_scale         0x00200L
  #define xlns32_logmask       0x1ffffL
  #define xlns32_signmask      0x20000L
  #define xlns32_logsignmask   0x10000L
  #define xlns32_canonmask     0x20000L
  #define xlns32_sqrtmask      0x08000L
  #define xlns32_esszer        0x03fa0L
  #define xlns32_canonshift    17

  #define xlns32_one          0x10000L
  #define xlns32_neg_one      0x30000L
  #define xlns32_two          0x10200L
  #define xlns32_neg_two      0x30200L
  #define xlns32_half         0x0FE00L
  #define xlns32_neg_half     0x2FE00L
  #define xlns32_pos_inf      0x1FFFFL
  #define xlns32_neg_inf      0x3FFFFL

  #ifdef xlns32_ideal
  #else
   #define xlns32_filename      "xlns32tblF9.h"
   #define xlns32_tablesize     128//(4096*3+1000)//??
   #define xlns32_zhmask        0x03f80L
   #define xlns32_zlmask        0x0007fL
   #define xlns32_zhshift       7

   #define xlns32_db0shift      14
   #define xlns32_db1shift      7
   #define xlns32_db0mask       0x04000L
   #define xlns32_db1mask       0x03f80L
   #define xlns32_db2mask       0x0007fL
   #define xlns32_db0size       2 
   #define xlns32_db1size       128 
   #define xlns32_db2size       128
  #endif
#else
  typedef unsigned int xlns32;
  typedef signed int xlns32_signed;
  #define xlns32_zero          0x00000
  #define xlns32_scale         0x00200
  #define xlns32_logmask       0x1ffff
  #define xlns32_signmask      0x20000
  #define xlns32_logsignmask   0x10000
  #define xlns32_canonmask     0x20000
  #define xlns32_sqrtmask      0x20000
  #define xlns32_esszer        0x03fa0
  #define xlns32_canonshift    17

  #define xlns32_one          0x10000
  #define xlns32_neg_one      0x30000
  #define xlns32_two          0x10200
  #define xlns32_neg_two      0x30200
  #define xlns32_half         0x0FE00
  #define xlns32_neg_half     0x2FE00
  #define xlns32_pos_inf      0x1FFFF
  #define xlns32_neg_inf      0x3FFFF

  #ifdef xlns32_ideal
  #else
   #define xlns32_filename      "xlns32tblF9.h"
   #define xlns32_tablesize     128//(4096*3+1000)//??
   #define xlns32_zhmask        0x03f80
   #define xlns32_zlmask        0x0007f
   #define xlns32_zhshift       7

   #define xlns32_db0shift      14
   #define xlns32_db1shift      7
   #define xlns32_db0mask       0x04000
   #define xlns32_db1mask       0x03f80
   #define xlns32_db2mask       0x0007f
   #define xlns32_db0size       2 
   #define xlns32_db1size       128 
   #define xlns32_db2size       128
  #endif
#endif
