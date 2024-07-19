/*  https://www.w3schools.com/c/c_files_read.php
    path    p
    read    r
    write   w
    append  a
*/

#include <stdio.h>

void createFile(const char *p) {
    FILE *f;
    f = fopen(p, "w");
    fclose(f);
}

void writeToFile(const char *p, const char *t) {
    FILE *f;
    f = fopen(p, "w");
    fputs(t, f);
    fclose(f);
}

void writeToFileFormat(const char *p, const char *t) {
    FILE *f;
    f = fopen(p, "w");
    fprintf(f, "%s", t);
    fclose(f);
}

void appendToFile(const char *p, const char *t) {
    FILE *f;
    f = fopen(p, "a");
    fputs(t, f);
    fclose(f);
}

void appendToFileFormat(const char *p, const char *t) {
    FILE *f;
    f = fopen(p, "a");
    fprintf(f, "%s", t);
    fclose(f;
}
    
void readFile(const char *p, char *t, int s) {
    FILE *f;
    f = fopen(p, "r");

    /*  return NULL
    */
    while(fgets(t, s, f)) {
        printf("%s\n", t);
    }
    fclose(f);
}

int main() {
    const char *p = "../rsc/filename.txt";
    createFile(p);
    writeToFile(p, "Hello World!\n");
    appendToFile(p, "Bye World!\n");
    char t[100];
    readFile(p, t, 100);
}
