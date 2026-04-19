xlns16 xlns16_add_monte(xlns16 x, xlns16 y)
{
        xlns16 randombits;
        randombits = (rand()%0x4000)<<2;
	return xlns32_add_lpvip( (((xlns32)x)<<16)|randombits, (((xlns32)y)<<16)|randombits)>>16;
}
