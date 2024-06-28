#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/*  sdl */
#include "./view/view.h"

/* imagepixels  */
#include "../../digits.h"






/*  window    */
#define TITLE_CAP 256
#define SCALE_FACTOR 0.15f

/*  rendering frames  */
#define FPS 60
#define DELTA_TIME (1.0f / FPS)


#define WIGGLE_COUNT 3
#define WIGGLE_DURATION (0.40f / WIGGLE_COUNT)


#define COLON_INDEX 10




/*  char    */
#define CHAR_WIDTH (300 / 2)
#define CHAR_HEIGHT (380 / 2)
#define CHARS_COUNT 8

/*  initial window size, text size    */
#define TEXT_WIDTH (CHAR_WIDTH * CHARS_COUNT)
#define TEXT_HEIGHT (CHAR_HEIGHT)



/*  color   */
#define MAIN_COLOR_R 220
#define MAIN_COLOR_G 220
#define MAIN_COLOR_B 220

#define PAUSE_COLOR_R 220
#define PAUSE_COLOR_G 120
#define PAUSE_COLOR_B 120

#define BACKGROUND_COLOR_R 24
#define BACKGROUND_COLOR_G 24
#define BACKGROUND_COLOR_B 24




/********** TIME **********/


void localTime(float *seconds) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    *seconds = tm->tm_sec
               + tm->tm_min  * 60.0f
               + tm->tm_hour * 60.0f * 60.0f;
}










/********** PARSER **********/


/*  time parser     */
float parse_time(const char *time) {
    float result = 0.0f;

    while (*time) {
        char *endptr = NULL;

        /* string to float */
        float x = strtof(time, &endptr);

        if (time == endptr) {
            fprintf(stderr, "`%s` is not a number\n", time);
            exit(1);
        }

        switch (*endptr) {
            case '\0':

            // seconds
            case 's':
                result += x;
                break;
            // minutes
            case 'm':
                result += x * 60.0f;
                break;
            // hours
            case 'h':
                result += x * 60.0f * 60.0f;
                break;

            default:
                fprintf(stderr, "`%c` is an unknown time unit\n", *endptr);
                exit(1);
        }

        time = endptr;
        if (*time) 
            time += 1;
    }

    return result;
}


typedef enum Mode {
    MODE_ASCENDING = 0,
    MODE_COUNTDOWN,
    MODE_CLOCK,
} Mode;

typedef struct Arguments {
    Mode mode;
    int flag_p;
    int flag_e;
    float initialascendingclock;
    float initialcountdownclock;  // float displayed_time_initial;
} Arguments;


