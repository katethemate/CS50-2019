#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[]){
   // if(argc >2 || argc<2)
    if(argc!=2)
    {
        printf("error");
        return 1;
    }

    int k = atoi(argv[1]);
    string p = get_string("plaintext:");
    printf("cyphertext: ");

    for (int i = 0, n = strlen(p); i < n; i++)
    {
        if(isalpha(p[i]))
        {
            if(isupper(p[i]))
            {
                printf("%c", (p[i]+k-'A')%26+'A');
            }
            else if(islower(p[i]))
            {
                printf("%c", (p[i]+k-'a')%26+'a');
            }
        }
        else
        {
            printf("%c", p[i]);
        }


    }
    printf("\n");



    return 0;
}