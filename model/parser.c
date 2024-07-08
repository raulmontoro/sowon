/********** PARSER TIME **********/


/*  time parser     */
float parse_time(const char *time, timeunits) {
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








/********** PARSER MAIN ARGUMENTS **********/


typedef enum Mode {
    MODE_STOPWATCH = 0,
    MODE_COUNTDOWN,
    MODE_CLOCK,
} Mode;

typedef struct Arguments {
    Mode mode;
    int flag_p;
    int flag_e;
    int hours;
    int minutes;
    int seconds;
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

