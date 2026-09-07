#ifdef xlns32_arch16
  typedef unsigned long xlns32;
  typedef signed long xlns32_signed;
  #define xlns32_zero          0x00000L
  #define xlns32_scale         0x00800L
  #define xlns32_logmask       0x7ffffL
  #define xlns32_signmask      0x80000L
  #define xlns32_logsignmask   0x40000L
  #define xlns32_canonmask     0x80000L
  #define xlns32_sqrtmask      0x20000L
  #define xlns32_esszer        0x0cfa0L
  #define xlns32_canonshift    19

  #define xlns32_one          0x40000L
  #define xlns32_neg_one      0xC0000L
  #define xlns32_two          0x40800L
  #define xlns32_neg_two      0xC0800L
  #define xlns32_half         0x3F800L
  #define xlns32_neg_half     0xBF800L
  #define xlns32_pos_inf      0x7FFFFL
  #define xlns32_neg_inf      0xFFFFFL
#else
  typedef unsigned int xlns32;
  typedef signed int xlns32_signed;
  #define xlns32_zero          0x00000
  #define xlns32_scale         0x00800
  #define xlns32_logmask       0x7ffff
  #define xlns32_signmask      0x80000
  #define xlns32_logsignmask   0x40000
  #define xlns32_canonmask     0x80000
  #define xlns32_sqrtmask      0x20000
  #define xlns32_esszer        0x0cfa0
  #define xlns32_canonshift    19

  #define xlns32_one          0x40000
  #define xlns32_neg_one      0xC0000
  #define xlns32_two          0x40800
  #define xlns32_neg_two      0xC0800
  #define xlns32_half         0x3F800
  #define xlns32_neg_half     0xBF800
  #define xlns32_pos_inf      0x7FFFF
  #define xlns32_neg_inf      0xFFFFF

  #define xlns32_tablesize     (4096*3+1000)//??
  #define xlns32_zhmask        0x0fff00L
  #define xlns32_zlmask        0x000ffL
  #define xlns32_zhshift       8

  #ifdef xlns32_ideal
  #else
   #define xlns32_db0shift      14
   #define xlns32_db1shift      7
   #define xlns32_db0mask       0x0fc000L
   #define xlns32_db1mask       0x03f80L
   #define xlns32_db2mask       0x0007fL
   #define xlns32_db0size       128
   #define xlns32_db1size       128
   #define xlns32_db2size       64
  #endif
#endif
