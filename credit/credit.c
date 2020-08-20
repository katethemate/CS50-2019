#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    int key = atoi(argv[1]);
    printf("%i \n", key);
   
    if (argc==2){
        string plaintext = get_string("plaintext: ");
        printf("ciphertext: ");
        for(int i=0, s = strlen(plaintext); i<s; i++){
            if (isupper(plaintext[i])){
                 printf("%c", plaintext[i] + (key % 26) +'a'-'a');
            }else{
                 printf("%c", plaintext[i] + (key % 26));
            }
        }
        printf("\n");
        
    }else{
        printf("Usage: ./caesar key\n");
    }
    
}

