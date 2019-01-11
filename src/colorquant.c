#include "colorquant.h"
#include <stdlib.h>

PIXCMAP* pix_median_cut_quant(PIX32* pix, int max_colors, int sigbits, int max_sub, int* count) {
	// TODO: para test
	if (sigbits == 0) {
		sigbits = DEFAULT_SIG_BITS;
	}
	if (max_sub <= 0) {
		max_sub = 10;
	}
	const int sub_sample = 1;
	int* histo = pix_median_cut_histo(pix, sigbits, sub_sample);

	BOX3D* vbox = pix_get_color_region(pix, sigbits, sub_sample);
	vbox->n_pix = vbox_get_count(vbox, histo, sigbits);
	vbox->vol = vbox_get_volume(vbox);

	HEAP* heap = heap_create(0, SORT_DECREASING);
	heap_add(heap, vbox);
	const int pop_colors = (int)(FRACT_BY_POPULATION * max_colors);
	int n_iters = 0;
	int n_colors = 1;
	BOX3D *vbox1, *vbox2;
	while (TRUE) {
		vbox = heap_remove(heap);
		if (vbox_get_count(vbox, histo, sigbits) == 0) {
			heap_add(heap, vbox);
			continue;
		}
		median_cut_apply(histo, sigbits, vbox, &vbox1, &vbox2);
		if (!vbox1) {
			break;
		}
		if (vbox1->vol > 1) {
			vbox1->sort_param = vbox1->n_pix;
		}
		free(vbox);
		heap_add(heap, vbox1);
		if (vbox2) {
			if (vbox2->vol > 1)
				vbox2->sort_param = vbox2->n_pix;
			heap_add(heap, vbox2);
			n_colors++;
		}
		if (n_colors >= pop_colors)
			break;
		if (n_iters++ > MAX_ITERS_ALLOWED) {
			break;
		}
	}
	HEAP* heap_sec = heap_create(0, SORT_DECREASING);
	while ((vbox = heap_remove(heap))) {
		vbox->sort_param = vbox->n_pix * vbox->vol;
		heap_add(heap_sec, vbox);
	}
	heap_destroy(&heap, TRUE);
	while (TRUE) {
		vbox = heap_remove(heap_sec);
		if (vbox_get_count(vbox, histo, sigbits) == 0) {
			heap_add(heap_sec, vbox);
			continue;
		}
		median_cut_apply(histo, sigbits, vbox, &vbox1, &vbox2);
		if (!vbox1) {
			break;
		}
		if (vbox1->vol > 1) {
			vbox1->sort_param = vbox1->n_pix * vbox1->vol;
		}
		free(vbox);
		heap_add(heap_sec, vbox1);
		if (vbox2) {
			if (vbox2->vol > 1) {
				vbox2->sort_param = vbox2->n_pix * vbox2->vol;
			}
			heap_add(heap_sec, vbox2);
			n_colors++;
		}
		if (n_colors >= max_colors)
			break;
		if (n_iters++ > MAX_ITERS_ALLOWED) {
			break;
		}
	}
	heap = heap_create(0, SORT_DECREASING);
	// re-sort
	while ((vbox = heap_remove(heap_sec))) {
		vbox->sort_param = vbox->n_pix;
		heap_add(heap, vbox);
	}
	heap_destroy(&heap_sec, TRUE);
	PIXCMAP* cmap = pix_cmap_generate_from_median_cuts(heap, histo, sigbits, count);
	heap_destroy(&heap, TRUE);
	free(histo);
	return cmap;
}
PIXCMAP* pix_median_cut_quant24(PIX24* pix, int max_colors, int sigbits, int max_sub, int* count) {
	// TODO: para test
	if (sigbits == 0) {
		sigbits = DEFAULT_SIG_BITS;
	}
	if (max_sub <= 0) {
		max_sub = 10;
	}
	const int sub_sample = max_sub;
	int* histo = pix_median_cut_histo24(pix, sigbits, sub_sample);

	BOX3D* vbox = pix_get_color_region24(pix, sigbits, sub_sample);
	vbox->n_pix = vbox_get_count(vbox, histo, sigbits);
	vbox->vol = vbox_get_volume(vbox);
	HEAP* heap = heap_create(0, SORT_DECREASING);
	heap_add(heap, vbox);
	const int pop_colors = (int)(FRACT_BY_POPULATION * max_colors);
	int n_iters = 0;
	int n_colors = 1;
	BOX3D *vbox1, *vbox2;
	while (TRUE) {
		vbox = heap_remove(heap);
		if (vbox_get_count(vbox, histo, sigbits) == 0) {
			heap_add(heap, vbox);
			continue;
		}
		median_cut_apply(histo, sigbits, vbox, &vbox1, &vbox2);
		if (!vbox1) {
			break;
		}
		if (vbox1->vol > 1) {
			vbox1->sort_param = vbox1->n_pix;
		}
		free(vbox);
		heap_add(heap, vbox1);
		if (vbox2) {
			if (vbox2->vol > 1)
				vbox2->sort_param = vbox2->n_pix;
			heap_add(heap, vbox2);
			n_colors++;
		}
		if (n_colors >= pop_colors)
			break;
		if (n_iters++ > MAX_ITERS_ALLOWED) {
			break;
		}
	}
	HEAP* heap_sec = heap_create(0, SORT_DECREASING);
	while ((vbox = heap_remove(heap))) {
		vbox->sort_param = vbox->n_pix * vbox->vol;
		heap_add(heap_sec, vbox);
	}
	heap_destroy(&heap, TRUE);
	while (TRUE) {
		vbox = heap_remove(heap_sec);
		if (vbox_get_count(vbox, histo, sigbits) == 0) {
			heap_add(heap_sec, vbox);
			continue;
		}
		median_cut_apply(histo, sigbits, vbox, &vbox1, &vbox2);
		if (!vbox1) {
			break;
		}
		if (vbox1->vol > 1) {
			vbox1->sort_param = vbox1->n_pix * vbox1->vol;
		}
		free(vbox);
		heap_add(heap_sec, vbox1);
		if (vbox2) {
			if (vbox2->vol > 1) {
				vbox2->sort_param = vbox2->n_pix * vbox2->vol;
			}
			heap_add(heap_sec, vbox2);
			n_colors++;
		}
		if (n_colors >= max_colors)
			break;
		if (n_iters++ > MAX_ITERS_ALLOWED) {
			break;
		}
	}
	heap = heap_create(0, SORT_DECREASING);
	// re-sort
	while ((vbox = heap_remove(heap_sec))) {
		vbox->sort_param = vbox->n_pix;
		heap_add(heap, vbox);
	}
	heap_destroy(&heap_sec, TRUE);
	PIXCMAP* cmap = pix_cmap_generate_from_median_cuts(heap, histo, sigbits, count);
	heap_destroy(&heap, TRUE);
	free(histo);
	return cmap;
}

