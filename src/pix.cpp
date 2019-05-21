#include "pix.h"
#include "vbox.h"

std::shared_ptr<PIXCMAP> pix_cmap_create(int depth) {
  std::shared_ptr<PIXCMAP> cmap = std::make_shared<PIXCMAP>();
  if (cmap == NULL) {
    return NULL;
  }
  cmap->depth = depth;
  cmap->n_alloc = 1 << depth;
  cmap->array = std::shared_ptr<RGBC_QUAD[]>(new RGBC_QUAD[cmap->n_alloc]);
  cmap->n = 0;
  return cmap;
}

int pix_cmap_add_color(std::shared_ptr<PIXCMAP> cmap, int r_val, int g_val,
                       int b_val, size_t count) {
  auto cta = cmap->array;
  cta[cmap->n].red = r_val;
  cta[cmap->n].green = g_val;
  cta[cmap->n].blue = b_val;
  cta[cmap->n].count = count;
  cmap->n++;
  return 0;
}

std::shared_ptr<PIXCMAP>
pix_cmap_generate_from_median_cuts(Box3dHeap &heap,
                                   std::shared_ptr<int[]> histo, int sigbits) {
  int r, g, b;
  r = g = b = 0;
  auto cmap = pix_cmap_create(8);
  int index = 0;
  while (heap.size() > 0) {
    std::shared_ptr<Box3d> vbox = heap.top();
    heap.pop();
    vbox_get_average_color(vbox, histo, sigbits, index, r, g, b);
    pix_cmap_add_color(cmap, r, g, b, vbox->n_pix);
    index++;
  }
  return cmap;
}
