#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// Determines whether a string represents a rest

int main(void){

//int duration(string fraction)
//{

string fraction= "3/8";
    int numerator = atoi(&fraction[0]);
    int denominator = atoi(&fraction[2]);
    if (denominator==8){
        return numerator;
    }else{
        int result= numerator*8/denominator;
        printf("%i", result);
        return result;
    }
//}

}
