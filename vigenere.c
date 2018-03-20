#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 20
#define MAX_MESSAGE_SIZE 1000

int mod(int i, int j);
void decrypt(char *copy, char *key, int n);
void checkAnswer(char *copy, char *key);

int main (int argc, char *argv[]) {

    char *message;
    if(argc == 1) {
        printf("Enter message to be decrypted: ");
        message = malloc(sizeof(char) * MAX_MESSAGE_SIZE);
        fgets(message, MAX_MESSAGE_SIZE, stdin);
        message[strlen(message) - 1] = '\0';
    } else {
        message = argv[1];
    }

    FILE *dictionary1 = fopen("sowpods", "r");

    printf("Possible Matches for %s:\n\n", message);

    char *key = malloc(sizeof(char) * MAX_WORD_SIZE);
    while(fgets(key, MAX_WORD_SIZE, dictionary1) != NULL) {

        char *copy = strdup(message);
        int n = strlen(key) - 2;
        key[n+1] = '\0';
        //I'm guessing the key size is five...
        if(n != 8) {
            free(copy);
            continue;
        }

        // printf("Trying key %s...\n",key);

        decrypt(copy, key, n);

        // printf("-->Decryption was %s\n", copy);
        checkAnswer(copy, key);

        free(copy);
    }

    fclose(dictionary1);
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

void checkAnswer(char *copy, char *key) {
    FILE *dictionary2 = fopen("sowpods", "r");
    char *word = malloc(sizeof(char) * MAX_WORD_SIZE);
    int count = 0;
    while(fgets(word, MAX_WORD_SIZE, dictionary2) != NULL) {
        word[strlen(word)-2] = '\0';
        if(strlen(word) <= 3) continue;
        // printf("------>Searching for %s...\n", word);
        if(strstr(copy, word) != NULL) {
            // printf("----------------->Found it!\n");
            count++;
            // printf("-->Found %s\n", word);
        }
        // else printf("No %s\n", word);
    }
    if(count > 9) printf("%s with key %s and count %d\n", copy, key, count);
    fclose(dictionary2);
}
