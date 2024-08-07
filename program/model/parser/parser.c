#include "error.h"

/*  strcmp()
*/
#include <string.h>


/* strtof()
*/
#include <stdlib.h>


int isNumber(char c) {
    return '0' <= c <= '9';
}

int buildInteger(char * c) {
    int i = 0;
    int s = sizeString(c);
    while (*c != '\0') {
        i += *c-'0'*power10(s);
        --s;
    }
    
}

int isChararcter(char c) {
    return 'a' <= c <= 'z' ||
           'A' <= c <= 'Z';
}

int isUppercase(char c) {
    return 'A' <= c <= 'Z';
}

int sizeString(char *c) {
    int s = 0;

    char *a = c;
    while (*a != '\0') {
        ++s;
        ++a;
    }
    return s;
}

int compareStringsB(char *a, char *b, int s) {
    
    int s2 = sizeString(*b);
    int b = s == s2;

    int i = 0;
    while(b && i<s) {
        b = *a == *b;
        ++a;
        ++i;
    }
}

int compareFlags(char *a, Arguments *b) {

    int s = sizeString(*argv);

    b->flag_m = compareStringsB(a, "-m", s);
    
    b->flag_t = compareStringsB(a, "-t", s);
    
}

int compareFlagsValues () {
    if (compareStringsB(a, "clock", s)) {
    }

    if (compareStringsB(a, "stopwatch", s)) {
    }

    if (compareStringsB(a, "countdown", s)) {
    }
}

void defaultConfig(Argument *a) {
    a->flag_m = 0;
    a->flag_t = 0;
    a->hours = 0;
    a->minutes = 0;
    a->seconds = 0;
    a->mode = clock;
}


void parser(int argc, char **argv, Arguments *a) {
    if (argc == 1) {
        defaultConfig(a);
    }
    else {
        for (int i=1; i<argc; ++i) {
            if (*argv == '-') {
                compareFlags(*argv, a);
            }
            else if (isNumber(*argv)) {
                int i = buildInteger(*argv);
            }
        }
    }
}
