#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_WORDS 267753
#define MAX_WORD_SIZE 20
#define MAX_MESSAGE_SIZE 1000

#define COLOR_RED     "\x1b[91m"
#define COLOR_GREEN   "\x1b[92m"
#define COLOR_CYAN    "\x1b[96m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLUE    "\x1b[94m"

void decrypt(char *message, char *key, int n);
int mod(int i, int j);

int main (int argc, char *argv[]) {

    char *messages[5] = {"LpaGbbfctNiPvwdbjnPuqolhhtygWhEuafjlirfPxxl",
                       "WdafvnbcDymxeeulWOtpoofnilwngLhblUfecvqAxs",
                       "UijMltDjeumxUnbiKstvdrVhcoDasUlrvDypegublg",
                       "LpaAlrhGmjikgjdmLlcsnnYmIsoPcglaGtKeQcemiu",
                       "LpaDohqcOzVbglebjPdTnoTzbyRbuwGftflTliPiqp"};
    char *key = argv[1];
    for(int i = 0; i < 5; i++) {
        decrypt(messages[i], key, strlen(messages[i]));
    }
    return 0;
}

void decrypt(char *message, char *key, int n) {
    char *copy = strdup(message);
    int size = strlen(copy);
    int i = 0, j = 0;
    while(i < n) {
        j = i;
        while(j < size) {
            if(isupper(copy[j])) {
                copy[j] = mod(message[j] - toupper(key[i]), 26)+'A';
            } else {
                copy[j] = mod(message[j] - key[i], 26)+'a';
            }
            j += n;
        }
        i++;
    }
    printf("Before: "COLOR_CYAN"%50s "COLOR_RESET"After: "COLOR_RED"%50s\n"COLOR_RESET, message, copy);
}

int mod(int i, int j) {
    if(i < j && i >= 0) return i;
    else if(i >= j) return mod(i-j, j);
    else return mod(i+j, j);
}
