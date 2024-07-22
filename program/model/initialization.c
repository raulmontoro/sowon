typedef struct State {
    
    Mode mode;
    float hours;
    float minutes;
    float seconds;
    int pause;
    size_t wiggle_index;


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


void initialState(Arguments arguments) {


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
