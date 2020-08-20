#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc==2){
        
        char password[5], salt[2];
        strncpy(salt,argv[1],2);
        char dict[]={'\0','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

        for(int i =1; i<52; i++){
            password[0]=dict[i];
            for(int j =0; j<52; j++){
                password[1]=dict[j];
                for(int k =0; k<52; k++){
                    password[2]=dict[k];
                    for(int l =0; l<52; l++){
                        password[3]=dict[l];
                        for(int m =0; m<52; m++){
                            password[4]=dict[m];
                            //printf("%s \n", password);
                            if(strcmp(argv[1], crypt(password, salt)) == 0){
                                printf("%s \n", password);
                                return 0;
                            }
                        }
                    }        
                }
            }
        }
    }else{ 
        printf("FATAL ERROR: The keys to the Kingdom are safely stored away from prying eyes, try again dumbo!\nUsage: ./crack hash\n");
     return 1;
    }
    printf("FATAL ERROR: unable to crack password\n");
    return 1;

}
