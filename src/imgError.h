#ifndef IMG_ERROR_H
#define IMG_ERROR_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define SUCCESSFULLY 0
#define CANNOT_OPEN_FILE -1
#define READ_FAIL -2
#define BAD_ALLOC -3

void printErrors(int8_t status);

int8_t isOpened(FILE *f);

int8_t isAllocated(void *p);

#endif  //IMG_ERROR_H