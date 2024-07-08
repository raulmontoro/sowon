void timeTitle(char *prev_title, int prev_title_size, char timestr[9], State state) {

        /*  print time as window's title */
        char title[prev_title_size];

        /*  title construction */
        snprintf(title, prev_title_size, "sowon - %d%d:%d%d:%d%d", timestr[0], timestr[1], timestr[3], timestr[4], timestr[6], timestr[7]);

        if (strcmp(state.prev_title, title) != 0) {
            windowTitle(title);
        }

        /*  store old title, 
            after new title has been rendered */
        memcpy(prev_title, title, prev_title_size);
}
