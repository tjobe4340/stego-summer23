/*
if pixel < 128 make 0, if > make 255
just make sure cover image is larger than message image
doesn't need to work perfectly
possible approach
    want hidden image to get darker/brighter slower than cover
maybe make nubers 127/128 or 128/129 if those don't work
makes our image come into focus as contrast is increased
message image all pixels are either 127/128
or pixel value if 128, clear(and) if 127
try with msb first
*/
#include "main.h"
//#include <opencv>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>

//struct fInfo f1;//cover image
//struct fInfo f2;//image to hide
//struct fInfo f3;// final product

int main(int argc, char * argv[]){
    int c, i=0;
    unsigned int target;// target location of pixel data
    fInfo f1;//cover image
    fInfo f2;//image to hide
    fInfo f3;// new image
    char type[3], string[255];
    unsigned char dim[5];
    char hex_str[9];//for converting numbers
    unsigned char test;

    if (argc < 3){
        usage();
    }
    
    f1.fp=fopen(argv[1], "rb");
    if(f1.fp==NULL){
        perror("ERROR: could not open first file");
        usage();
    }

    f2.fp=fopen(argv[2], "rb");
    if(f1.fp==NULL){
        perror("ERROR: could not open second file");
        usage();
    }

    f3.fp = fopen("test.bmp", "wb+");
    if(f3.fp==NULL){
        perror("ERROR: could not create file");
        usage();
    }

    //creats copy of cover to hide data in
    while((c=getc(f1.fp))!=EOF){
        putc(c, f3.fp);
        
    }
    rewind(f1.fp);
    rewind(f3.fp);
    
    headerInfo(&f3);
    headerInfo(&f2);

    convert(&f3);//converts to black/white

    rewind(f3.fp);
    rewind(f2.fp);

    hide(&f3, f2);

    rewind(f1.fp); //rewinds pointer
    rewind(f2.fp);
    rewind(f3.fp);
    fclose(f1.fp);
    fclose(f2.fp);
    fclose(f3.fp);
    return 0;
}

//void changeContrast(int change){
    /* f(x)= ax+b or f(x)= a(x-128)+128+b where a is contrast and b is brightness
    *
    * or F=(259(C+255))/(255(259-C)) stored as a float, and C is desired level of contrast
    * factor = (259 * (contrast + 255)) / (255 * (259 - contrast))
    * color = GetPixelColor(x, y) //should get color
    * newRed   = Truncate(factor * (Red(colour)   - 128) + 128) //ensure value is between 0 and 255
    * newGreen = Truncate(factor * (Green(colour) - 128) + 128) //ensure value is between 0 and 255
    * newBlue  = Truncate(factor * (Blue(colour)  - 128) + 128) //ensure value is between 0 and 255
    * PutPixelColor(x, y) = RGB(newRed, newGreen, newBlue)
    * R'=F(R-128)+128
    * 
    * we are only doing it on grey scale so much simpler
    * 
    */
//     printf("variable change:%d\n", change);
//     float f = (259*(change+255))/(255*(259-change));
//     printf("variable f:%f\n", f);
// }

void usage(){
    printf("usage:\\main.exe [cover image] [message image]\n");
    exit(-1);
}

void headerInfo(fInfo* f){
    char type[3], hex_str[9];
    int c, i;
    unsigned char dim[5];

    fgets(type, 3, f->fp);// file type
    type[2]='\0';
    f->location=2;
    if(strcmp(type, "BM") != 0){
        perror("ERROR: incorrect file type needs BM\n");
        usage();
    }
    for(i=0;i<8;i++){
        c=fgetc(f->fp);
        f->location++;
    }
    //get offset
    for(i=3;i>=0;i--){
        dim[i]=fgetc(f->fp);
        f->location++;
    }
    dim[4]='\0';

    for (int i = 0; i < 4; i++) {// for converting to hex
        sprintf(&hex_str[i * 2], "%02X", dim[i]);
    }
    // Convert hex string to integer
    f->offset = strtol(hex_str, NULL, 16);

    //get width
    for(i=0;i<4;i++){
        c=fgetc(f->fp);
        f->location++;
    }
    for(i=3;i>=0;i--){
        dim[i]=fgetc(f->fp);
        f->location++;
    }
    dim[4]='\0';

    for (int i = 0; i < 4; i++) {
        sprintf(&hex_str[i * 2], "%02X", dim[i]);
    }

    // Convert hex string to integer
    f->width = strtol(hex_str, NULL, 16);

    //gets height
    for(i=3;i>=0;i--){
        dim[i]=fgetc(f->fp);
        f->location++;
    }
    dim[4]='\0';

    for (int i = 0; i < 4; i++) {
        sprintf(&hex_str[i * 2], "%02X", dim[i]);
    }

    // Convert hex string to integer
    f->height = strtol(hex_str, NULL, 16);
}

void convert(fInfo* fc){
    int temp=0, target=0, c, i;
    
    target = fc->offset - fc->location;

    for(i=0;i<target;i++){
        c=fgetc(fc->fp);
    }
    while((c=getc(fc->fp))!=EOF){    
        if(c>127){
            temp=255;
            fseek(fc->fp, -1, SEEK_CUR);
            fputc(temp, fc->fp);
            fseek(fc->fp, 0, SEEK_CUR);
        }else{
            temp=0;
            fseek(fc->fp, -1, SEEK_CUR);
            fputc(temp, fc->fp);
            fseek(fc->fp, 0, SEEK_CUR);
        }
    }
}

void hide(fInfo* fa, fInfo message){
    int c, d, temp, i;
    
    for(i=0;i<fa->offset;i++){
        c=fgetc(fa->fp);
    }
    for(i=0;i<message.offset;i++){
        c=fgetc(message.fp);
    }
    while((c=getc(fa->fp))!=EOF){
        d=fgetc(message.fp);
        if(d>127){
            if(c==255){
                temp=240;
                fseek(fa->fp, -1, SEEK_CUR);
                fputc(temp, fa->fp);
                fseek(fa->fp, 0, SEEK_CUR);
            }else{
                temp=16;
                fseek(fa->fp, -1, SEEK_CUR);
                fputc(temp, fa->fp);
                fseek(fa->fp, 0, SEEK_CUR);
            }
        }
    }
}