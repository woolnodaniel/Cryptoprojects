#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_WORDS 267753
#define MAX_WORD_SIZE 20
#define MAX_MESSAGE_SIZE 1000

#define COLOR_RED     "\x1b[91m"
#define COLOR_GREEN   "\x1b[92m"
#define COLOR_CYAN    "\x1b[96m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLUE    "\x1b[94m"

int mod(int i, int j);
void decrypt(char *copy, char *key, int n);
void checkAnswer(char *copy, char *key);//, char **dictionary);

int main (int argc, char *argv[]) {

    char *message;
    char *key = NULL;
    if(argc == 1) {
        printf("Enter message to be decrypted: ");
        message = malloc(sizeof(char) * MAX_MESSAGE_SIZE);
        fgets(message, MAX_MESSAGE_SIZE, stdin);
        message[strlen(message) - 1] = '\0';
    } else {
        message = argv[1];
        if (argc > 2) key = argv[2];
    }

    if(key != NULL) {
        int n = strlen(key) - 2;
        decrypt(message, key, n);
        printf("Decryption with key %s: "COLOR_RED"%s\n"COLOR_RESET, key, message);
        return 0;
    }

    printf("Possible Matches for "COLOR_GREEN"%s:\n\n"COLOR_RESET, message);
    int i = 2;
    key = malloc(sizeof(char) * MAX_WORD_SIZE);
    while (i < MAX_WORD_SIZE) {
        FILE *dictionary1 = fopen("sowpods", "r");
        printf("Using keys of size "COLOR_CYAN"%d"COLOR_RESET":\n", i);
        while(fgets(key, MAX_WORD_SIZE, dictionary1) != NULL) {
            char *copy = strdup(message);
            int n = strlen(key) - 2;
            key[n+1] = '\0';
            if(n != i) {
                free(copy);
                continue;
            }

            printf("Trying key %s...\n",key);

            decrypt(copy, key, n);

            // printf("-->Decryption was %s\n", copy);
            checkAnswer(copy, key);//, dictionary);

            free(copy);
        }
        i++;
        fclose(dictionary1);
    }
    free(key);
    return EXIT_SUCCESS;
}

void decrypt(char *copy, char *key, int n) {
    int size = strlen(copy);
    int i = 0, j = 0;
    while(i < n) {
        j = i;
        while(j < size) {
            copy[j] = mod(copy[j] - key[i], 26)+'a';
            // printf("-->After modification with %c: %s", key[i], copy);
            j += n;
        }
        i++;
    }
}

int mod(int i, int j) {
    if(i < j && i >= 0) return i;
    else if(i >= j) return mod(i-j, j);
    else return mod(i+j, j);
}

void checkAnswer(char *copy, char *key) {//}, char **dictionary) {
    FILE *dictionary2 = fopen("sowpods", "r");
    char *word = malloc(sizeof(char) * MAX_WORD_SIZE);
    int count = 0;
    while(fgets(word, MAX_WORD_SIZE, dictionary2)) {
        word[strlen(word)-2] = '\0';
        if(strlen(word) < 7) continue;
        // printf("------>Searching for %s...\n", word);
        if(strstr(copy, word) != NULL) {
            // printf("----------------->Found it!\n");
            count++;
            printf("-->Found %s\n", word);
            // getchar();
        }
        // else printf("No %s\n", word);
    }
    if(count > 0) {
        printf("      "COLOR_RED"%s"COLOR_RESET" with key "COLOR_RED"%s"COLOR_RESET" and count "COLOR_RED"%d"COLOR_RESET"\a\n", copy, key, count);
        getchar();
    }
    free(word);
    fclose(dictionary2);
}
