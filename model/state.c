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


/*  pre:    
    post:   cartesian coordinates
            position where rendering starts 
            to fit CHAR_COUNT characters 
            at user_scale*fit_scale scale
*/
void initial_pen(int w, 
                 int h,
                 float user_scale,
                 float fit_scale, 
                 int *pen_x,
                 int *pen_y) {
    
    // character width after scaling 
    const int effective_digit_width = (int)floorf(
                                            (float)CHAR_WIDTH*user_scale*fit_scale
                                      );
    // character height after scaling
    const int effective_digit_height = (int)floorf(
                                            (float)CHAR_HEIGHT*user_scale*fit_scale
                                       );
    
    // position where rendering starts 
    *pen_x = w/2 - effective_digit_width*CHARS_COUNT/2;
    *pen_y = h/2 - effective_digit_height/2;
}





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



/*  UPDATE CONFIGURATION STATE  */

/*  pre:
    post: window resize adjusted
*/
void fitScale(int w, int h, float *fit_scale) {

    *fit_scale = 1.0;

    // width/height ratio
    float text_aspect_ratio = (float) TEXT_WIDTH / (float) TEXT_HEIGHT;
    float window_aspect_ratio = (float) w / (float) h;

    if(text_aspect_ratio > window_aspect_ratio) {
        *fit_scale = (float) w / (float) TEXT_WIDTH;
    } else {
        *fit_scale = (float) h / (float) TEXT_HEIGHT;
    }
}







/*  paused  */

void pauseToggle(State *state) {
    state->paused = !state->paused;
}

/*  zoom   */

void zoomInitial(State *state) {
    state->user_scale = 1.0f;
}

/* pre:
   post: zoom in, represente in 'user_scale'
*/
void zoomIn(State *state) {
    state->user_scale += SCALE_FACTOR*state->user_scale;
}

void zoomOut(State *state) {
    state->user_scale -= SCALE_FACTOR*state->user_scale;
}
