//Daniel Woolnough, 01/05/18
//Implementation of arithmetic encoding using a stop digit to denote end of message
//Breaks the message into the given number of pieces
//some assumptions:
//      results need no more than 100 decimal places;
// results printed to 100 decimal places (more may be needed for larger messages)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define COLOR_RED     "\x1b[91m"
#define COLOR_GREEN   "\x1b[92m"
#define COLOR_CYAN    "\x1b[96m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLUE    "\x1b[94m"

#define LINE    putchar('\n');
#define ENCODE_ERROR fprintf(stderr, COLOR_CYAN"\nEncode Usage:\n"COLOR_RESET" ./ar_encode "COLOR_RED"-e"COLOR_GREEN" \"message\""COLOR_RESET" #pieces\n\n");
#define DECODE_ERROR fprintf(stderr, COLOR_CYAN"\nDecode Usage:\n"COLOR_RESET" ./ar_encode "COLOR_RED"-d"COLOR_RESET" #pieces "COLOR_GREEN"float[1] float[2] ... float[pieces]\n\n"COLOR_RESET);
#define INVALID_ERROR fprintf(stderr, COLOR_RED"\n\"%s\""COLOR_RESET" is not a valid message (alphabetic characters only, no spaces), or"COLOR_RED" %s"COLOR_RESET" is not a valid integer \n\n", argv[2], argv[3]);
#define INVALID_ERROR_2 fprintf(stderr, COLOR_RED"\"%s\""COLOR_RESET" is not a valid float (numeric characters or decimal point only), or"COLOR_RED" %s"COLOR_RESET" is not a valid integer \n\n", message, pieces);
#define ARGS_ERROR fprintf(stderr, "\nNumber of inputs (%d) does not equal %d\n\n", argc-3, atoi(argv[2]));



#define MAX_NUM_DECIMALS 100

float * defineProb(void);
float * defineCmFreq(float *x, float upper, float lower);
float encode(char *message, float *x, float upper, float lower);
void decode(float encryption, float *x, float upper, float lower);
char *removeTrail0(float number);
void checkString(char *message, char *pieces);

int main (int argc, char *argv[]) {

    if(argc == 1 || (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0)) {
        ENCODE_ERROR;
        DECODE_ERROR;
        exit(0);
    }

    float *x = defineProb(); //array of english language letter frequencies

    //partition defined by upper and lower bounds
    //start with entire probability distribution (0 to 1)
    float upper = 1;
    float lower = 0;
    float result;
    if(strcmp(argv[1], "-e") == 0) {
        if(argc != 4) {
            ENCODE_ERROR;
            exit(0);
        }
        char *message = argv[2];
        int pieces = atoi(argv[3]);
        if(pieces <= 0) {
            INVALID_ERROR;
            exit(0);
        }
        char **messages = malloc(sizeof(char *) * pieces);
        for(int j = 0; j < pieces; j++) {
            int length = strlen(message)/pieces+1;
            messages[j] = malloc(sizeof(char) * length);
            for(int i = j; i < strlen(message); i+=pieces) {
                if(!isalpha(message[i])) {
                    INVALID_ERROR;
                    exit(0);
                }
                messages[j][(i-j)/pieces] = message[i];
            }
            result = encode(messages[j], x, upper, lower);
            char *strResult = removeTrail0(result);
            printf("Piece %d (\"%s\"): %s\n", j+1, messages[j], strResult);
            free(strResult);
        }
    } else {
        if(argc < 4) {
            DECODE_ERROR;
            exit(0);
        }
        int pieces = atoi(argv[2]);
        if(argc-3 != pieces) {
            ARGS_ERROR;
            exit(0);
        }
        LINE;
        for(int i = 0; i < pieces; i++) {
            checkString(argv[i+3], argv[2]);
            float encryption = strtof(argv[i+3], NULL);
            printf("RESULT: ");
            decode(encryption, x, upper, lower);
            LINE;
        }
        LINE;
    }

    free(x);
    return EXIT_SUCCESS;
}

//create and return array of english letter frequencies
//source:
//https://en.wikipedia.org/wiki/Letter_frequency#Relative_frequencies_of_letters_in_the_English_language
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

float * defineCmFreq(float *x, float upper, float lower) {
    float *cmFreq = malloc(sizeof(float) * 28);//27 partitions = 28 values
    for(int i = 0; i < 28; i++) {
        if(i == 0) cmFreq[i] = lower;
        else if (i == 27) cmFreq[i] = upper;
        else cmFreq[i] = cmFreq[i - 1] + x[i - 1] * (upper - lower);

        //include this line if you wish to see each subinterval
        // printf("for %d and %c: %.20f\n", i,  message[0], cmFreq[i]);
    }
    return cmFreq;
}

//perform the encoding
//implemented recursively
//returns the final result as a float
float encode(char *message, float *x, float upper, float lower){

    //cumulative frequencies for this partition
    float *cmFreq = defineCmFreq(x, upper, lower);

    float result;

    //end of message, add a stop symbol and return.
    if(message == NULL || message[0] == '\0') {
        result = (cmFreq[26] + cmFreq[27])/2;
        free(cmFreq);
        return result;
    }

    //else, recursively call in next partition
    int i = 0;
    while(!isalpha(message[i]) && message[i] != '\0') i++;
    // printf("I is %d\n", i);
    if(message[i] != '\0'){
        int index = tolower(message[i]) - 'a';
        result =  encode(&message[i+1], x, cmFreq[index + 1], cmFreq[index]);
    } else {
        result = (cmFreq[26] + cmFreq[27])/2;
    }
    free(cmFreq);
    return result;
}

void decode(float encryption, float *x, float upper, float lower) {

    // printf("Between %.20f and %.20f\n\n", lower, upper);

    float *cmFreq = defineCmFreq(x, upper, lower);
    int i = 0;
    for(; encryption > cmFreq[i + 1]; i++);
    if(i == 26) return;
    printf("%c", i + 'a');
    decode(encryption, x, cmFreq[i + 1], cmFreq[i]);

}

char *removeTrail0(float number) {
    char *result = malloc(sizeof(char) * MAX_NUM_DECIMALS);
    sprintf(result, "%.100g", number);

    char *p = strchr(result, '.'); //locate decimal point
    while(*p != '\0') {
        p++;
    }
    *p-- = '\0';
    while(*p == '0') {
        *p-- = '\0';
    }
    if(*p == '.') *p = '\0';
    return result;
}

void checkString(char *message, char *pieces) {
    int length = strlen(message);
    int dot = 0;
    for(int i = 0; i < length; i++) {
        if(isdigit(message[i])) {
            continue;
        } else if(message[i] == '.' && dot == 0) {
            dot = 1;
            continue;
        } else {
            INVALID_ERROR_2;
            exit(0);
        }
    }
}
