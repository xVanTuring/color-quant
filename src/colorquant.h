#ifndef COLOR_QUANT_H
#define COLOR_QUANT_H
#define DEFAULT_SIG_BITS 5
#define FRACT_BY_POPULATION 0.75
#define MAX_ITERS_ALLOWED 1000
#include "vbox.h"
#include "pix.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern int* pix_median_cut_histo(PIX* pix, int sigbits, int sub_sample);

	extern void get_color_index(PIX* pix, int i, int rshift, int sigbits, int* index);
	extern BOX3D* pix_get_color_region(PIX* pix, int sigbits, int sub_sample);

	extern int median_cut_apply(int* histo,
		int sigbits,
		BOX3D* vbox,
		BOX3D** pvbox1,
		BOX3D** pvbox2);

	extern PIXCMAP* pix_median_cut_quant(PIX* pix, int max_colors, int sigbits, int max_sub, int* counter);
	extern PIXCMAP* histo_median_cut_quant(int* histo, BOX3D* vbox, int max_colors, int sigbits, int* count);
#ifdef __cplusplus
}
#endif
#endif
