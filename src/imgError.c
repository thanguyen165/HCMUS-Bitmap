#include"imgError.h"

/*
 * print errors to terminal
 */
void printErrors(int8_t status) {
    switch (status) {
        case CANNOT_OPEN_FILE:
            printf("Cannot open file\n");
            break;
        case READ_FAIL:
            printf("Read file unsuccessfully\n");
            break;
        case BAD_ALLOC:
            printf("Bad alloc\n");
            break;
        case SUCCESSFULLY:
            break;
    }
    return;
}

/*
 * Alert to stdout if `FILE *f` is not opened successfully
 */
int8_t isOpened(FILE *f) {
    if (f == NULL) {
        printErrors(CANNOT_OPEN_FILE);
        return CANNOT_OPEN_FILE;
    }
    return SUCCESSFULLY;
}

/*
 * Check if Allocated successfully
 */
int8_t isAllocated(void *p) {
    if (p == NULL) {
        printErrors(BAD_ALLOC);
        return BAD_ALLOC;
    }
    return SUCCESSFULLY;
}