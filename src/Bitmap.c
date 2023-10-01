#include "Bitmap.h"

/*
 * free Bitmap fields
 */
void freeBitmap(struct Bitmap *bmp) {
    freeBMPPixelArray(&bmp->pixelArray);
    return;
}

void printBMPHeader(const struct Bitmap *bmp) {
    const struct BMPHeader *header = &bmp->header;
    printf("%-25s: %c%c\n", "Signature", header->signature[0], header->signature[1]);
    printf("%-25s: %d KB\n", "File size", header->file_size/(1<<10));
    printf("%-25s: %d (bytes)\n", "Pixel array offset", header->pixelArrayByteoffset);
    return;
}

void printBMPDIB(const struct Bitmap *bmp) {
    const struct BMPDIB *dib = &bmp->dib;
    printf("%-25s: %d (px)\n", "Width", dib->imgWidth);
    printf("%-25s: %d (px)\n", "Height", dib->imgHeight);
    printf("%-25s: %d x %d\n", "Resolution", dib->horizontalResolution, dib->verticalResolution);
    printf("%-25s: %d\n", "Compression Algorithm", dib->compressionAlgorithm);
    printf("%-25s: %u\n", "Color planes", dib->planes);
    printf("%-25s: %u (bits)\n", "Color depth", dib->depth);
    printf("\n");
    return;
}

/*
 *print BMP file information 
 */
void printBMPInfo(const struct Bitmap *bmp) {
    printBMPHeader(bmp);
    printBMPDIB(bmp);
    printf("%-25s: %lu (bytes)\n","Size of Pixel", sizeof(struct BMPPixel));
    printf("%-25s: %d (bytes)\n","Padding", bmp->pixelArray.padding);
    return;
}

int8_t readBMPInfo(struct Bitmap *bmp, FILE *srcFile) {
    if (fread(bmp, sizeof(struct Bitmap), 1, srcFile) != 1) {
        printErrors(READ_FAIL);
        return 1;
    }
    bmp->pixelArray.data = NULL;
    bmp->pixelArray.height = 0;
    bmp->pixelArray.width = 0;
    bmp->pixelArray.padding = 0;
    int8_t error = prepareBMPPixelArray(&bmp->pixelArray, bmp->dib.imgWidth, bmp->dib.imgHeight);
    return error;
}

/*
 *read from `FILE *srcFile` and save to `*bmp`
 */
int8_t readBMP(struct Bitmap *bmp, FILE *srcFile) {
    int status;
    status = readBMPInfo(bmp, srcFile);
    if (status) {
        return 1;
    }
    status = readBMPPixelArray(&bmp->pixelArray, srcFile, bmp->header.pixelArrayByteoffset);
    if (status) {
        return 1;
    }
    return 0;
}

/*
 * write `Bitmap *bmp` to `FILE *desFile`
 */
void writeBitmap(const struct Bitmap *bmp, FILE *desFile) {
    if (bmp == NULL || desFile == NULL) {
        return;
    }

    fwrite(&bmp->header, sizeof(struct BMPHeader), 1, desFile);
    fwrite(&bmp->dib, sizeof(struct BMPDIB), 1, desFile);
    writeBMPPixelArray(&bmp->pixelArray, desFile, bmp->header.pixelArrayByteoffset);
}

void copyBitmap(struct Bitmap *desBitmap, struct Bitmap *srcBitmap) {
    desBitmap->header = srcBitmap->header;
    desBitmap->dib = srcBitmap->dib;
    copyBMPPixelArray(&srcBitmap->pixelArray, 0, 0, srcBitmap->dib.imgWidth, srcBitmap->dib.imgHeight, &desBitmap->pixelArray);
    return;
}

void cutBitmap(struct Bitmap *srcBitmap, int32_t start_row, int32_t start_column, uint32_t new_width, uint32_t new_height, struct Bitmap *desBitmap) {
    struct BMPHeader *desHeader = &desBitmap->header;
    struct BMPDIB *desDIB = &desBitmap->dib;
    
    *desHeader = srcBitmap->header;
    *desDIB = srcBitmap->dib;

    copyBMPPixelArray(&srcBitmap->pixelArray, start_row, start_column, new_width, new_height, &desBitmap->pixelArray);
    desDIB->imgWidth = desBitmap->pixelArray.width;
    desDIB->imgHeight = desBitmap->pixelArray.height;
    desHeader->file_size = desHeader->pixelArrayByteoffset + (new_width*sizeof(struct BMPPixel) + srcBitmap->pixelArray.padding)*new_height;

    return;
}