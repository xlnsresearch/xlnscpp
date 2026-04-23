//Monte-Carlo LNS (MCLNS) is given in doi:10.1049/iet-cta.2010.0736 (and some earlier papers cited there).
//The original MCLNS and its most efficient hardware only needs the noise at the end.
//Here, xlns32_add_lpvip(x+noise, y+noise) is the same as (max(x,y)+noise) +sb(-|x-y|) is mostly 
//software implementation trick that is roughly equivalent using only a few lines of code. 
//The trick is identical noise added to both x and y makes |x+noise-(y+noise)| cancels out noise.
//The main purpose of MCLNS here is to sum a large sequence of xlns16 values (as in an LLM)
//without having to keep an xlns32 accumulator.  This is most successful the result is not near zero;
//it is less effective when the sum is near zero (catastrophic cancelation)

//MCLNS does not require quality "randomness"; the default is a max len linear feedback shift register
//alternatively, if 1 < xlns16_num_rand < 15 is defined, this code uses rand()
//  small xlns16_num_rand give poor results;  larger values are limited to lpvip accuracy

#ifdef xlns16_num_rand
        // if so, use that # bits from rand() 
#else   // otherwise need global for lin feedback shift reg 
  xlns16 xlns16_randombits= 7;  //any seed in 1 <= seed <= 65535;
#endif

xlns16 xlns16_add_monte(xlns16 x, xlns16 y)
{
    #ifdef xlns16_num_rand     // use specified bits from rand() 
        xlns16 xlns16_randombits;
        xlns16_randombits = (rand()%(1<<xlns16_num_rand))<<(16-xlns16_num_rand);
    #else                     // use max len linear feedback shift reg
        xlns16 bit = xlns16_randombits & 1;
        xlns16_randombits >>= 1;
        if (bit) {
            xlns16_randombits ^= 0xB400u; // Mask for taps 16, 15, 13, 4
        }
    #endif
    return xlns32_add_lpvip( (((xlns32)x)<<16)|xlns16_randombits, (((xlns32)y)<<16)|xlns16_randombits)>>16;
}