int* pix_median_cut_histo(PIX32* pix, const int sigbits, int sub_sample) {
	const int histo_size = 1 << (3 * sigbits);
	int* histo = calloc(histo_size, sizeof(int));
	if (histo == NULL) {
		return NULL;
	}
	const int rshift = 8 - sigbits;
	RGBA_QUAD* pixs = pix->pixs;
	const int num = pix->n;
	int index;
	for (int i = 0; i < num; i += sub_sample) {
		if (pixs[i].alpha > 125) {
			if (!(pixs[i].red > 250 && pixs[i].green > 250 && pixs[i].blue > 250)) {
				get_color_index(&pixs[i], rshift, sigbits, &index);
				histo[index]++;
			}
		}
	}
	return histo;
}
int* pix_median_cut_histo24(PIX24* pix, const int sigbits, int sub_sample) {
	const int histo_size = 1 << (3 * sigbits);
	int* histo = calloc(histo_size, sizeof(int));
	if (histo == NULL) {
		return NULL;
	}
	const int rshift = 8 - sigbits;
	RGB_QUAD* pixs = pix->pixs;
	const int num = pix->n;
	int index;
	for (int i = 0; i < num; i += sub_sample) {
		if (!(pixs[i].red > 250 && pixs[i].green > 250 && pixs[i].blue > 250)) {
			get_color_index24(&pixs[i], rshift, sigbits, &index);
			histo[index]++;
		}
	}
	return histo;
}
void get_color_index(RGBA_QUAD* pix, int rshift, int sigbits, int* index) {
	const int r = pix->red >> rshift;
	const int g = pix->green >> rshift;
	const int b = pix->blue >> rshift;
	*index = (r << (2 * sigbits)) + (g << sigbits) + b;
}
void get_color_index24(RGB_QUAD* pix, int rshift, int sigbits, int* index) {
	const int r = pix->red >> rshift;
	const int g = pix->green >> rshift;
	const int b = pix->blue >> rshift;
	*index = (r << (2 * sigbits)) + (g << sigbits) + b;
}

