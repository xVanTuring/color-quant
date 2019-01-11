#include <stdio.h>
#include <sys/stat.h>
#include "colorquant.h"
#include<time.h>
int file_size(char* filename);

int main() {
	
	const int f_size = file_size("D:\\Project\\MyPro\\color-quant\\js\\data.rgb") / 3;
	if (f_size <= 0) {
		return -1;
	}
	FILE* rgb_file = fopen("D:\\Project\\MyPro\\color-quant\\js\\data.rgb", "rb");
	RGB_QUAD* rgb_list = malloc(f_size * sizeof(RGB_QUAD));
	fread(rgb_list, sizeof(RGB_QUAD), f_size, rgb_file);
	PIX24* pix = malloc(sizeof(PIX24));
	pix->n = f_size;
	pix->pixs = rgb_list;
	const int max_color = 9;
	int* counter = malloc(sizeof(int) * max_color);
	const clock_t start = clock();
	PIXCMAP* cmap = pix_median_cut_quant24(pix, max_color, 5, 1, counter);
	const clock_t end = clock();
	printf("time=%f ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);
	RGB_QUAD *colorMapArray = cmap->array;
	for (size_t i = 0; i < cmap->n; i++)
	{
		printf("R: %d G: %d B: %d \n",
			colorMapArray[i].red,
			colorMapArray[i].green,
			colorMapArray[i].blue);
	}
	free(cmap->array);
	free(cmap);
	fclose(rgb_file);
	free(pix->pixs);
	free(pix);
	return 0;
}

int file_size(char* filename) {
	struct stat statbuf;
	stat(filename, &statbuf);
	const int size = statbuf.st_size;

	return size;
}
