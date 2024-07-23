void updateTitle(char *title1, 
                 int size1, 
                 char timestr[9], 
                 State state) {

        /*  print time as window's title */
        char title2[size1];

        /*  title construction */
        snprintf(title2, 
                 size1, 
                 "sowon - %d%d:%d%d:%d%d", 
                 timestr[0], timestr[1], timestr[3], timestr[4], timestr[6], timestr[7]);

        /*  store old title, 
            after new title has been rendered */
        memcpy(title1, title2, size1);

        if (strcmp(state.title1, title2) != 0) {
            windowTitle(title2);
        }
}