BOX3D* pix_get_color_region(PIX32* pix, int sigbits, int sub_sample) {
	int g_min, b_min, g_max, b_max;

	int r_min = g_min = b_min = 1000000;
	int r_max = g_max = b_max = 0;
	const int rshift = 8 - sigbits;
	int mask = 0xff >> rshift;
	RGBA_QUAD* pixs = pix->pixs;
	const int num = pix->n;
	for (int i = 0; i < num; i += sub_sample) {
		RGBA_QUAD* pix = &pixs[i];
		const int r = pix->red >> rshift;
		const int g = pix->green >> rshift;
		const int b = pix->blue >> rshift;
		if (r < r_min) {
			r_min = r;
		}
		else if (r > r_max) {
			r_max = r;
		}

		if (g < g_min) {
			g_min = g;
		}
		else if (g > g_max) {
			g_max = g;
		}

		if (b < b_min) {
			b_min = b;
		}
		else if (b > b_max) {
			b_max = b;
		}
	}
	return box_3d_create(r_min, r_max, g_min, g_max, b_min, b_max);
}
BOX3D* pix_get_color_region24(PIX24* pix, int sigbits, int sub_sample) {
	int g_min, b_min, g_max, b_max;

	int r_min = g_min = b_min = 1000000;
	int r_max = g_max = b_max = 0;
	const int rshift = 8 - sigbits;
	int mask = 0xff >> rshift;
	RGB_QUAD* pixs = pix->pixs;
	const int num = pix->n;
	for (int i = 0; i < num; i += sub_sample) {
		RGB_QUAD* pix = &pixs[i];
		const int r = pix->red >> rshift;
		const int g = pix->green >> rshift;
		const int b = pix->blue >> rshift;
		if (r < r_min) {
			r_min = r;
		}
		else if (r > r_max) {
			r_max = r;
		}

		if (g < g_min) {
			g_min = g;
		}
		else if (g > g_max) {
			g_max = g;
		}

		if (b < b_min) {
			b_min = b;
		}
		else if (b > b_max) {
			b_max = b;
		}
	}
	return box_3d_create(r_min, r_max, g_min, g_max, b_min, b_max);
}
int median_cut_apply(int* histo, int sigbits, BOX3D* vbox, BOX3D** pvbox1, BOX3D** pvbox2) {
	if (pvbox1)
		*pvbox1 = NULL;
	if (pvbox2)
		*pvbox2 = NULL;
	if (vbox_get_count(vbox, histo, sigbits) == 0) {
		return 1;
	}
	const int rw = vbox->r2 - vbox->r1 + 1;
	const int gw = vbox->g2 - vbox->g1 + 1;
	const int bw = vbox->b2 - vbox->b1 + 1;
	if (rw == 1 && gw == 1 && bw == 1) {
		*pvbox1 = box_3d_copy(vbox);
		return 0;
	}
	int maxw = MAX(rw, gw);
	maxw = MAX(maxw, bw);
	int i, j, k, sum, index;
	int total = 0;
	int partial_sum[128];
	if (maxw == rw) {
		for (i = vbox->r1; i <= vbox->r2; i++) {
			sum = 0;
			for (j = vbox->g1; j <= vbox->g2; j++) {
				for (k = vbox->b1; k <= vbox->b2; k++) {
					index = (i << (2 * sigbits)) + (j << sigbits) + k;
					sum += histo[index];
				}
			}
			total += sum;
			partial_sum[i] = total;
		}
	}
	else if (maxw == gw) {
		for (i = vbox->g1; i <= vbox->g2; i++) {
			sum = 0;
			for (j = vbox->r1; j <= vbox->r2; j++) {
				for (k = vbox->b1; k <= vbox->b2; k++) {
					index = (i << sigbits) + (j << (2 * sigbits)) + k;
					sum += histo[index];
				}
			}
			total += sum;
			partial_sum[i] = total;
		}
	}
	else {
		/* maxw == bw */
		for (i = vbox->b1; i <= vbox->b2; i++) {
			sum = 0;
			for (j = vbox->r1; j <= vbox->r2; j++) {
				for (k = vbox->g1; k <= vbox->g2; k++) {
					index = i + (j << (2 * sigbits)) + (k << sigbits);
					sum += histo[index];
				}
			}
			total += sum;
			partial_sum[i] = total;
		}
	}
	BOX3D* vbox1 = NULL;
	BOX3D* vbox2 = NULL;
	int left, right;
	if (maxw == rw) {
		for (i = vbox->r1; i <= vbox->r2; i++) {
			if (partial_sum[i] > total / 2) {
				vbox1 = box_3d_copy(vbox);
				vbox2 = box_3d_copy(vbox);
				left = i - vbox->r1;
				right = vbox->r2 - i;
				if (left <= right)
					vbox1->r2 = MIN(vbox->r2 - 1, i + right / 2);
				else /* left > right */
					vbox1->r2 = MAX(vbox->r1, i - 1 - left / 2);
				vbox2->r1 = vbox1->r2 + 1;
				break;
			}
		}
	}
	else if (maxw == gw) {
		for (i = vbox->g1; i <= vbox->g2; i++) {
			if (partial_sum[i] > total / 2) {
				vbox1 = box_3d_copy(vbox);
				vbox2 = box_3d_copy(vbox);
				left = i - vbox->g1;
				right = vbox->g2 - i;
				if (left <= right)
					vbox1->g2 = MIN(vbox->g2 - 1, i + right / 2);
				else /* left > right */
					vbox1->g2 = MAX(vbox->g1, i - 1 - left / 2);
				vbox2->g1 = vbox1->g2 + 1;
				break;
			}
		}
	}
	else {
		/* maxw == bw */
		for (i = vbox->b1; i <= vbox->b2; i++) {
			if (partial_sum[i] > total / 2) {
				vbox1 = box_3d_copy(vbox);
				vbox2 = box_3d_copy(vbox);
				left = i - vbox->b1;
				right = vbox->b2 - i;
				if (left <= right)
					vbox1->b2 = MIN(vbox->b2 - 1, i + right / 2);
				else /* left > right */
					vbox1->b2 = MAX(vbox->b1, i - 1 - left / 2);
				vbox2->b1 = vbox1->b2 + 1;
				break;
			}
		}
	}
	*pvbox1 = vbox1;
	*pvbox2 = vbox2;
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
