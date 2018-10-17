#include <stdio.h>
#include <cs50.h>

int main(void){

    int n;
    do
    { n = get_int("height: ");
    }
    while (n<0 || n>24);


   //loop for level
    for(int i=0; i<n; i++){

        //print spaces
        for(int k = n; k>i+1; k--){
         printf  (" ");
        }

        //print hashtags
        for(int j=0; j<i+1 ; j++){
            printf ("#");
        }

        printf("\n");

    }
}
