/********** TIME **********/


void localTime(float *seconds) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    *seconds = tm->tm_sec
               + tm->tm_min  * 60.0f
               + tm->tm_hour * 60.0f * 60.0f;
}



/*  pre:
    post:       float to char

    notes:      https://stackoverflow.com/questions/10279718/append-char-to-string-in-c
                https://www.w3schools.com/c/c_strings.php

                iteration over a c-string
                    https://stackoverflow.com/questions/3213827/how-to-iterate-over-a-string-in-c

                number of digits
                    https://www.geeksforgeeks.org/program-count-digits-integer-3-different-methods/
*/
void hoursMinutesSeconds(State *state, char timestr[9]) {
        // TODO: support amount of hours >99

        const size_t time = (size_t) ceilf(fmaxf(state->displayed_time, 0.0f));
        /*  hours   */
        const size_t hours = time/60/60;
        const size_t hoursfirstdigit = hours/10;
        const size_t hoursseconddigit = hours%10;
        
        /*  minutes */
        const size_t minutes = time/60%60;
        const size_t minutesfirstdigit = minutes/10;
        const size_t minutesseconddigit = minutes%10;
        
        /*  seconds */
        const size_t seconds = time % 60;
        const size_t secondsfirstdigit = seconds/10;
        const size_t secondsseconddigit = seconds%10;
        
        timestr[0] = hoursfirstdigit; 

        timestr[1] = hoursseconddigit; 

        timestr[2] = COLON_INDEX;

        timestr[3] = minutesfirstdigit; 

        timestr[4] = minutesseconddigit; 

        timestr[5] = COLON_INDEX;

        timestr[6] = secondsfirstdigit; 

        timestr[7] = secondsseconddigit; 

        timestr[8] = '\0';
}



void updateTime(State *state) {
    if (!state->paused) {
        switch (state->mode) {
            case MODE_STOPWATCH: {
                state->displayed_time += DELTA_TIME;
            } 
            break;
        
            case MODE_COUNTDOWN: {
                if (state->displayed_time > 1e-6) {
                    state->displayed_time -= DELTA_TIME;
                } 
                else {
                    state->displayed_time = 0.0f;
                    if (state->exit_count_down) {
                        quitSDL();
                    }
                }
            } 
            break;
        
            case MODE_CLOCK: {
                localTime(&state->displayed_time);
            } 
            break;
        }
    }
}

