/*  PROGRAM NAME:   SOWON
    DESCRIPTION:    
        features
            clock
            stopwatch
            countdown
        events
            pause
            resume
            reset
*/


#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdio.h>

/*  strtof()
    https://cplusplus.com/reference/cstdlib/strtof/
*/
#include <stdlib.h>

/*  https://cplusplus.com/reference/cstring
*/
#include <string.h>
#include <time.h>
#include <math.h>



int main(int argc, char **argv) {
    
    /* parsing */
    Arguments arguments;
    parseMainArguments(argc, argv, &arguments);
    
    /* initial state */
    State initstate;
    initialState(arguments, &initstate);

    /*  loop */    
    frameLoop(&initstate);
    
    return 0;
}
