//Daniel Woolnough, 02/03/18
//Implementation of arithmetic encoding using a stop digit to denote end of message
//some assumptions: Messages no longer than 1000 characters;
//      no punctuation, spaces, capitals, or non-alphabetic symbols inputted
// result printed to 50 decimal places (more may be needed for larger messages)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define MAX_MESSAGE_SIZE 1000

float * defineProb(void);
float encode(char *message, float *x, float upper, float lower);

int main (void) {

    float *x = defineProb(); //array of english language letter frequencies

    char *message = malloc(sizeof(char) * MAX_MESSAGE_SIZE);
    fgets(message, MAX_MESSAGE_SIZE, stdin);

    //partition defined by upper and lower bounds
    //start with entire probability distribution (0 to 1)
    float upper = 1;
    float lower = 0;
    float result = encode(message, x, upper, lower);

    printf("RESULT: %.50f\n", result);

    free(message);
    free(x);
    return result;
}

float *defineProb(void) {
    float *x = malloc(sizeof(int) * 27);//extra 'letter' to denote end of message (stop symbol)
    x[0] = 0.081670000;
    x[1] = 0.014920000;
    x[2] = 0.027820000;
    x[3] = 0.042530000;
    x[4] = 0.127020000;
    x[5] = 0.022280000;
    x[6] = 0.020150000;
    x[7] = 0.060940000;
    x[8] = 0.069660000;
    x[9] = 0.001530000;
    x[10] = 0.007720000;
    x[11] = 0.040250000;
    x[12] = 0.024060000;
    x[13] = 0.067490000;
    x[14] = 0.075070000;
    x[15] = 0.019290000;
    x[16] = 0.000950000;
    x[17] = 0.059870000;
    x[18] = 0.063270000;
    x[19] = 0.090560000;
    x[20] = 0.027580000;
    x[21] = 0.009780000;
    x[22] = 0.023600000;
    x[23] = 0.001500000;
    x[24] = 0.019740000;
    x[25] = 0.000740000;
    x[26] = 0.000010000;
    return x;
}

float encode(char *message, float *x, float upper, float lower){
    printf("\n\n\nUsing upper: %.20f and lower:%.20f\n\n\n", upper, lower);

    //cumulative frequencies for this partition
    float *cmFreq = malloc(sizeof(float) * 28);
    for(int i = 0; i < 28; i++) {
        if(i == 0) cmFreq[i] = lower;
        else cmFreq[i] = cmFreq[i - 1] + x[i - 1] * (upper - lower);
        printf("for %d and %c: %.20f\n", i,  message[0], cmFreq[i]);
    }

    //end of message, add a stop symbol and return.
    if(message == NULL || message[0] == '\0' || !isalpha(message[0])) {
        float result = (cmFreq[26] + cmFreq[27])/2;
        free(cmFreq);
        return result;
    }

    //else, recursively call in next partition
    int index = message[0] - 'a';
    float result =  encode(&message[1], x, cmFreq[index + 1], cmFreq[index]);
    free(cmFreq);
    return result;
}
