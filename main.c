#include "main.h"
//#include <opencv>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>

int main(int argc, char * argv[]){
    //printf("Hello world\n");
    int c, i=0;
    FILE *fp=NULL;
    FILE *fp2=NULL;

    //if (argc < 3){
    //    usage();
    //}
    
    fp=fopen(argv[1], "r");
    for(i=0;i<4;i++){
        c=fgetc(fp);
        //printf("%c", c); //reads ascii of file
        printf("%X", c); //reads hex values of file
    }
    rewind(fp); //rewinds pointer
    fclose(fp);
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