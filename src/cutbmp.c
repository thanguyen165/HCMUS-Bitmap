#include"imgError.h"
#include"Bitmap.h"
#include<string.h>
#include<stdlib.h>

int main(int arvc, char ** argv) {
    int8_t status;

    FILE *f = fopen(argv[1], "rb");
    // if file cannot be opened, exit program
    if (isOpened(f) != SUCCESSFULLY) { 
        return 1;
    }

    struct Bitmap *bmp = (struct Bitmap*)calloc(1, sizeof(struct Bitmap));
    if (bmp == NULL) {
        fclose(f);
        return 1;
    }

    status = readBMP(bmp, f);
    if (status) {
        freeBitmap(bmp);
        free(bmp);
        bmp = NULL;
        fclose(f);
        return 1;
    }
    printBMPInfo(bmp);

    struct Bitmap *bmp_tmp = (struct Bitmap*)calloc(1, sizeof(struct Bitmap));
    if (bmp_tmp == NULL) {
        freeBitmap(bmp);
        free(bmp);
        bmp = NULL;
        fclose(f);
        return 1;
    }

    int32_t piece_height = 1, piece_width = 1;
    if (arvc == 4) {
        if (strcmp(argv[2], "-h") == 0) {
            piece_height = atoi(argv[3]);
        }
        if (strcmp(argv[2], "-w") == 0) {
            piece_width = atoi(argv[3]);
        }
    }
    else if (arvc == 6) {
        if (strcmp(argv[2], "-h") == 0) {
            piece_height = atoi(argv[3]);
        }
        if (strcmp(argv[2], "-w") == 0) {
            piece_width = atoi(argv[3]);
        }
        if (strcmp(argv[4], "-h") == 0) {
            piece_height = atoi(argv[5]);
        }
        if (strcmp(argv[4], "-w") == 0) {
            piece_width = atoi(argv[5]);
        }
    }

    for (int32_t r = 0; r < piece_height; r++) {
        for (int32_t c = 0; c < piece_width; c++) {
            char path[] = "output01.bmp";
            path[6] = r + '0';
            path[7] = c + '0';
            FILE *g = fopen(path, "wb");
            cutBitmap(bmp, r * bmp->dib.imgHeight/piece_height, c * bmp->dib.imgWidth/piece_width, bmp->dib.imgWidth/piece_width, bmp->dib.imgHeight/piece_height, bmp_tmp);
            writeBitmap(bmp_tmp, g);
            fclose(g);
        }
    }
    
    freeBitmap(bmp);
    free(bmp);
    bmp = NULL;

    freeBitmap(bmp_tmp);
    free(bmp_tmp);
    bmp_tmp = NULL;

    fclose(f);
    return 0;
}