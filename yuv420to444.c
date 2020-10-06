#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t g_planar_format = 0;

void convert420to422(uint8_t* input, uint32_t w, uint32_t h, uint8_t* output) {
	if ((input == NULL) || (output == NULL)) {
		printf("Invalid input parameters, input buf = %p, output buf = %p\n", input, output);
		return;
	}
	else if ((w * h) <= 0) {
		printf("Invalid input size, %dx%d\n", w, h);
		return;
	}
	else {
		uint8_t* pYSrc = input;
		uint8_t* pUSrc = pYSrc + (w * h);
		uint8_t* pVSrc = pUSrc + ((w * h) / 4);
		uint8_t* pYDst = output;
		uint8_t* pUDst = pYDst + (w * h);
		uint8_t* pVDst = pUDst + (w * h / 2);
		uint32_t i;
		uint32_t stride = w / 2;

		printf("Converting 420 -> 422\n");

		// The luma Y channel is the same.
		memcpy(pYDst, pYSrc, w * h);

		// Convert the chroma channels, upconvert the vertical to full resolution
		for (i = 0; i < h / 2; i++) {
			// Use the nearest neighbour

			memcpy(pUDst + (i * 2 * stride), pUSrc + (i * stride), stride);
			// The new next U chroma line has the same values as the previous line.
			memcpy(pUDst + (((i * 2) + 1) * stride), pUSrc + (i * stride), stride);

			memcpy(pVDst + (i * 2 * stride), pVSrc + (i * stride), stride);
			// The new next V chroma line has the same values as the previous line.
			memcpy(pVDst + (((i * 2) + 1) * stride), pVSrc + (i * stride), stride);
		}
	}
}

void convert422to444(uint8_t* input, uint32_t w, uint32_t h, uint8_t* output) {
	if ((input == NULL) || (output == NULL)) {
		printf("Invalid input parameters, input buf = %p, output buf = %p\n", input, output);
		return;
	}
	else if ((w * h) <= 0) {
		printf("Invalid input size, %dx%d\n", w, h);
		return;
	}
	else {
		uint8_t* pYSrc = input;
		uint8_t* pUSrc = pYSrc + (w * h);
		uint8_t* pVSrc = pUSrc + ((w * h) / 2);
		uint8_t* pYDst = output;
		uint8_t* pUDst = pYDst + (w * h);
		uint8_t* pVDst = pUDst + (w * h);
		uint8_t* pUVDst = pYDst + (w * h);
		uint32_t i, k;
		uint32_t stride_src = w / 2;
		uint32_t stride_dst = w;

		printf("Converting 422 -> 444, planar = %d\n", g_planar_format);

		// The luma Y channel is the same.
		memcpy(pYDst, pYSrc, w * h);

		// Convert the chroma channels, upconvert the horizontal to full resolution
		for (i = 0; i < h; i++) {
			// Use the nearest neighbour
			// The new next U/V chromas have the same values as the previous ones.
			for (k = 0; k < w / 2; k++) {
				if (g_planar_format == 0) { // semi-planar, interleaved UV
					pUVDst[(i * 2 * stride_dst) + (k * 4) + 0] = pUSrc[(i * stride_src) + k];
					pUVDst[(i * 2 * stride_dst) + (k * 4) + 1] = pVSrc[(i * stride_src) + k];
					pUVDst[(i * 2 * stride_dst) + (k * 4) + 2] = pUSrc[(i * stride_src) + k];
					pUVDst[(i * 2 * stride_dst) + (k * 4) + 3] = pVSrc[(i * stride_src) + k];
				}
				else {
					pUDst[(i * stride_dst) + (k * 2) + 0] = pUSrc[(i * stride_src) + k];
					pUDst[(i * stride_dst) + (k * 2) + 1] = pUSrc[(i * stride_src) + k];

					pVDst[(i * stride_dst) + (k * 2) + 0] = pVSrc[(i * stride_src) + k];
					pVDst[(i * stride_dst) + (k * 2) + 1] = pVSrc[(i * stride_src) + k];
				}
			}
		}
	}
}

