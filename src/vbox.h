#ifndef VBOX_3D_H
#define VBOX_3D_H
#include<stddef.h>
struct Box3d {
	size_t sort_param; /* parameter on which to sort the vbox */
	size_t n_pix; /* number of pixels in the vbox        */
	size_t vol; /* quantized volume of vbox            */
	int r1; /* min r index in the vbox             */
	int r2; /* max r index in the vbox             */
	int g1; /* min g index in the vbox             */
	int g2; /* max g index in the vbox             */
	int b1; /* min b index in the vbox             */
	int b2; /* max b index in the vbox             */
};

typedef struct Box3d BOX3D;
#ifdef __cplusplus
extern "C" {
#endif
extern BOX3D* box_3d_create(int r1, int r2, int g1,
                            int g2, int b1, int b2);

extern BOX3D* box_3d_copy(BOX3D* vbox);

extern size_t vbox_get_count(BOX3D* vbox, int* histo, int sigbits);

extern size_t vbox_get_volume(BOX3D* vbox);
extern void vbox_get_average_color(BOX3D* vbox, int* histo,
                                   int sigbits, int index,
                                   int* p_rval, int* p_gval, int* p_bval);
#ifdef __cplusplus
}
#endif
#endif
