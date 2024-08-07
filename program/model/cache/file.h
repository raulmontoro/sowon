/*  text file 
    https://www.w3schools.com/c/c_files_read.php

    path    p
    read    r
    write   w
    append  a
*/


void createFile(const char *p);

void writeToFile(const char *p, const char *t);

/*  format specifier    */
void writeToFileFormat(const char *p, const char *t);

void appendToFile(const char *p, const char *t);

void readFile(const char *p, char *t, int s);
