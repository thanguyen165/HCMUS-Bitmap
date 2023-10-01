#include"BMPPixelArray.h"
#include"imgError.h"

/*
 * delete all the fields
 */
void freeBMPPixelArray(struct BMPPixelArray *pixelArray) {
    if (pixelArray->data == NULL) return;
    
    for (uint32_t i = 0; i < pixelArray->height; ++i) {
        free(pixelArray->data[i]);
    }

    free(pixelArray->data);

    pixelArray->data = NULL;
    pixelArray->width = 0;
    pixelArray->height = 0;
    pixelArray->padding = 0;

    return;
}

/*
 * allocating the pixel array
 */
int8_t prepareBMPPixelArray(struct BMPPixelArray *pixelArray, uint32_t width, uint32_t height) {
    if (pixelArray == NULL) {
        return 1;
    }

    // delete old pixel array
    freeBMPPixelArray(pixelArray);
    
    uint32_t widthInBytes = width * sizeof(struct BMPPixel);
    uint32_t padding = (4 - widthInBytes % 4) % 4;

    struct BMPPixel** pixels = (struct BMPPixel**)calloc(height, sizeof(struct BMPPixel*));
    if (isAllocated(pixels) != SUCCESSFULLY) {
        return 1;
    }

    for (int32_t h = 0; h < height; ++h) {
        // extra for the padding
        pixels[h] = (struct BMPPixel*)malloc(widthInBytes + padding);
        if (isAllocated(pixels[h]) != SUCCESSFULLY) {
            // free the allocated rows
            for (int32_t i = 0; i < h; ++i) {
                free(pixels[i]);
                pixels[i] = NULL;
            }
            break;
        }
    }

    // if allocated fail
    if (pixels[0] == NULL) {  
        free(pixels);
        pixelArray->data = NULL;
        return 1;
    }

    pixelArray->width = width;
    pixelArray->height = height;
    pixelArray->data = pixels;
    pixelArray->padding = padding;

    return 0;
}

/*
 * read Color Table from `FILE *srcFile`
 */
int8_t readBMPPixelArray(struct BMPPixelArray *pixelArray, FILE *srcFile, const int32_t off_set) {
    if (pixelArray->data == NULL || srcFile == NULL) {
        return 1;
    }

    uint32_t width = pixelArray->width;
    uint32_t height = pixelArray->height;
    uint32_t padding = pixelArray->padding;
    uint32_t widthInBytes = width * sizeof(struct BMPPixel);
    uint8_t buffer[10];
    
    // move FILE pointer to color table position
    fseek(srcFile, off_set, SEEK_SET);

    for (int32_t h = 0; h < height; ++h) {
        // read all the pixels in row, one by one
        for (int32_t c = 0; c < width; ++c) {
            fread(buffer, sizeof(struct BMPPixel), 1, srcFile);
            pixelArray->data[h][c].blue = buffer[0];
            pixelArray->data[h][c].green = buffer[1];
            pixelArray->data[h][c].red = buffer[2];
        }
        // read the padding
        fread(buffer, padding, 1, srcFile);
        memcpy(pixelArray->data[h] + widthInBytes, buffer, padding);
    }

    return 0;
}

/*
 * write Color Table to `FILE *desFile`
 */
void writeBMPPixelArray(const struct BMPPixelArray *pixelArray, FILE *desFile, const int32_t off_set) {
    if (pixelArray->data == NULL || desFile == NULL) {
        return;
    }

    // move FILE pointer to color table position
    fseek(desFile, off_set, SEEK_SET);

    uint8_t padding_values[3] = {0};
    for (uint32_t h = 0; h < pixelArray->height; ++h) {
        // write all the pixels to file, one by one
        for (uint32_t c = 0; c < pixelArray->width; ++c) {
            fwrite(&pixelArray->data[h][c], sizeof(struct BMPPixel), 1, desFile);
        }
        // write down the padding
        fwrite(padding_values, pixelArray->padding, 1, desFile);
    }

    return;
}

void replaceBMPPixelArray(struct BMPPixelArray *src, int32_t row_src, int32_t column_src, uint32_t w, uint32_t h, struct BMPPixelArray *des, int32_t row_des, int32_t column_des) {
    if (src == NULL || des == NULL) return;
    if (src->data == NULL || des->data == NULL) return;

    for (int32_t r = 0; r < h; ++r) {
        for (int32_t c = 0; c < w; ++c) {
            des->data[row_des + r][column_des + c] = src->data[row_src + r][column_src + c];
        }
    }

    return;
}

int8_t copyBMPPixelArray(struct BMPPixelArray *src, int32_t row_src, int32_t column_src, uint32_t w, uint32_t h, struct BMPPixelArray *des) {
    // delete old data from BMPPixelArray *des
    freeBMPPixelArray(des);
    
    int8_t isError = prepareBMPPixelArray(des, w, h);
    if (isError) {
        return 1;
    }

    int32_t row_des = 0;
    int32_t column_des = 0;
    replaceBMPPixelArray(src, row_src, column_src, w, h, des, row_des, column_des);
    return 0;
}