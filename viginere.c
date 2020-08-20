#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    
    if(argc==2){
        string keyword = argv[1];
        string plaintext = get_string("plaintext: ");
        int plainlen = strlen(plaintext);
        int keylen = strlen(argv[1]);
        int counter = 0;
        int letter;
        int keyletter;
       
        printf("ciphertext: ");
        for(int i=0; i<plainlen; i++){
            if(counter>keylen-1){
                counter=0;
            }
            letter = plaintext[i];
            keyletter = argv[1][counter];
               
            if(keyletter>96 && keyletter<123){
                if (letter>96 && letter<123){
                    letter = 97 + ((letter-97+ keyletter-97)%26);
                    printf("%c", letter);
                }else if(letter<91 && letter>64){
                    letter = 65 + ((letter-65+ keyletter-97)%26);
                    printf("%c", letter);
                }else{
                    printf(" ");
                    counter--;
                }
                
                
            }else if(keyletter<91 && keyletter>64){
                if (letter>96 && letter<123){
                    letter = 97 + ((letter-97+ keyletter-65)%26);
                    printf("%c", letter);
                }else if(letter<91 && letter>64){
                    letter = 65 + ((letter-65+ keyletter-65)%26);
                    printf("%c", letter);
                }else{
                    printf(" ");
                    counter--;
                }    
            }
            counter++;
            
        }
        printf("\n");
    }else{
        printf("Usage: ./vigenere keyword\n");
    }
}
