#ifndef PIX_H
#define PIX_H 1
#include "vbox.h"
#include <memory>
#include <queue>
struct RGBA_Quad {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
};

// typedef struct RGBA_Quad RGBA_QUAD;

struct RGB_Quad {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};


struct RGBC_QUAD {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  size_t count;
};

struct Pix {
  void *pixs;
  int n;
  int depth;
};

typedef struct Pix PIX;

struct PixColormap {
  RGBC_QUAD *array; //
  int depth;
  int n_alloc;
  int n;
};

typedef struct PixColormap PIXCMAP;

PIXCMAP *pix_cmap_generate_from_median_cuts(Box3dHeap &heap,
                                            std::shared_ptr<int[]> histo,
                                            int sigbits);
PIXCMAP *pix_cmap_create(int depth);
int pix_cmap_add_color(PIXCMAP *cmap, int r_val, int g_val, int b_val,
                       size_t count);
#endif
