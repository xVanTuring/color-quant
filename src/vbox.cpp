#include "vbox.h"

std::shared_ptr<Box3d> box_3d_create(int r1, int r2, int g1, int g2, int b1,
                                     int b2) {
  auto vbox = std::make_shared<Box3d>();
  vbox->r1 = r1;
  vbox->r2 = r2;
  vbox->g1 = g1;
  vbox->g2 = g2;
  vbox->b1 = b1;
  vbox->b2 = b2;
  return vbox;
}

std::shared_ptr<Box3d> box_3d_copy(std::shared_ptr<Box3d> vbox) {
  auto vbox_clone =
      box_3d_create(vbox->r1, vbox->r2, vbox->g1, vbox->g2, vbox->b1, vbox->b2);
  vbox_clone->n_pix = vbox->n_pix;
  vbox_clone->vol = vbox->vol;
  return vbox_clone;
}

size_t vbox_get_count(std::shared_ptr<Box3d> vbox, std::shared_ptr<int> histo,
                      int sigbits) {
  size_t n_pix = 0;
  for (int r = vbox->r1; r <= vbox->r2; ++r) {
    for (int g = vbox->g1; g <= vbox->g2; ++g) {
      for (int b = vbox->b1; b <= vbox->b2; ++b) {
        const int index = (r << (2 * sigbits)) + (g << sigbits) + b;
        n_pix += histo.get()[index];
      }
    }
  }
  // printf("%u\n", n_pix);
  return n_pix;
}

size_t vbox_get_volume(std::shared_ptr<Box3d> vbox) {
  if (!vbox) {
    return 0;
  }
  return ((vbox->r2 - vbox->r1 + 1) * (vbox->g2 - vbox->g1 + 1) *
          (vbox->b2 - vbox->b1 + 1));
}

void vbox_get_average_color(std::shared_ptr<Box3d> vbox,
                            std::shared_ptr<int> histo, int sigbits,
                            int index, int &p_rval, int &p_gval, int &p_bval) {
  p_rval = p_gval = p_bval = 0;
  int n_total = 0;
  const int multiple = 1 << (8 - sigbits);
  int r_sum, g_sum, b_sum;
  r_sum = g_sum = b_sum = 0;
  for (int r = vbox->r1; r <= vbox->r2; ++r) {
    for (int g = vbox->g1; g <= vbox->g2; ++g) {
      for (int b = vbox->b1; b <= vbox->b2; ++b) {
        const int histo_index = (r << 2 * sigbits) + (g << sigbits) + b;
        n_total += histo.get()[histo_index];
        r_sum += (int)(histo.get()[histo_index] * (r + 0.5) * multiple);
        g_sum += (int)(histo.get()[histo_index] * (g + 0.5) * multiple);
        b_sum += (int)(histo.get()[histo_index] * (b + 0.5) * multiple);
        if (index >= 0) {
          histo.get()[histo_index] = index;
        }
      }
    }
  }
  if (n_total == 0) {
    p_rval = multiple * (vbox->r1 + vbox->r2 + 1) / 2;
    p_gval = multiple * (vbox->g1 + vbox->g2 + 1) / 2;
    p_bval = multiple * (vbox->b1 + vbox->b2 + 1) / 2;
  } else {
    p_rval = r_sum / n_total;
    p_gval = g_sum / n_total;
    p_bval = b_sum / n_total;
  }
}
