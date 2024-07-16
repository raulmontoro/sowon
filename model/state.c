/*  STOPWATCH   
    TIMER/COUNTDOWN
    CLOCK
*/

typedef enum Mode {
    MODE_STOPWATCH,
    MODE_COUNTDOWN,
    MODE_CLOCK,
} Mode;

typedef struct State {
    Mode mode;
    float displaytime;
    float initialcountdownclock;
    int paused;
    int exit_count_down;
    
    size_t wiggle_index;
    float wiggle_cooldownARGUMENTS ;

    char prev_title[TITLE_CAP];

    /* current window width */
    int w;

    /* current window height */
    int h;

    float fit_scale;
    float user_scale;
    
    /* cartesian coordinates of next digit */
    int pen_x;
    int pen_y;
    int fullscreen;

} State;


void initialState(Arguments arguments, State *state) {

    // default initial state for stopwatch mode when no arguments 
    *state = (State){MODE_STOPWATCH,
                     0.0f, 
                     0.0f,
                     0, 
                     0, 
                     0, 
                     WIGGLE_DURATION, 
                     "hello world",
                     TEXT_WIDTH,
                     TEXT_HEIGHT,
                     1.0f, 
                     1.0f,
                     0,
                     0,
                     0};

    /*  flags   */
    state->mode = arguments.mode;
    state->paused = arguments.flag_p; 
    state->exit_count_down = arguments.flag_e; 
    

    /*  mode    */
    switch(state->mode) {
        case MODE_STOPWATCH:
            break;

        case MODE_COUNTDOWN:
            state->initialcountdownclock = arguments.initialcountdownclock;
            state->displaytime = state->initialcountdownclock; 
            break;

        case MODE_CLOCK:
            localTime(&state->displaytime);
            break;
    }
}



