
/*      -m clock
        -t 1h30m00s
        -t 1h 30m 00s
        -mclock-t1h30m15s
*/

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


/********** MAIN ARGUMENTS PARSER **********/

/*  https://cplusplus.com/reference/cstring
*/
#include <string.h>


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
    *arguments =(Arguments){0,
                            0,
                            0,
                            0,
                            0,
                            CLOCK};

    /* at least one argument */ 
    for (int i = 1; i < argc; ++i) {

            if (strcmp(argv[i], "-m") == 0) {
                arguments->flag_m = 1;
            } 
            else if (strcmp(argv[i], "-t") == 0) {
                arguments->flag_t = 1;
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
                float float1 = 0.0f;
                char *str1 = NULL;
                char *endptr = NULL;

                /*  string to float */
                float1 = strtof(argv[i], &endptr);
                
                /* float found and we are in a string */
                if (endptr != argv[i]) {
                    switch(*endptr) {
                        case 'h':
                            arguments->hours = float1;
                            break;

                        case 'm':
                            arguments->minutes = float1;
                            break;

                        case 's':
                            arguments->seconds = float1;
                            break;

                        case '\0':
                            arguments->seconds = float1;

                        default:
                            fprintf(stderr, "`%c` is an unknown time unit\n", *endptr);
                            exit(1);
                    }
                }
            }
    }
}

