// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    float f = atoi(argv[1]);

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_r;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_r = bf;
    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_r;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_r = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    bi_r.biWidth = bi.biWidth * f;
    bi_r.biHeight = bi.biHeight *f;
    
    // Determine the old and new paddings
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) %4) % 4;
    int padding_r = (4 - (bi_r.biWidth * sizeof(RGBTRIPLE)) %4) % 4;
    
    bi_r.biSizeImage = (bi_r.biWidth * sizeof(RGBTRIPLE) + padding_r) * abs(bi_r.biHeight);
    bf_r.bfSize = bf.bfSize - bi.biSizeImage + bi_r.biSizeImage;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_r, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_r, sizeof(BITMAPINFOHEADER), 1, outptr);

   

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        //read each line f times
        for (int k=0; k<f; k++){
           
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                for (int l=0; l<f; l++){
                    
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // Add a new padding
            for (int l = 0; l < padding_r; l++){
                fputc(0x00, outptr);
            }
            
             // Return to the beginning of a scanline
            if (k < f - 1){
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            
            }
        }
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding_r; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
