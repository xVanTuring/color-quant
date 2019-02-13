#include "pix.h"
#include "vbox.h"

PIXCMAP *pix_cmap_create(int depth) {
  PIXCMAP *cmap = malloc(sizeof(PIXCMAP));
  if (cmap == NULL) {
    return NULL;
  }
  cmap->depth = depth;
  cmap->n_alloc = 1 << depth;
  RGBC_QUAD *cta = calloc(cmap->n_alloc, sizeof(RGBC_QUAD));
  cmap->array = cta;
  cmap->n = 0;
  return cmap;
}

int pix_cmap_add_color(PIXCMAP *cmap, int r_val, int g_val, int b_val,
                       int count) {
  RGBC_QUAD *cta = cmap->array;
  cta[cmap->n].red = r_val;
  cta[cmap->n].green = g_val;
  cta[cmap->n].blue = b_val;
  cta[cmap->n].count = count;
  cmap->n++;
  return 0;
}

PIXCMAP *pix_cmap_generate_from_median_cuts(HEAP *heap, int *histo,
                                            int sigbits) {
  int r, g, b;
  r = g = b = 0;
  PIXCMAP *cmap = pix_cmap_create(8);
  int index = 0;
  while (heap_get_count(heap) > 0) {
    BOX3D *vbox = heap_remove(heap);
    vbox_get_average_color(vbox, histo, sigbits, index, &r, &g, &b);
    pix_cmap_add_color(cmap, r, g, b, vbox->n_pix);
    free(vbox);
    index++;
  }
  return cmap;
}
