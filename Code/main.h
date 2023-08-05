#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    FILE *fp;// file pointer
    //char type[3];
    unsigned int width;//width of image
    unsigned int height;//length of image
    unsigned int offset;//offset value for pixel data
    unsigned int location;// current fp location
}fInfo;

void changeContrast(int change);
void usage();
void headerInfo(fInfo* f);
void convert(fInfo* fc);
void hide(fInfo* fa, fInfo message);
void hide_g(fInfo* ga, fInfo message_g);