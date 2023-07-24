#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct fileInfo{
    FILE *fp;
    //char type[3];
    unsigned int width;
    unsigned int height;
    unsigned int size;
}fInfo;

void changeContrast(int change);
void usage();