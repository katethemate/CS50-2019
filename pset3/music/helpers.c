// Helper functions for music

#include <cs50.h>
#include <math.h>
#include "helpers.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction){
    int numerator = atoi(&fraction[0]);
    int denominator = atoi(&fraction[2]);
    if (denominator==8){
        return numerator;
    }else{
        int result= numerator*8/denominator;
        return result;
    }
}

bool is_rest(string s){
    if(strcmp(s,"")){
        return false;
    }else{
        return true;
    }
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int octave = 0;
    char letter = note[0];
    int semitone = 0;

    if(strlen(note)==2){
        octave = atoi (&note[1]);
    }else{
        octave = atoi (&note[2]);
    }

    if (letter == 'C'){
        semitone = 3;
    }else if(letter == 'D'){
        semitone = 5;
    }else if(letter == 'E'){
        semitone = 7;
    }else if(letter == 'F'){
        semitone = 8;
    }else if(letter == 'G'){
        semitone = 10;
    }else if(letter == 'A'){
        semitone = 0;
    }else if(letter == 'H'){
        semitone = 2;
    }

    int number = 440 * pow(2.0,(octave-4));

    printf("%i", semitone);

    int result = number * pow(2.0,(semitone/12.0));

    if(note[1] == '#'){
        result = result +1;
    }else if(note[1] == 'b'){
        result = result -1;
    }

    return result;

}

