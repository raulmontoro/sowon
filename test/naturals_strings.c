#include <stdio.h>

int isNatural(char *argv) {
    int a = 1;
    while(a != 0 && *argv != '\0') {
        a = *argv >= '0' && *argv <= '9';
        ++argv;
    }
    return a;
}

int isString(char *argv) {
    int r = 1;
    while(r != 0 && *argv != '\0') {
        r = (*argv >= 'A' && *argv <= 'Z') ||
            (*argv >= 'a' && *argv <= 'z');
        ++argv;
    }
    return r;
}

int main() {
    /*  notice the abscens fo whitespace */
    char *s = "helloworld";
    printf("%d\n", isString(s));
    return 0;
}

