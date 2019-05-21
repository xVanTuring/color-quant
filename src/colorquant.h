#ifndef COLOR_QUANT_H
#define COLOR_QUANT_H 1
#define DEFAULT_SIG_BITS 5
#define FRACT_BY_POPULATION 0.75
#define MAX_ITERS_ALLOWED 1000
#include "pix.h"
#include "vbox.h"

std::shared_ptr<int[]> pix_median_cut_histo(PIX *pix, int sigbits,
                                            int sub_sample);

void get_color_index(PIX *pix, int i, int rshift, int sigbits, int *index);
std::shared_ptr<Box3d> pix_get_color_region(PIX *pix, int sigbits,
                                            int sub_sample);

int median_cut_apply(std::shared_ptr<int[]> histo, int sigbits,
                     std::shared_ptr<Box3d> vbox,
                     std::shared_ptr<Box3d> &pvbox1,
                     std::shared_ptr<Box3d> &pvbox2);

PIXCMAP *pix_median_cut_quant(PIX *pix, int max_colors, int sigbits,
                              int max_sub);
PIXCMAP *histo_median_cut_quant(std::shared_ptr<int[]> histo, Box3d *vbox,
                                int max_colors, int sigbits);
#endif
