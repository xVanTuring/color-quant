#ifndef PIX_H
#define PIX_H
#include "heap.h"

struct RGBA_Quad {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

typedef struct RGBA_Quad RGBA_QUAD;

struct RGB_Quad {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

typedef struct RGB_Quad RGB_QUAD;

//struct Pix32 {
//	RGBA_QUAD* pixs;
//	int n;
//};
struct Pix {
	void* pixs;
	int n;
	int depth;
};

//typedef struct Pix32 PIX32;
typedef struct Pix PIX;

struct PixColormap {
	void* array; // RGB_QUAD
	int depth;
	int n_alloc;
	int n;
};

typedef struct PixColormap PIXCMAP;

#ifdef __cplusplus
extern "C" {
#endif
extern PIXCMAP* pix_cmap_generate_from_median_cuts(HEAP* heap, int* histo, int sigbits, int* count);
extern PIXCMAP* pix_cmap_create(int depth);
extern int pix_cmap_add_color(PIXCMAP* cmap, int r_val, int g_val, int b_val);
#ifdef __cplusplus
}
#endif
#endif
