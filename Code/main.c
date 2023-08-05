/*
*Group 3
*Tyler Jobe
*Tyler Chanes
*Brandon Evins
*/
#include "main.h"

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
        perror("ERROR: could not open first file\n");
        usage();
    }

    f2.fp=fopen(argv[2], "rb");
    if(f2.fp==NULL){
        perror("ERROR: could not open second file\n");
        usage();
    }

    if(argc<4){
        f3.fp=fopen("hidden.bmp", "wb+");
    }else{
        f3.fp=fopen(argv[3], "wb+");
    }
    if(f3.fp==NULL){
        perror("ERROR: could not open third file\n");
        usage();
    }
    
    /*if(argv[4]=="-g"){
        f3.flag=1;
    }else if(argv[4]=="-w"){
        f3.flag=0;
    }else{
        perror("ERROR: no flag specified\n");
        usage();
    }*/

    //creats copy of cover to hide data in
    while((c=getc(f1.fp))!=EOF){
        putc(c, f3.fp);
        
    }
    rewind(f1.fp);
    rewind(f3.fp);
    
    headerInfo(&f3, 3);
    headerInfo(&f2, 2);

    if(f2.width>f3.width||f2.height>f3.height){
        perror("ERROR: trying to hide message file that is bigger than cover\n");
        usage();
    }

    //if(f3.flag==0){
        convert(&f3);//converts to black/white
        rewind(f3.fp);
        rewind(f2.fp);
        hide(&f3, f2);
    // }else{//could not get to work in time
    //     convert_g(&f3, 0.2f);//grayscale hiding
    //     rewind(f3.fp);
    //     rewind(f2.fp);
    //     hide_g(&f3, f2);
    // }

    rewind(f1.fp); //rewinds pointer
    rewind(f2.fp);
    rewind(f3.fp);
    fclose(f1.fp);
    fclose(f2.fp);
    fclose(f3.fp);
    usage();
    return 0;
}

void usage(){
    printf("usage:\\main.exe [cover image] [message image] [output image]\n");// [flag]\n-g:grayscale\n-w:black and white\n");
    exit(-1);
}

void headerInfo(fInfo* f, int fileNum){
    char type[3], hex_str[9];
    int c, i;
    unsigned char dim[5];

    printf("Getting header info for file %d\n", fileNum);//progress

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

    printf("Converting to Black/White\n");//progress
    
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

void convert_g(fInfo* gc, float factor){
    int temp=0, target=0, c, i;
    float try;

    printf("Reducing contrast\n");//progress
    
    target = gc->offset - gc->location;//target location

    //goes to target location
    for(i=0;i<target;i++){
        c=fgetc(gc->fp);
    }

    //edits contrast
    while((c=getc(gc->fp))!=EOF){
        try=(float)c/255.0f;//convert to float range [0,1]
        try=0.5f + factor * (try-0.5f);//reduce contrast by factor, centered around 0 (hence +/-0.5f)
        temp=(unsigned char) (try*255.0f);//puts back into original range

        fseek(gc->fp, -1, SEEK_CUR);//moves file pointer back 1
        fputc(temp, gc->fp);//places new value
        fseek(gc->fp, 0, SEEK_CUR);//moves fp to current
    }
}

void hide(fInfo* fa, fInfo message){
    int c, d, temp, i, wide, high, countW=0, countH=0;

    printf("Hiding\n");//progress

    //gets width difference
    wide=fa->width-message.width;

    //gets height difference
    high=fa->height-message.height;

    //goes to respective offsets
    for(i=0;i<fa->offset;i++){
        c=fgetc(fa->fp);
    }
    for(i=0;i<message.offset;i++){
        c=fgetc(message.fp);
    }

    //hides information
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
        //logic for if message image is smaller
        countW++;
        if(countW>=message.width){
            for(i=0;i<wide;i++){
                c=getc(fa->fp);
            }
            countW=0;
            countH++;
        }
        if(countH>=message.height){
            break;
        }
    }
}

//test for hiding in gray
void hide_g(fInfo* ga, fInfo message_g){
    int c, d, temp, i, wide, high, countW=0, countH=0;

    //gets width difference
    wide=ga->width-message_g.width;

    //gets height difference
    high=ga->height-message_g.height;

    //goes to respective offsets
    for(i=0;i<ga->offset;i++){
        c=fgetc(ga->fp);
    }
    for(i=0;i<message_g.offset;i++){
        c=fgetc(message_g.fp);
    }

    //hides information does not currently work
    while((c=getc(ga->fp))!=EOF){
        d=fgetc(message_g.fp);
        temp=c%32;
        if(d>127){
            temp-=temp;
        }else{
            temp+=temp;
        }

        fseek(ga->fp, -1, SEEK_CUR);
        fputc(temp, ga->fp);
        fseek(ga->fp, 0, SEEK_CUR);
        
    }
}