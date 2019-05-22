/*====================================================================*
 -  Copyright (C) 2001-2016 Leptonica.  All rights reserved.
 -
 -  Redistribution and use in source and binary forms, with or without
 -  modification, are permitted provided that the following conditions
 -  are met:
 -  1. Redistributions of source code must retain the above copyright
 -     notice, this list of conditions and the following disclaimer.
 -  2. Redistributions in binary form must reproduce the above
 -     copyright notice, this list of conditions and the following
 -     disclaimer in the documentation and/or other materials
 -     provided with the distribution.
 -
 -  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 -  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 -  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 -  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ANY
 -  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 -  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 -  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 -  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 -  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 -  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 -  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *====================================================================*/

#include "colorquant.h"

std::shared_ptr<PIXCMAP> histo_median_cut_quant(std::shared_ptr<int> histo,
                                                std::shared_ptr<Box3d> vbox,
                                                int max_colors, int sigbits) {
  if (sigbits == 0) {
    sigbits = DEFAULT_SIG_BITS;
  }
  auto heap = Box3dHeap();
  heap.push(vbox);
  const int pop_colors = (int)(FRACT_BY_POPULATION * max_colors);
  int n_iters = 0;
  int n_colors = 1;
  std::shared_ptr<Box3d> vbox1 = nullptr, vbox2 = nullptr;

  while (true) {
    vbox = heap.top();
    heap.pop();
    if (vbox_get_count(vbox, histo, sigbits) == 0) {
      heap.push(vbox);
      continue;
    }
    median_cut_apply(histo, sigbits, vbox, vbox1, vbox2);
    if (!vbox1) {
      break;
    }
    if (vbox1->n_pix > 0) {
      if (vbox1->vol > 1) {
        vbox1->sort_param = vbox1->n_pix;
      }
      heap.push(vbox1);
    } else {
      n_colors--;
    }

    if (vbox2) {
      if (vbox2->n_pix > 0) {
        if (vbox2->vol > 1) {
          vbox2->sort_param = vbox2->n_pix;
        }
        // heap_add(heap, vbox2);
        heap.push(vbox2);
        n_colors++;
      }
      // else {
      //   // free(vbox2);
      // }
    }
    if (n_colors >= pop_colors)
      break;
    if (n_iters++ > MAX_ITERS_ALLOWED) {
      break;
    }
  }
  auto heap_sec = Box3dHeap();
  while (heap.size() > 0) {
    vbox = heap.top();
    heap.pop();
    vbox->sort_param = vbox->n_pix * vbox->vol;
    heap_sec.push(vbox);
  }
  while (true) {
    vbox = heap_sec.top();
    heap_sec.pop();
    if (vbox_get_count(vbox, histo, sigbits) == 0) {
      // heap_add(heap_sec, vbox);
      heap_sec.push(vbox);
      continue;
    }
    median_cut_apply(histo, sigbits, vbox, vbox1, vbox2);
    if (!vbox1) {
      break;
    }
    // free(vbox);
    if (vbox1->n_pix > 0) {
      if (vbox1->vol > 1) {
        vbox1->sort_param = vbox1->n_pix * vbox1->vol;
      }
      // heap_add(heap_sec, vbox1);
      heap_sec.push(vbox1);
    } else {
      // free(vbox1);
      n_colors--;
    }
    if (vbox2) {
      if (vbox2->n_pix > 0) {
        if (vbox2->vol > 1) {
          vbox2->sort_param = vbox2->n_pix * vbox2->vol;
        }
        // heap_add(heap_sec, vbox2);
        heap_sec.push(vbox2);
        n_colors++;
      } else {
        // free(vbox2);
      }
    }
    if (n_colors >= max_colors)
      break;
    if (n_iters++ > MAX_ITERS_ALLOWED) {
      break;
    }
  }
  // re-sort
  while (heap_sec.size() > 0) {
    vbox = heap_sec.top();
    heap_sec.pop();
    vbox->sort_param = vbox->n_pix;
    heap.push(vbox);
  }
  auto cmap = pix_cmap_generate_from_median_cuts(heap, histo, sigbits);
  return cmap;
}
std::shared_ptr<PIXCMAP> pix_median_cut_quant(std::shared_ptr<PIX> pix,
                                              int max_colors, int sigbits,
                                              int max_sub) {
  if (pix->depth != 3 && pix->depth != 4) {
    return nullptr;
  }
  if (max_sub <= 0) {
    max_sub = 10;
  }
  int sub_sample = 1;
  if (max_sub == 1) {
    sub_sample = 1;
  } else {
    // 1000x1000 reso
    sub_sample = (int)(pix->n / 1000000.);
    sub_sample = std::max(1, std::min(max_sub, sub_sample));
  }
  auto histo = pix_median_cut_histo(pix, sigbits, sub_sample);

  const int histosize = 1 << (3 * sigbits);
  bool smalln = true;
  int ncolors = 0;
  for (int i = 0; i < histosize; i++) {
    if (histo.get()[i]) {
      ncolors++;
    }
    if (ncolors > max_colors) {
      smalln = false;
      break;
    }
  }
  if (smalln) {
    return nullptr;
  }

  std::shared_ptr<Box3d> vbox = pix_get_color_region(pix, sigbits, sub_sample);
  vbox->n_pix = vbox_get_count(vbox, histo, sigbits);
  vbox->vol = vbox_get_volume(vbox);
  if (histo == nullptr || vbox == nullptr) {
    return nullptr;
  }
  return histo_median_cut_quant(histo, vbox, max_colors, sigbits);
}
std::shared_ptr<int> pix_median_cut_histo(std::shared_ptr<PIX> pix, int sigbits,
                                          int sub_sample) {
  const int histo_size = 1 << (3 * sigbits);
  std::shared_ptr<int> histo(new int[histo_size](),
                             std::default_delete<int[]>());
  if (histo == nullptr) {
    return nullptr;
  }
  const int rshift = 8 - sigbits;
  const int num = pix->n;
  int index;
  for (int i = 0; i < num; i += sub_sample) {
    get_color_index(pix, i, rshift, sigbits, &index);
    histo.get()[index]++;
  }
  return histo;
}
void get_color_index(std::shared_ptr<PIX> pix, int i, int rshift, int sigbits,
                     int *index) {
  int r;
  int g;
  int b;
  if (pix->depth == 3) {
    RGB_Quad *pixs = (RGB_Quad *)pix->pixs;
    r = pixs[i].red >> rshift;
    g = pixs[i].green >> rshift;
    b = pixs[i].blue >> rshift;
  } else {
    RGBA_Quad *pixs = (RGBA_Quad *)pix->pixs;
    r = pixs[i].red >> rshift;
    g = pixs[i].green >> rshift;
    b = pixs[i].blue >> rshift;
  }
  *index = (r << (2 * sigbits)) + (g << sigbits) + b;
}
std::shared_ptr<Box3d> pix_get_color_region(std::shared_ptr<PIX> pix,
                                            int sigbits, int sub_sample) {

  int g_min, b_min, g_max, b_max;

  int r_min = g_min = b_min = 1000000;
  int r_max = g_max = b_max = 0;
  const int rshift = 8 - sigbits;
  const int num = pix->n;
  int r;
  int g;
  int b;
  for (int i = 0; i < num; i += sub_sample) {
    if (pix->depth == 3) {
      r = ((RGB_Quad *)pix->pixs)[i].red >> rshift;
      g = ((RGB_Quad *)pix->pixs)[i].green >> rshift;
      b = ((RGB_Quad *)pix->pixs)[i].blue >> rshift;
    } else {
      r = ((RGBA_Quad *)pix->pixs)[i].red >> rshift;
      g = ((RGBA_Quad *)pix->pixs)[i].green >> rshift;
      b = ((RGBA_Quad *)pix->pixs)[i].blue >> rshift;
    }

    if (r < r_min) {
      r_min = r;
    } else if (r > r_max) {
      r_max = r;
    }

    if (g < g_min) {
      g_min = g;
    } else if (g > g_max) {
      g_max = g;
    }

    if (b < b_min) {
      b_min = b;
    } else if (b > b_max) {
      b_max = b;
    }
  }
  return box_3d_create(r_min, r_max, g_min, g_max, b_min, b_max);
}
int median_cut_apply(std::shared_ptr<int> histo, int sigbits,
                     std::shared_ptr<Box3d> vbox,
                     std::shared_ptr<Box3d> &pvbox1,
                     std::shared_ptr<Box3d> &pvbox2) {
  if (pvbox1)
    pvbox1 = nullptr;
  if (pvbox2)
    pvbox2 = nullptr;
  if (vbox_get_count(vbox, histo, sigbits) == 0) {
    return 1;
  }
  if (vbox == nullptr) {
    return 1;
  }
  const int rw = vbox->r2 - vbox->r1 + 1;
  const int gw = vbox->g2 - vbox->g1 + 1;
  const int bw = vbox->b2 - vbox->b1 + 1;
  if (rw == 1 && gw == 1 && bw == 1) {
    pvbox1 = box_3d_copy(vbox);
    return 0;
  }
  int maxw = std::max(rw, gw);
  maxw = std::max(maxw, bw);
  int i, j, k, sum, index;
  int total = 0;
  int partial_sum[128] = {0};
  if (maxw == rw) {
    for (i = vbox->r1; i <= vbox->r2; i++) {
      sum = 0;
      for (j = vbox->g1; j <= vbox->g2; j++) {
        for (k = vbox->b1; k <= vbox->b2; k++) {
          index = (i << (2 * sigbits)) + (j << sigbits) + k;
          sum += histo.get()[index];
        }
      }
      total += sum;
      partial_sum[i] = total;
    }
  } else if (maxw == gw) {
    for (i = vbox->g1; i <= vbox->g2; i++) {
      sum = 0;
      for (j = vbox->r1; j <= vbox->r2; j++) {
        for (k = vbox->b1; k <= vbox->b2; k++) {
          index = (i << sigbits) + (j << (2 * sigbits)) + k;
          sum += histo.get()[index];
        }
      }
      total += sum;
      partial_sum[i] = total;
    }
  } else {
    /* maxw == bw */
    for (i = vbox->b1; i <= vbox->b2; i++) {
      sum = 0;
      for (j = vbox->r1; j <= vbox->r2; j++) {
        for (k = vbox->g1; k <= vbox->g2; k++) {
          index = i + (j << (2 * sigbits)) + (k << sigbits);
          sum += histo.get()[index];
        }
      }
      total += sum;
      partial_sum[i] = total;
    }
  }
  std::shared_ptr<Box3d> vbox1 = nullptr;
  std::shared_ptr<Box3d> vbox2 = nullptr;
  int left, right;
  if (maxw == rw) {
    for (i = vbox->r1; i <= vbox->r2; i++) {
      if (partial_sum[i] > total / 2) {
        vbox1 = box_3d_copy(vbox);
        vbox2 = box_3d_copy(vbox);
        left = i - vbox->r1;
        right = vbox->r2 - i;
        if (left <= right)
          vbox1->r2 = std::min(vbox->r2 - 1, i + right / 2);
        else /* left > right */
          vbox1->r2 = std::max(vbox->r1, i - 1 - left / 2);
        vbox2->r1 = vbox1->r2 + 1;
        break;
      }
    }
  } else if (maxw == gw) {
    for (i = vbox->g1; i <= vbox->g2; i++) {
      if (partial_sum[i] > total / 2) {
        vbox1 = box_3d_copy(vbox);
        vbox2 = box_3d_copy(vbox);
        left = i - vbox->g1;
        right = vbox->g2 - i;
        if (left <= right)
          vbox1->g2 = std::min(vbox->g2 - 1, i + right / 2);
        else /* left > right */
          vbox1->g2 = std::max(vbox->g1, i - 1 - left / 2);
        vbox2->g1 = vbox1->g2 + 1;
        break;
      }
    }
  } else {
    /* maxw == bw */
    for (i = vbox->b1; i <= vbox->b2; i++) {
      if (partial_sum[i] > total / 2) {
        vbox1 = box_3d_copy(vbox);
        vbox2 = box_3d_copy(vbox);
        left = i - vbox->b1;
        right = vbox->b2 - i;
        if (left <= right)
          vbox1->b2 = std::min(vbox->b2 - 1, i + right / 2);
        else /* left > right */
          vbox1->b2 = std::max(vbox->b1, i - 1 - left / 2);
        vbox2->b1 = vbox1->b2 + 1;
        break;
      }
    }
  }
  pvbox1 = vbox1;
  pvbox2 = vbox2;
  if (!vbox1) {
    return 1;
  }
  if (!vbox2) {
    return 1;
  }
  vbox1->n_pix = vbox_get_count(vbox1, histo, sigbits);
  vbox2->n_pix = vbox_get_count(vbox2, histo, sigbits);
  vbox1->vol = vbox_get_volume(vbox1);
  vbox2->vol = vbox_get_volume(vbox2);
  return 0;
}
