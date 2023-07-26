#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct fileInfo{
    FILE *fp;
    //char type[3];
    unsigned int width;//width of image
    unsigned int height;//length of image
    unsigned int offset;//offset value for pixel data
    unsigned int location;// current fp location
}fInfo;

void changeContrast(int change);
void usage();