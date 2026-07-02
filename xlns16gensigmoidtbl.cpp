// Generate xlns16sigmoidtbl.h: tbl[i] = fp2xlns16(sigmoid(xlns162fp(i))).

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define xlns16_ideal
#define xlns16_alt
#include "xlns16.cpp"

static float ref_sigmoid(float x) {
	if (x <= -88.0f) return 0.0f;
	if (x >= 88.0f) return 1.0f;
	return 1.0f / (1.0f + exp(-x));
}

int main()
{
	FILE * f;
	int i;
	unsigned out;
	xlns16 code;
	float fx;

	f = fopen("xlns16sigmoidtbl.h", "w");
	if (!f) {
		perror("fopen xlns16sigmoidtbl.h");
		return 1;
	}

	fprintf(f, "xlns16 xlns16sigmoidtbl[65536] = {\n");
	for (i = 0; i < 65536; i++) {
		code = (xlns16)(unsigned short)i;
		fx = xlns162fp(code);
		out = (unsigned)fp2xlns16(ref_sigmoid(fx));
		if (i % 16 == 0 && i > 0) fprintf(f, ",\n");
		if (i % 16 > 0) fprintf(f, ",");
		fprintf(f, "%u", out);
	}
	fprintf(f, "\n};\n");
	fclose(f);
	return 0;
}
