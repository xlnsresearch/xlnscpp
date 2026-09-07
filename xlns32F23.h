#ifdef xlns32_arch16
  typedef unsigned long xlns32;
  typedef signed long xlns32_signed;
  #define xlns32_zero          0x00000000L
  #define xlns32_scale         0x00800000L
  #define xlns32_logmask       0x7fffffffL
  #define xlns32_signmask      0x80000000L
  #define xlns32_logsignmask   0x40000000L
  #define xlns32_canonmask     0x80000000L
  #define xlns32_sqrtmask      0x20000000L
  #define xlns32_esszer        0x0cfa0000L
  #define xlns32_canonshift    31

  #define xlns32_one          0x40000000L
  #define xlns32_neg_one      0xC0000000L
  #define xlns32_two          0x40800000L
  #define xlns32_neg_two      0xC0800000L
  #define xlns32_half         0x3F800000L
  #define xlns32_neg_half     0xBF800000L
  #define xlns32_pos_inf      0x7FFFFFFFL
  #define xlns32_neg_inf      0xFFFFFFFFL

  #ifdef xlns32_ideal
  #else
    #define xlns32_filename      "xlns32tbl.h"
    #define xlns32_tablesize     (4096*3+1000)
    #define xlns32_zhmask        0x0fffc000L
    #define xlns32_zlmask        0x00003fffL
    #define xlns32_zhshift       14

    #define xlns32_db0shift      19
    #define xlns32_db1shift      10
    #define xlns32_db0mask       0x0ff80000L
    #define xlns32_db1mask       0x0007fc00L
    #define xlns32_db2mask       0x000003ffL
    #define xlns32_db0size       512
    #define xlns32_db1size       512
    #define xlns32_db2size       1024
  #endif
#else
  typedef unsigned int xlns32;
  typedef signed int xlns32_signed;
  #define xlns32_zero          0x00000000
  #define xlns32_scale         0x00800000
  #define xlns32_logmask       0x7fffffff
  #define xlns32_signmask      0x80000000
  #define xlns32_logsignmask   0x40000000
  #define xlns32_canonmask     0x80000000
  #define xlns32_sqrtmask      0x20000000
  #define xlns32_esszer        0x0cfa0000
  #define xlns32_canonshift    31

  #define xlns32_one          0x40000000
  #define xlns32_neg_one      0xC0000000
  #define xlns32_two          0x40800000
  #define xlns32_neg_two      0xC0800000
  #define xlns32_half         0x3F800000
  #define xlns32_neg_half     0xBF800000
  #define xlns32_pos_inf      0x7FFFFFFF
  #define xlns32_neg_inf      0xFFFFFFFF

  #ifdef xlns32_ideal
  #else
    #define xlns32_filename      "xlns32tbl.h"
    #define xlns32_tablesize     (4096*3+1000)
    #define xlns32_zhmask        0x0fffc000
    #define xlns32_zlmask        0x00003fff
    #define xlns32_zhshift       14

    #define xlns32_db0shift      19
    #define xlns32_db1shift      10
    #define xlns32_db0mask       0x0ff80000
    #define xlns32_db1mask       0x0007fc00
    #define xlns32_db2mask       0x000003ff
    #define xlns32_db0size       512
    #define xlns32_db1size       512
    #define xlns32_db2size       1024
  #endif
#endif

