#ifndef BITMAP_H
#define BITMAP_H

#include"BMPPixelArray.h"
#include"imgError.h"
#include<stdio.h>
#include<stdint.h>

#pragma pack(push , 1)

struct BMPHeader {
    char signature[2];
    int32_t file_size;
    int16_t reserved[2];
    int32_t pixelArrayByteoffset;
};

struct BMPDIB {
    uint32_t DIB_size;
    uint32_t imgWidth;
    uint32_t imgHeight;
    uint16_t planes;
    uint16_t depth;
    uint32_t compressionAlgorithm;
    uint32_t colorTable_size;
    int32_t horizontalResolution;
    int32_t verticalResolution;
    uint32_t countColorsInColorTable;
    uint32_t countImportantColorsInColorTable;
};

struct Bitmap {
    struct BMPHeader header;
    struct BMPDIB dib;
    struct BMPPixelArray pixelArray;
};

#pragma pack(pop)

void freeBitmap(struct Bitmap *bmp);

void printBMPInfo(const struct Bitmap *bmp);

int8_t readBMP(struct Bitmap *bmp, FILE *srcFile);

void writeBitmap(const struct Bitmap *bmp, FILE *desFile);


void copyBitmap(struct Bitmap *desBitmap, struct Bitmap *srcBitmap);

void cutBitmap(struct Bitmap *srcBitmap, int32_t start_row, int32_t start_column, uint32_t new_width, uint32_t new_height, struct Bitmap *desBitmap);

#endif  //BITMAP_H