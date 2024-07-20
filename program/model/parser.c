/********** TIME PARSER **********/


float timeParser(const char *time, timeunits) {
    float result = 0.0f;
    
    /*  12h
        30m
        45s 
        12h30m45s
    */
    while (*time) {
        char *endptr = NULL;

        /*  string 
            to 
            float */
        float x = strtof(time, &endptr);

        if (time == endptr) {
            fprintf(stderr, "`%s` is not a number\n", time);
            exit(1);
        }

        time = endptr;
        if (*time) 
            time += 1;
    }

    return result;
}









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









/********** MAIN ARGUMENTS PARSER **********/


typedef enum Mode {
    MODE_CLOCK,
    MODE_STOPWATCH,
    MODE_COUNTDOWN,
} Mode;

typedef struct Arguments {
    int flag_m;
    int flag_t;
    int hours;
    int minutes;
    int seconds;
    Mode mode;
} Arguments;


/* argument parser  */
void mainParser(int argc,
                char **argv,
                Arguments *arguments) {
    
    /*  default when no arguments in main */
    *arguments =(Arguments){MODE_STOPWATCH,
                            0,
                            0,
                            0,
                            0,
                            0};

    /* at least one argument */ 
    for (int i = 1; i < argc; ++i) {

            if (strcmp(argv[i], "-p") == 0) {
                arguments->flag_p = 1;
            } 
            else if (strcmp(argv[i], "-e") == 0) {
                arguments->flag_e = 1;
            }
            else if (strcmp(argv[i], "stopwatch") == 0) {
                arguments->mode = MODE_STOPWATCH;
                arguments->initialstopwatchclock = parse_time(argv[i+1]);
                ++i;
            }
            else if (strcmp(argv[i], "countdown") == 0) {
                arguments->mode = MODE_COUNTDOWN;
                arguments->initialcountdownclock= parse_time(argv[i+1]);
                ++i;
            }
            // time clock
            else if (strcmp(argv[i], "clock") == 0) {
                arguments->mode = MODE_CLOCK;
            }
            else {
                char *endptr = NULL;
                float time = 0.0f;
                char *timeunits = NULL;
                time = strtof(argv[i], &endptr);

                if (endptr != argv[i]) {
                    switch(*endptr) {
                        case '\0':
                            seconds = time;

                        case 's':
                            seconds = time;
                            break;

                        case 'm':
                            minutes = time;
                            break;

                        case 'h':
                            hours = time;
                            break;

                        default:
                            fprintf(stderr, "`%c` is an unknown time unit\n", *endptr);
                            exit(1);
                    }
                }
            }


    }
}

