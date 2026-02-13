#if xlns16case == 0
 #define xlns16_whatcase "default\n"
#endif

#if xlns16case == 1
 #define xlns16_ideal
 #define xlns16_whatcase "ideal\n"
#endif

#if xlns16case == 2
 #define xlns16_alt
 #define xlns16_ideal
 #define xlns16_whatcase "alt ideal\n"
#endif

#if xlns16case == 3
 #define xlns16_alt
 #define xlns16_whatcase "alt\n"
#endif

#if xlns16case == 4
 #define xlns16_alt
 #define xlns16_table
 #define xlns16_whatcase "alt table\n"
#endif

#if xlns16case == 5
 #define xlns16_alt
 #define xlns16_altopt
 #define xlns16_whatcase "alt altopt\n"
#endif

