#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

     if (argc !=2){
         fprintf(stderr, "Usage: ./recover image");
         return 1;
     }


    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }




    int images_created = 0;

    FILE *image = NULL;

    unsigned char block[512];

    // check if we've found a jpeg yet or not
    int jpg_found = 0; //false

    //repeat until end of card
    while (fread(block, 512, 1, file) == 1)
    {
        //start of a new jpeg?
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
        {

            if(jpg_found==0){
                jpg_found = 1;
            }else{
                fclose(image);
            }

            // create new file
            char filename[8];
            sprintf(filename, "%03i.jpg", images_created);
            image = fopen(filename, "w");
            images_created ++;

        }


        if(jpg_found==1){
            // write 512 bytes to file once we start finding jpgs
            fwrite(&block, 512, 1, image);
        }

    }

    fclose(file);

    printf("%i", jpg_found);

    return 0;

}
