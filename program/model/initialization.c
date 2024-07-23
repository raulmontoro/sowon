void initialState(Arguments *arguments) {
    /*  window 
            title   'title'
            current window width    'w'
            current window height   'h'
            window resize scale     'windowscale'
            zoom resize scale       'zoomscale'
    */

    char title[TITLE_CAP] = "Sowon";
    int w = TEXT_WIDTH;
    int h = TEXT_HEIGHT;;
    float windowscale = 1.0f;
    float zoomscale = 1.0f;;

    Mode mode = arguments->mode;
    int wiggle_index = 0;
    int pause = 1;

    
    /* cartesian coordinates of next digit */
    int pen_x = 0;
    int pen_y = 0;
    int fullscreen = 0;
    

    switch(state->mode) {
        case MODE_CLOCK:
            float initialclock_hours   = arguments->hours;
            float initialclock_minutes = arguments->minutes;
            float initialclock_seconds = arguments->seconds;
            break;

        case MODE_STOPWATCH:
            float initialstopwatch_hours;
            float initialstopwatch_minutes;
            float initialstopwatch_seconds;
            break;

        case MODE_COUNTDOWN:
            float initialcountdown_hours;
            float initialcountdown_minutes;
            float initialcountdown_seconds;
            break;

    }
}
