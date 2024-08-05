/*  https://github.com/cofyc/argparse/tree/master
*/

/*  https://cplusplus.com/reference/cstring
*/

/*      -m clock
        -t 1h30m00s
        -t 1h 30m 00s
        -mclock-t1h30m15s

        12h
        30m
        45s 
        12h30m45s
    
*/




/*  strcmp()
*/
#include <string.h>


/* strtof()
*/
#include <stdlib.h>


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
            /*  string to float */
            else {
                float float1 = 0.0f;
                char *str1 = NULL;
                char *endptr = NULL;

                str1 = argv[i];

                float1 = strtof(str1, &endptr);
                
                /* float found and we are in a string */
                
                while (*str1) {
                    if (endptr == argv[i]) {
                        fprintf(stderr, "`%s` is not a number\n", time);
                        exit(1);
                    } 
                    else {
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
                                break;

                            default:
                                break;
                    }
                }
            }
            // if *endptr is not \0
            if (*endptr) {
                ++endptr;
            }
    }
}