void convert420to444(uint8_t* input, uint32_t w, uint32_t h, uint8_t* output) {
	if ((input == NULL) || (output == NULL)) {
		printf("Error, invalid input parameters, input buf = %p, output buf = %p\n", input, output);
		return;
	}
	else if (((int32_t)w * (int32_t)h) <= 0) {
		printf("Error, invalid input size, w = %d, h = %d\n", w, h);
		return;
	}
	else {
		uint8_t* pYSrc = input;
		uint8_t* pUSrc = pYSrc + (w * h);
		uint8_t* pVSrc = pUSrc + ((w * h) / 4);
		uint8_t* pYDst = output;
		uint8_t* pUDst = pYDst + (w * h);
		uint8_t* pVDst = pYDst + (w * h * 2);
		uint8_t* pUVDst = pYDst + (w * h);
		uint32_t stride_src = w / 2;
		uint32_t stride_dst = w;
		uint32_t i, k;
		uint32_t cur_line_dst = 0;
		uint32_t cur_line_src = 0;

		printf("Converting 420 -> 444, planar = %d\n", g_planar_format);

		// The luma Y channel is the same.
		memcpy(pYDst, pYSrc, w * h);

		// Convert the chroma channels.
		//
		// Upconvert the vertical and horizontal to full resolution
		// Use the nearest neighbor, the next chroma line/column has the same values as the previous line/column.
		for (i = 0; i < h; i++) {
			cur_line_src = i / 2 * stride_src;

			for (k = 0; k < w/2; k++) {
				if (g_planar_format == 0) {	// semi-planar, interleaved UV in the second plane
					// Double the horizontal
					// UVUV
					cur_line_dst = i * 2 * stride_dst;
					pUVDst[cur_line_dst + (k * 4) + 0] = pUSrc[cur_line_src + k];
					pUVDst[cur_line_dst + (k * 4) + 1] = pVSrc[cur_line_src + k];
					pUVDst[cur_line_dst + (k * 4) + 2] = pUSrc[cur_line_src + k];
					pUVDst[cur_line_dst + (k * 4) + 3] = pVSrc[cur_line_src + k];
				}
				else {
					// Double the horizontal for U
					// UU
					cur_line_dst = i * stride_dst;
					pUDst[cur_line_dst + (k * 2) + 0] = pUSrc[cur_line_src + k];
					pUDst[cur_line_dst + (k * 2) + 1] = pUSrc[cur_line_src + k];

					// Double the horizontal for V
					// VV
					pVDst[cur_line_dst + (k * 2) + 0] = pVSrc[cur_line_src + k];
					pVDst[cur_line_dst + (k * 2) + 1] = pVSrc[cur_line_src + k];
				}
			}
		}
	}
}

int main(int argc, char** argv) {
	FILE* fin = NULL;
	FILE* fout = NULL;
	int32_t w, h;
	int err;

	if (argc < 5) {
		printf("Usage: %s <input yuv420 planar file name> <width> <height> <output yuv444 semi-planar file name> <optional planar format: 0|1>\n", argv[0]);
		return 0;
	}

	w = atoi(argv[2]);
	h = atoi(argv[3]);

	if (argc == 6) {
		g_planar_format = atoi(argv[5]);
	}

	printf("Input info:\n");
	printf("  input file: %s\n", argv[1]);
	printf("  size: %dx%d\n", w, h);
	printf("  output file: %s\n", argv[4]);

	if ((w * h) <= 0) {
		printf("Error, invalid input size, w = %d, h = %d\n", w, h);
		return 0;
	}

#ifdef __GNUC__
	fin = fopen(argv[1], "rb");
#else
	err = fopen_s(&fin, argv[1], "rb");
#endif

	if (err == 0 && fin) {
#ifdef __GNUC__
		fout = fopen(argv[4], "wb");
#else
		err = fopen_s(&fout, argv[4], "wb");
#endif

		if (err == 0 && fout) {
			int in_size = w * h * 3 / 2;
			int out_size = w * h * 3;	// yuv444
			uint8_t* buf_input = (uint8_t*) malloc(in_size);
			uint8_t* buf_output = (uint8_t*) malloc(out_size);

			if (buf_input == NULL) {
				printf("Error, failed to allocate input buffer, %d bytes\n", in_size);
			}
			if (buf_output == NULL) {
				printf("Error, failed to allocate output buffer, %d bytes\n", out_size);
			}

			if (buf_input && buf_output) {
				// Read input data
				int read = fread(buf_input, 1, in_size, fin);
				if (read < in_size) {
					printf("Warning, read size is %d, expecting %d, continue...", read, in_size);
				}

				// Do the conversion
				// We can also convert 420->422->444, just for fun!
				convert420to444(buf_input, w, h, buf_output);

				// Write the converted data to file
				fwrite(buf_output, out_size, 1, fout);
				printf("Converted, size = %d bytes\n", out_size);
			}

			if (buf_input)
				free(buf_input);
			if (buf_output)
				free(buf_output);

			fclose(fout);
		}
		else {
			printf("Error, failed to create output file %s, err = %d\n", argv[4], err);
		}

		fclose(fin);
	}
	else {
		printf("Error, failed to open input file %s, err = %d\n", argv[1], err);
	}

	return 0;
}
