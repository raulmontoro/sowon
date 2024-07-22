/*  PROGRAM NAME:
    -------------
        sowon

    DESCRIPTION:    
    -------------
        features
            clock
            stopwatch
            countdown

        features 
            start/stop/reset/restart
                increase 
                    time numeric values
                decrease
            pause/resume
            zoom in/out
            full screen
                on/off
            quit

    USAGE
    -------------
        $ sowon
            -m
                clock
                stopwatch
                countdown

            -t
                h
                m
                s
*/


















/*  strtof()
    https://cplusplus.com/reference/cstdlib/strtof/
*/
#include <stdlib.h>



int main(int argc, char **argv) {
    
    /* parsing */
    Arguments arguments;
    parseMainArguments(argc, argv, &arguments);
    
    /* initial state */
    initialization(arguments);

    /*  loop */    
    frameLoop(&initstate);
    
    return 0;
}
