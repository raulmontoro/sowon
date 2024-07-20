typedef struct State {
    Mode mode;
    float hours;
    float minutes;
    float seconds;
    int pause;
    
    





    size_t wiggle_index;
    float wiggle_cooldown;

    char prev_title[TITLE_CAP];

    /* current window width */
    int w;

    /* current window height */
    int h;

    float fit_scale;
    float zoomscale;
    
    /* cartesian coordinates of next digit */
    int pen_x;
    int pen_y;
    int fullscreen;

} State;


void initialState(Arguments arguments, State *state) {

    // default initial state for stopwatch mode when no arguments 
    *state = (State){MODE_CLOCK,
                     0.0f, 
                     0.0f,
                     0.0f, 
                     WIGGLE_DURATION, 
                     "hello world",
                     TEXT_WIDTH,
                     TEXT_HEIGHT,
                     1.0f, 
                     1.0f,
                     0,
                     0,
                     0};

    state->mode = arguments.mode;
    state->hours = arguments.hours;
    state->minutes = arguments.minutes;
    state->seconds = arguments.seconds;
    state->pause = 1;
    

    switch(state->mode) {
        case MODE_CLOCK:
            break;

        case MODE_STOPWATCH:
            break;

        case MODE_COUNTDOWN:
            break;

    }
}
















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
    State initstate;
    initialState(arguments, &initstate);

    /*  loop */    
    frameLoop(&initstate);
    
    return 0;
}
