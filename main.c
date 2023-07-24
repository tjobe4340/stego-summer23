#include "main.h"
//#include <opencv>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>

//struct fInfo f1;//cover image
//struct fInfo f2;//image to hide
//struct fInfo f3;// final product

int main(int argc, char * argv[]){
    //printf("Hello world\n");
    int c, i=0;
    unsigned int target;// target locatoin of pixel data
    fInfo f1;//cover image
    //FILE *fp=NULL;//cover image
    //FILE *fp2=NULL;//image to hide
    //FILE *fp3=NULL;//final product
    char type[3], string[255];
    unsigned char dim[5];
    char hex_str[9];//for converting numbers

    //if (argc < 3){
    //    usage();
    //}
    
    f1.fp=fopen(argv[1], "r");
    /*if(f1.fp==NULL){
        perror("ERROR: could not open first file");
        //usage();
    }*/

    fgets(type, 3, f1.fp);// file type
    type[2]='\0';
    f1.location=2;
    //puts(type);
    //printf("\n");
    /*for(i=0;i<2;i++){
        type[i]=fgetc(f1.fp);
        //printf("%c", c); //reads ascii of file
        printf("%c", type[i]); //reads hex values of file
    }
    memcpy(string, type, 2);
    string[3]='\0';
    */
    if(strcmp(type, "BM") != 0){
        //perror("ERROR: incorrect file type %s, needs BM\n", type[0]);
        //usage();
    }

    for(i=0;i<8;i++){
        c=fgetc(f1.fp);
        f1.location++;
    }
    //get offset
    for(i=3;i>=0;i--){
        dim[i]=fgetc(f1.fp);
        //printf("%c", c); //reads ascii of file
        //printf("%X", dim[i]); //reads hex values of file
        f1.location++;
    }
    dim[4]='\0';

    for (int i = 0; i < 4; i++) {
        sprintf(&hex_str[i * 2], "%02X", dim[i]);
    }
    //printf("The hexadecimal string is %s\n", hex_str);

    // Convert hex string to integer
    f1.offset = strtol(hex_str, NULL, 16);
    //printf("The decimal value is %lu\n", f1.offset);

    //get width
    for(i=0;i<4;i++){
        c=fgetc(f1.fp);
        //printf("%X", c); //reads hex values of file
        f1.location++;
    }
    for(i=3;i>=0;i--){
        dim[i]=fgetc(f1.fp);
        //printf("%c", c); //reads ascii of file
        //printf("%X", dim[i]); //reads hex values of file
        f1.location++;
    }
    dim[4]='\0';

    for (int i = 0; i < 4; i++) {
        sprintf(&hex_str[i * 2], "%02X", dim[i]);
    }
    //printf("The hexadecimal string is %s\n", hex_str);

    // Convert hex string to integer
    f1.width = strtol(hex_str, NULL, 16);
    //printf("The decimal value is %lu\n", f1.width);

    //gets height
    for(i=3;i>=0;i--){
        dim[i]=fgetc(f1.fp);
        //printf("%c", c); //reads ascii of file
        //printf("%X", dim[i]); //reads hex values of file
        f1.location++;
    }
    dim[4]='\0';

    for (int i = 0; i < 4; i++) {
        sprintf(&hex_str[i * 2], "%02X", dim[i]);
    }
    //printf("The hexadecimal string is %s\n", hex_str);

    // Convert hex string to integer
    f1.height = strtol(hex_str, NULL, 16);
    //printf("The decimal value is %lu\n", f1.height);

    //printf("%u", f1.location);

    target = f1.offset - f1.location;
    //printf("%u", target);

    rewind(f1.fp); //rewinds pointer
    fclose(f1.fp);
    return 0;
    //for (i=0;i<argc;i++){
        /*switch (argv[i])
        {
            case '-hide':
            usage();
            break;

            case '-extract':
            //extract();
            break;

            case '-h':
            usage();
            break;

            //case "program.exe":
            //break;

            default:
            usage();

        }*/
        
    //}
    /*int choice=0;
    
    printf("input options: [1] hide [2] extract");
    scanf("%d", &choice);
    switch (choice){
        case 1:
        //do something
        break;

        case 2:
        //do something
        break;

        default:
        usage();
    }
    */
    changeContrast(255);
    return 0;
}

void changeContrast(int change){
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
    printf("variable change:%d\n", change);
    float f = (259*(change+255))/(255*(259-change));
    printf("variable f:%f\n", f);
}

void usage(){
    printf("usage:");
    exit;
}