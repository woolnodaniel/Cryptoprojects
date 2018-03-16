//Daniel Woolnough, 16/03/18
//Implementation of a frequency analyser
//Input: a string of text, passed in as command line argument
//Optional input: a -m flag to decrypt the message assuming a substitution
//  cipher, based upon the frequency analysis
//Output: An array of frequencies of each character
//some assumptions:
//      Message is entered with English alphabetic characters ;
//      Spaces are in their usual place for encrypted messages;

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#define COLOR_RED     "\x1b[91m"
#define COLOR_GREEN   "\x1b[92m"
#define COLOR_CYAN    "\x1b[96m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLUE    "\x1b[94m"
#define LINE    putchar('\n');

void returnFreq(char *message, int frequencies[]);
int *printFreq(int frequencies[]);
void findMatch(char *message, int frequencies[], int *lettersUsed);
int findmax(int array[]);
void rmv(int frequencies[], int index);

int main (int argc, char *argv[]) {

    if(argc == 1 || argc >= 4) {
        fprintf(stderr, COLOR_CYAN"\nUsage:"COLOR_RESET" ./frequencyAnalyser \"message\" flag\n"
                        COLOR_CYAN"Flag option"COLOR_RESET": -m to match frequencies with possible "
                        "decryption of substitution cipher\n\n");
        exit(0);
    }

    bool match = false;

    //check for optional flag
    if(argc == 3 && strcmp(argv[2], "-m") == 0) match = true;

    int frequencies[26] = {0};

    char *message = strdup(argv[1]);
    printf(COLOR_CYAN"\nYour message:"COLOR_RESET" %s\n", message);

    returnFreq(message, frequencies);
    int *lettersUsed = printFreq(frequencies);

    if(match) {
        findMatch(message, frequencies, lettersUsed);
    }

    free(message);

    return EXIT_SUCCESS;
}

void returnFreq(char *message, int frequencies[]) {
    int i = -1;
    char c;
    while (c != '\0') {
        i++;
        c = message[i];
        if(!isalpha(c)) continue;
        c = tolower(c);
        frequencies[c-'a']++;
    }
}

int *printFreq(int frequencies[]) {
    printf(COLOR_CYAN"\nFrequencies:\n"COLOR_RESET);
    int *lettersUsed = calloc(0, sizeof(int)*26);
    for(int i = 0; i < 26; i++) {
        if(frequencies[i] == 0) continue;
        printf("%3c", i+'a');
        lettersUsed[i] = 1;
    }
    LINE;

    for(int i = 0; i < 26; i++) {
        if(frequencies[i] == 0) continue;
        printf(COLOR_GREEN"%3d"COLOR_RESET, frequencies[i]);
    }
    LINE;
    LINE;
    return lettersUsed;
}

void findMatch(char *message, int frequencies[], int *lettersUsed) {

    //a very crude method of using the frequencies to decrypt

    printf(COLOR_CYAN"Possible Match:\n"COLOR_RESET);

    char englishLetters[26] = "etaoinshrdlcumwfgypbvkjxqz";
    char *decryption = malloc(sizeof(char) * 26);

    int highest = 0;

    for(int i = 0; i < 26; i++) {
        int index = findmax(frequencies);
        if(frequencies[index] == 0) break;
        decryption[index] = englishLetters[highest];
        highest++;
        frequencies[index] = 0;
    }

    highest = 0;
    while(message[highest] != '\0') {
        if(!isalpha(message[highest])) putchar(message[highest]);
        else printf("%c", decryption[(tolower(message[highest]) - 'a')]);
        highest++;
    }
    LINE;
}

int findmax(int array[]) {
    int max = 0;
    for(int i = 0; i < 26; i++) {
        if(array[i] > array[max]) {
            max = i;
        }
    }
    return max;
}

void rmv(int frequencies[], int index) {
    int i = index + 1;
    while(frequencies[index] != '\0') {
        frequencies[index] = frequencies[i];
        index++;
        i++;
    }
}
