#pragma warning(disable : 4996)
#include "colorquant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <time.h>
int file_size(const char *filename);
int run(const char *filename, int depth);
int main() {
  std::string filename4 = "./script/data.4.rgb";
  std::string filename3 = "./script/data.3.rgb";
  // on windows if you double click the exe,
  // make sure it's next to data.rgb
  run(filename4.c_str(), 4);
  run(filename3.c_str(), 3);
  return 0;
}
int run(const char *filename, int depth) {
  printf("%s\n", filename);
  int f_size = file_size(filename) / 3;
  if (depth == 4) {
    f_size = file_size(filename) / 4;
  }
  if (f_size <= 0) {
    printf("No File! Exiting...\n");
    return -1;
  }
  FILE *rgb_file = fopen(filename, "rb");
  PIX *pix = new PIX;
  void *rgb_list;
  if (depth == 3) {
    rgb_list = malloc(f_size * sizeof(RGB_Quad));
    if (rgb_list == NULL) {
      return -1;
    }
    fread(rgb_list, sizeof(RGB_Quad), f_size, rgb_file);
  } else {
    rgb_list = new RGBA_Quad[f_size];
    if (rgb_list == NULL) {
      return -1;
    }
    fread(rgb_list, sizeof(RGBA_Quad), f_size, rgb_file);
  }
  if (pix == NULL) {
    return -1;
  }
  pix->pixs = rgb_list;
  pix->depth = depth;
  pix->n = f_size;
  const int max_color = 9;
  const clock_t start = clock();
  PIXCMAP *cmap = pix_median_cut_quant(pix, max_color, 5, 0);
  const clock_t end = clock();
  printf("time=%f ms\n", ((double)end - start) / CLOCKS_PER_SEC * 1000);
  if (cmap == NULL) {
    return -1;
  }
  RGBC_QUAD *colorMapArray = cmap->array;
  for (int i = 0; i < cmap->n; i++) {
    printf("rgb(%d, %d, %d) %lu \n", colorMapArray[i].red,
           colorMapArray[i].green, colorMapArray[i].blue,
           colorMapArray[i].count);
  }
  free(cmap->array);
  free(cmap);
  fclose(rgb_file);
  free(pix->pixs);
  free(pix);
  return 0;
}

int file_size(const char *filename) {
  struct stat statbuf;
  stat(filename, &statbuf);
  const int size = statbuf.st_size;
  return size;
}
