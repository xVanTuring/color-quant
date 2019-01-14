#include <stdio.h>
#include <sys/stat.h>
#include "colorquant.h"
#include <time.h>
int file_size(char* filename);
int run(char *filename, int depth);
int main() {
	char *filename1 = "data.4.rgb";
	char *filename3 = "data.3.rgb";
	// on windows if you double click the exe, 
	// make sure it's next to data.rgb
	run(filename1, 4);
	run(filename3, 3);
	return 0;
}
int run(char *filename, int depth) {
	printf("%s\n", filename);
	int f_size = file_size(filename) / 3;
	if (depth == 4) {
		f_size = file_size(filename) / 4;
	}
	if (f_size <= 0) {
		printf("No File! Exiting...\n");
		return -1;
	}
	FILE* rgb_file = fopen(filename, "rb");
	PIX* pix = malloc(sizeof(PIX));
	void* rgb_list;
	if (depth == 3) {
		rgb_list = malloc(f_size * sizeof(RGB_QUAD));
		if (rgb_list == NULL) {
			return -1;
		}
		fread(rgb_list, sizeof(RGB_QUAD), f_size, rgb_file);
	}
	else {
		rgb_list = malloc(f_size * sizeof(RGBA_QUAD));
		if (rgb_list == NULL) {
			return -1;
		}
		fread(rgb_list, sizeof(RGBA_QUAD), f_size, rgb_file);
	}
	if (pix == NULL) {
		return -1;
	}
	pix->pixs = rgb_list;
	pix->depth = depth;
	pix->n = f_size;
	const int max_color = 9;
	size_t* counter = malloc(sizeof(size_t) * max_color);
	if (counter == NULL) {
		return -1;
	}
	const clock_t start = clock();
	PIXCMAP* cmap = pix_median_cut_quant(pix, max_color, 5, 40, counter);
	const clock_t end = clock();
	printf("time=%f ms\n", ((double)end - start) / CLOCKS_PER_SEC * 1000);
	if (cmap == NULL) {
		return -1;
	}
	RGB_QUAD *colorMapArray = cmap->array;
	for (int i = 0; i < cmap->n; i++) {
		printf("rgb(%d, %d, %d) %d \n",
			colorMapArray[i].red,
			colorMapArray[i].green,
			colorMapArray[i].blue, counter[i]);
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
