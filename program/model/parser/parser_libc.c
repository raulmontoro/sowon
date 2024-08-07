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
