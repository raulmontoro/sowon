int main() {
    const char *p1 = "../resource/filenameA.txt";
    createFile(p1);
    writeToFile(p, "Hello World!\n");
    appendToFile(p, "Bye World!\n");

    char t[100];
    readFile(p, t, 100);


    // const char *p2 = "../resource/filenameB.txt";
    // createFile(p2);
    // writeToFileFormat(p2, "hello world");
}
