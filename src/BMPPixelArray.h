#ifndef BMP_PIXEL_ARRAY
#define BMP_PIXEL_ARRAY

#include"BMPPixel.h"
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

struct BMPPixelArray {
    uint32_t width;
    uint32_t height;
    uint32_t padding;
    struct BMPPixel **data;
};

void freeBMPPixelArray(struct BMPPixelArray *pixelArray);

int8_t prepareBMPPixelArray(struct BMPPixelArray *pixelArray, uint32_t width, uint32_t height);

int8_t readBMPPixelArray(struct BMPPixelArray *pixelArray, FILE *srcFile, const int32_t off_set);

void writeBMPPixelArray(const struct BMPPixelArray *pixelArray, FILE *desFile, const int32_t off_set);

void replaceBMPPixelArray(struct BMPPixelArray *src, int32_t row_src, int32_t column_src, uint32_t w, uint32_t h, struct BMPPixelArray *des, int32_t row_des, int32_t column_des);

int8_t copyBMPPixelArray(struct BMPPixelArray *src, int32_t row_src, int32_t column_src, uint32_t w, uint32_t h, struct BMPPixelArray *des);


#endif  //BMP_PIXEL_ARRAY