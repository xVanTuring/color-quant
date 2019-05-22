#pragma warning(disable : 4996)
#include "colorquant.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <time.h>
int file_size(std::string filename);
int run(std::string filename, int depth);
int main() {
  std::string filename4 = "./script/data.4.rgb";
  std::string filename3 = "./script/data.3.rgb";
  run(filename4, 4);
  run(filename3, 3);
  return 0;
}
int run(std::string filename, int depth) {
  std::cout << filename << std::endl;
  int f_size = file_size(filename) / 3;
  if (depth == 4) {
    f_size = file_size(filename) / 4;
  }
  if (f_size <= 0) {
    printf("No File! Exiting...\n");
    return -1;
  }
  FILE *rgb_file = fopen(filename.c_str(), "rb");
  std::shared_ptr<PIX> pix = std::make_shared<PIX>();
  void *rgb_list;
  if (depth == 3) {
    rgb_list = malloc(f_size * sizeof(RGB_Quad));
    if (rgb_list == nullptr) {
      return -1;
    }
    fread(rgb_list, sizeof(RGB_Quad), f_size, rgb_file);
  } else {
    rgb_list = new RGBA_Quad[f_size];
    if (rgb_list == nullptr) {
      return -1;
    }
    fread(rgb_list, sizeof(RGBA_Quad), f_size, rgb_file);
  }
  if (pix == nullptr) {
    return -1;
  }
  pix->pixs = rgb_list;
  pix->depth = depth;
  pix->n = f_size;
  const int max_color = 9;
  const clock_t start = clock();
  std::shared_ptr<PIXCMAP> cmap = pix_median_cut_quant(pix, max_color, 5, 0);
  const clock_t end = clock();
  printf("time=%f ms\n", ((double)end - start) / CLOCKS_PER_SEC * 1000);
  if (cmap == nullptr) {
    return -1;
  }
  auto colorMapArray =
      cmap->array;
  for (auto &item : (*colorMapArray)) {
    printf("rgb(%d, %d, %d) %lu \n", item->red, item->green, item->blue,
           item->count);
  }
  fclose(rgb_file);
  free(pix->pixs);
  return 0;
}

int file_size(std::string filename) {
  struct stat statbuf;
  stat(filename.c_str(), &statbuf);
  const int size = statbuf.st_size;
  return size;
}
