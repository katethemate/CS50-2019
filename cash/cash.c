#include <stdio.h>
#include <cs50.h>

int main(void)
{
    float cash;
                   // store cash value 
    int coins;


    while (true) 
    {
        // take input the amount of cash owned
        printf("O hai! How much change is owed?\n");
        cash = get_float("");
            
        if (cash > 0) 
        {
            break;
        }
        
    }
    //printf("%f\n",cash);
    // convert decimal to int
    int cents = cash * 100;
    //printf("%i\n", cents);
    // count the number of coins
    coins = 0;
    
    
    while(cents>0){
        if(cents>=25){
            coins += cents/25;
            cents = cents%25;
        }else if(cents>=10){
            coins += cents/10;
            cents = cents%10;
        }else if(cents>=5){
            coins += cents/25;
            cents = cents%25;
        }else{
            coins += cents;
            cents = 0;
        }
    
    }
    

    /*
    
    
    // no of 25 cents coin
    while ( cents >= 25 ) 
    {
           coins += 1;
           cents = cents - 25;
    }

    // no of 10 cents coin
    while ( cents >= 10 ) 
    {
           cents = cents - 10;
           coins += 1;
    }
   
    // no of 5 cents coin
    while ( cents >= 5 ) 
    {
           cents = cents - 5;
           coins += 1;
    }*/

    printf("%i\n", coins);

    return 0;
}