/* argument parser  */
void mainParser(int argc,
                char **argv,
                Arguments *arguments) {
    
    /*  default when no arguments in main */
    *arguments =(Arguments){MODE_ASCENDING,
                            0,
                            0,
                            0,
                            5*60};

    /* at least one argument */ 
    for (int i = 1; i < argc; ++i) {

            // pause
            if (strcmp(argv[i], "-p") == 0) {
                arguments->flag_p = 1;
            } 
            // exist
            else if (strcmp(argv[i], "-e") == 0) {
                arguments->flag_e = 1;
            }
            else if (strcmp(argv[i], "ascending") == 0) {
                arguments->mode = MODE_ASCENDING;
                arguments->initialascendingclock = parse_time(argv[i+1]);
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
            // countdown
            else {
                arguments->mode = MODE_COUNTDOWN;
                arguments->initialcountdownclock = parse_time(argv[i]);
            }
    }
}




/***********  STATE *************/

typedef struct State {
    Mode mode;
    float displayed_time;
    float initialcountdownclock;
    int paused;
    int exit_count_down;
    
    size_t wiggle_index;
    float wiggle_cooldown;

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

    // default initial state for ascending mode when no arguments 
    *state = (State){MODE_ASCENDING,
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
        case MODE_ASCENDING:
            break;

        case MODE_COUNTDOWN:
            state->initialcountdownclock = arguments.initialcountdownclock;
            state->displayed_time = state->initialcountdownclock; 
            break;

        case MODE_CLOCK:
            localTime(&state->displayed_time);
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










/*  event key down  compute */
void eventCompute(State *initstate,
                       State *state, 
                       Event event) {

    switch(clockevent) {
        case NONE:
            break;

        case SPACE:
            pauseToggle(state);
            break;

        case EQUALS:
            zoomInitial(state);
            break;

        case MINUS:
            zoomOut(state);
            break;

        case ZERO:
            zoomInitial(state);
            break;

        case SHIFTZERO:
            zoomIn(state);
            break;

        case F5:
            state = initstate;
            break;
        case F11:
            fullScreenToggle();
            break;

        case WHEELUP:
            state->user_scale += SCALE_FACTOR * state->user_scale;
            break;

        case WHEELDOWN:
            state->user_scale -= SCALE_FACTOR * state->user_scale;
            break;
    }
}










void wiggleCoolDown(State *state) {
    if (state->wiggle_cooldown <= 0.0f) {
        state->wiggle_index++;
        state->wiggle_cooldown = WIGGLE_DURATION;
    }
    else {
        state->wiggle_cooldown -= DELTA_TIME;
    }
}








/********** TIME **********/

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
            case MODE_ASCENDING: {
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





/********** WINDOW **********/

void updateWindowResizeAndZoomInOut(State *state) {
    // window width and height
    windowSize(&state->w, &state->h);
    
    // widow resize ratio
    fitScale(state->w, state->h, &state->fit_scale);
        
}


void updatePen(State *state) {
    initial_pen(state->w,
                state->h,
                state->user_scale,
                state->fit_scale,
                &state->pen_x, 
                &state->pen_y);
    
}

void timeTitle(char *prev_title, int prev_title_size, char timestr[9], State state) {

        /*  print time as window's title */
        char title[prev_title_size];

        /*  title construction */
        snprintf(title, prev_title_size, "sowon - %d%d:%d%d:%d%d", timestr[0], timestr[1], timestr[3], timestr[4], timestr[6], timestr[7]);

        if (strcmp(state.prev_title, title) != 0) {
            windowTitle(title);
        }

        /*  store old title, 
            after new title has been rendered */
        memcpy(prev_title, title, prev_title_size);
}




/************************************************ INFINITE LOOP */
void infiniteLoop(State *state) {

    int quit = 0;
    while (!quit) {

        /*  time string 00:00:00\0 
        */
        char timestr[9];
        hoursMinutesSeconds(&state, timestr);
        
        
        /* render */
        timeTitle(state.prev_title, TITLE_CAP, timestr, state);




        backgroundColour(BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 255);
        
        if (state.paused) {
            textureColour(PAUSE_COLOR_R, PAUSE_COLOR_G, PAUSE_COLOR_B);
        }
        else {
            textureColour(MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B);
        }

        clearRenderer();
        createRendering(state.wiggle_index, 
                        state.pen_x, 
                        state.pen_y, 
                        state.fit_scale, 
                        state.user_scale, 
                        state.paused, 
                        timestr,
                        CHAR_WIDTH,
                        CHAR_HEIGHT,
                        PAUSE_COLOR_R,
                        PAUSE_COLOR_G,
                        PAUSE_COLOR_B,
                        MAIN_COLOR_R,
                        MAIN_COLOR_G,
                        MAIN_COLOR_B,
                        WIGGLE_COUNT);

        renderingToScreen();
        


        /*  events */
        Event event = NONE;
        eventLoop(&quit, &event);
        eventCompute(initstate, &state, event);


        /* update   */
        updateWindowResizeAndZoomInOut(&state);
        updatePen(&state);
        wiggleCoolDown(&state);
        updateTime(&state);
        
        delayInfiniteLoop(DELTA_TIME);

    }

    quitSDL();
}




/********** MAIN **********/

int main(int argc, char **argv) {
    
    /* parsing */
    Arguments arguments;
    mainParser(argc, argv, &arguments);
    
    /* initial state */
    State initstate;
    initialState(arguments, &initstate);

    /* sdl  */
    /* window same size as rendering text */
    initSDL(TEXT_WIDTH,
            TEXT_HEIGHT,
            png,
            png_width,
            png_height);

    /*  loop */    
    infiniteLoop(&initstate);
    
    return 0;
}
