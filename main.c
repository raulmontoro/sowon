#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>

#include "./digits.h"

/*  window    */
#define TITLE_CAP 256
#define SCALE_FACTOR 0.15f

/*  rendering frames  */
#define FPS 60
#define DELTA_TIME (1.0f / FPS)


/*  sprite  */
#define SPRITE_CHAR_WIDTH (300 / 2)
#define SPRITE_CHAR_HEIGHT (380 / 2)

/*  char    */
#define CHAR_WIDTH (300 / 2)
#define CHAR_HEIGHT (380 / 2)
#define CHARS_COUNT 8

/*  initial window size, text size    */
#define TEXT_WIDTH (CHAR_WIDTH * CHARS_COUNT)
#define TEXT_HEIGHT (CHAR_HEIGHT)

#define WIGGLE_COUNT 3
#define WIGGLE_DURATION (0.40f / WIGGLE_COUNT)

#define COLON_INDEX 10


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





/********** ERROR HANDLER **********/

void secc(int code)
{
    if (code < 0) {
        fprintf(stderr, "SDL pooped itself: %s\n", SDL_GetError());
        abort();
    }
}

void secp(void *ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "SDL pooped itself: %s\n", SDL_GetError());
        abort();
    }
}









/********** PARSER **********/

/*  time parser     */
float parse_time(const char *time) {
    float result = 0.0f;

    while (*time) {
        char *endptr = NULL;
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
        if (*time) time += 1;
    }

    return result;
}




/********** PARSER **********/


typedef enum Mode {
    MODE_ASCENDING = 0,
    MODE_COUNTDOWN,
    MODE_CLOCK,
} Mode;

typedef struct MainArguments {
    Mode mode;
    int flag_p;
    int flag_e;
    int initialclockcountdown;
} MainArguments;

int isNaturalNumber(char *argv) {
    int a = 1;
    while(a != 0 && *argv != '\0') {
        a = *argv >= '0' && *argv <= '9';
        ++argv;
    }
    return a;
}

int isString(char *argv) {
    int a = 1;
    while(a != 0 && *argv != '\0') {
        a = *argv >= 'A' && *argv <= 'Z' &&
            *argv >= 'a' && *argv <= 'z';
        ++argv;
    }
    return a;
}


/* argument parser  */
void mainArgumentsParser(int argc,
                         char **argv,
                         MainArguments *mainarguments) {
    
    /*  default when no arguments */
    *mainarguments =(MainArguments){MODE_ASCENDING,
                                    0,
                                    0
                                    0};
    /* at least one argument */ 
    for (int i = 1; i < argc; ++i) {
        if (isNatural(*argv+i)) {
        }
        
        if (isString(*argv+i)) {
            // pause
            if (strcmp(argv[i], "-p") == 0) {
                mainarguments->flag_p = 1;
            } 
            // exist
            else if (strcmp(argv[i], "-e") == 0) {
                mainarguments->flag_e = 1;
            }
            // time clock
            else if (strcmp(argv[i], "clock") == 0) {
                mainarguments->mode = MODE_CLOCK;
            }
            // countdown
            else {
                mainarguments->mode = MODE_COUNTDOWN;
            }
        }

    }

}




/***********  PROGRAM STATE *************/

typedef struct ProgramState {
    float displayed_time;
    float displayed_time_initial;
    int paused;
    int exit_count_down;
    size_t wiggle_index;
    float wiggle_cooldown;
    float user_scale;
    char prev_title[TITLE_CAP];
    int w;
    int h;
    float fit_scale;
    int pen_x;
    int pen_y;
} ProgramState;

void initialState(MainArguments mainarguments, ProgramState *programstate) {

    // default initial state for ascending mode when no arguments 
    *programstate = (ProgramState){0.0f, 
                                   0.0f, 
                                   0, 
                                   0,
                                   0, 
                                   WIGGLE_DURATION, 
                                   1.0f, 
                                   "hello world",
                                   TEXT_WIDTH,
                                   TEXT_HEIGHT,
                                   1.0f,
                                   0,
                                   0};
    
    programstate->paused = mainarguments.flag_p; 
    programstate->exit_count_down = mainarguments.flag_e; 

    if (mainarguments.mode == MODE_ASCENDING) {
        programstate->displayed_time = 0.0f; 
    }
    else if (mainarguments.mode == MODE_COUNTDOWN) {
    }

    mainarguments->displayed_time = parse_time(argv[i]);
    mainarguments->paused = 1;

    switch(mainarguments->mode) {
        case MODE_ASCENDING:
            break;

        case MODE_COUNTDOWN:
            mainarguments->displayed_time_initial = mainarguments->displayed_time;
            break;

        case MODE_CLOCK:
            break;
    }

}







/***********  SDL *************/

void initializeSDL() {
    secc(SDL_Init(SDL_INIT_VIDEO));
    secc(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"));
}


void createWindow(SDL_Window **window) {
    *window = SDL_CreateWindow(
                     "sowon",
                     0, 0, TEXT_WIDTH, TEXT_HEIGHT,
                     SDL_WINDOW_RESIZABLE);
    secp(window);
}

void createRenderer(SDL_Window *window, SDL_Renderer **renderer) {
    *renderer = SDL_CreateRenderer(window, -1,
                 SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    secp(renderer);
}



SDL_Texture *createTextureFromFile(SDL_Renderer *renderer) {
   SDL_Surface* image_surface;
    image_surface =  SDL_CreateRGBSurfaceFrom(
                        png,
                        (int) png_width,
                        (int) png_height,
                        32,
                        (int) png_width * 4,
                        0x000000FF,
                        0x0000FF00,
                        0x00FF0000,
                        0xFF000000);

    secp(image_surface);
    
    SDL_Texture *digits = SDL_CreateTextureFromSurface(renderer, image_surface);

    secp(digits);
    secc(SDL_SetTextureColorMod(digits, MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B));
    return digits;
}


void initSDL(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **digits) {
    initializeSDL();
    createWindow(window);
    createRenderer(*window, renderer);
    *digits = createTextureFromFile(*renderer);
}







void windowSize(SDL_Window *window, int *w, int *h) {
    SDL_GetWindowSize(window, w, h);
}

void fullScreenToggle(SDL_Window *window) {
    Uint32 window_flags;
    secc(window_flags = SDL_GetWindowFlags(window));

    if(window_flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        secc(SDL_SetWindowFullscreen(window, 0));
    } 
    else {
        secc(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP));
    }
}


// SELECTS DIGIT FROM DIGITS.PNG DIGITS IMAGE
// digit_index, column
// wiggle_index, row
void srcRect(int digit_index,
             int wiggle_index,
             SDL_Rect *src_rect) {

   *src_rect = (SDL_Rect){(int) (digit_index*SPRITE_CHAR_WIDTH),
                              (int) (wiggle_index*SPRITE_CHAR_HEIGHT),
                              SPRITE_CHAR_WIDTH,
                              SPRITE_CHAR_HEIGHT};
}

void dstRect(int *pen_x,
             int pen_y, 
             float user_scale, 
             float fit_scale, 
             SDL_Rect *dst_rect) {

   // RESIZING DIGIT 
   // transforms digit chosen form image
   // new dimensions
   const int effective_digit_width = (int) floorf((float) CHAR_WIDTH * user_scale * fit_scale);
   const int effective_digit_height = (int) floorf((float) CHAR_HEIGHT * user_scale * fit_scale);


   *dst_rect = (SDL_Rect){*pen_x,
                          pen_y,
                          effective_digit_width,
                          effective_digit_height};

    // new cartesian coordinates of next new digit
   *pen_x += effective_digit_width;
}








/*  choosing subimage from and image    
    and resizing */
void render_digit_at(SDL_Renderer *renderer, 
                     SDL_Texture *digits,
                     SDL_Rect *src_rect,
                     SDL_Rect *dst_rect) {


   // ADDS EACH NEW DIGIT TO RENDERER, ONE BY ONE
   SDL_RenderCopy(renderer, digits, src_rect, dst_rect);
    
}


/*  B   
            https://stackoverflow.com/questions/10279718/append-char-to-string-in-c
            https://www.w3schools.com/c/c_strings.php

        iteration over a c-string
            https://stackoverflow.com/questions/3213827/how-to-iterate-over-a-string-in-c

        number of digits
            https://www.geeksforgeeks.org/program-count-digits-integer-3-different-methods/
*/
void hoursMinutesSeconds(Config *config, char timestr[9]) {
        // TODO: support amount of hours >99

        const size_t time = (size_t) ceilf(fmaxf(config->displayed_time, 0.0f));
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



void backgroundColour(SDL_Renderer *renderer) {
        // black background color 
        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 255);
}

void textureColour(SDL_Texture *digits, Config *config) {
        // texture colour, digits
        if (config->paused) {
            secc(SDL_SetTextureColorMod(digits, PAUSE_COLOR_R, PAUSE_COLOR_G, PAUSE_COLOR_B));
        } else {
            secc(SDL_SetTextureColorMod(digits, MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B));
        }
}

void clearRenderer(SDL_Renderer *renderer) {
        SDL_RenderClear(renderer);
}


void createRendering(SDL_Renderer *renderer, 
                     SDL_Texture *digits, 
                     Config *config, 
                     char timestr[9]) {

        SDL_Rect src_rect;
        SDL_Rect dst_rect;
       

        for (int i = 0; i<8; ++i) {
            srcRect(timestr[i],
                    (config->wiggle_index + i)%WIGGLE_COUNT,
                    &src_rect);

            dstRect(&config->pen_x, config->pen_y,
                    config->user_scale, config->fit_scale, 
                    &dst_rect);

            render_digit_at(renderer, 
                            digits, 
                            &src_rect,
                            &dst_rect);
        }   

}











void timeInWindowTitle(SDL_Window *window, Config *config, char timestr[9]) {

        /*  print time as window's title */
        char title[TITLE_CAP] ="Hello World!";
        //snprintf(title, sizeof(title), "%02zu:%02zu:%02zu - sowon", hours, minutes, seconds);
        snprintf(title, sizeof(title), "%d%d:%d%d:%d%d - sowon", timestr[0], timestr[1], timestr[3], timestr[4], timestr[6], timestr[7]);

        if (strcmp(config->prev_title, title) != 0) {
            SDL_SetWindowTitle(window, title);
        }

        memcpy(title, config->prev_title, TITLE_CAP);
}

void renderingToScreen(SDL_Renderer *renderer) {
        SDL_RenderPresent(renderer);
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



/*  paused  */

void pauseToggle(Config *config) {
    config->paused = !config->paused;
}

/*  zoom   */

void zoomInitial(Config *config) {
    config->user_scale = 1.0f;
}

/* pre:
   post: zoom in, represente in 'user_scale'
*/
void zoomIn(Config *config) {
    config->user_scale += SCALE_FACTOR*config->user_scale;
}

void zoomOut(Config *config) {
    config->user_scale -= SCALE_FACTOR*config->user_scale;
}


/*  reset clock  */

void resetClock(Config *config, SDL_Texture *digits) {

        config->displayed_time = 0.0f;
        config->paused = 0;
        
        if (config->flag_p) {
            config->paused = 1;
        }
        else {
            config->displayed_time = config->displayed_time_initial;
        }

        if (config->paused) {
            secc(SDL_SetTextureColorMod(digits, PAUSE_COLOR_R, PAUSE_COLOR_G, PAUSE_COLOR_B));
        }
        else {
            secc(SDL_SetTextureColorMod(digits, MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B));
        }
}

int quitSDL() {
   SDL_Quit();
   return 0;
}







/*  EVENTS  */

typedef enum KeyDownEvent {
    NONEKEY,
    SPACE,
    EQUALS,
    MINUS,
    ZERO,
    SHIFTZERO,
    F5,
    F11
} KeyDownEvent;

/*  event key down  compute */
void keyDownEventCompute(SDL_Window *window,
                        SDL_Texture *digits, 
                        Config *config, 
                        KeyDownEvent keydownevent) {

    switch(keydownevent) {
        case NONEKEY:
            break;

        case SPACE:
            pauseToggle(config);
            break;

        case EQUALS:
            zoomInitial(config);
            break;

        case MINUS:
            zoomOut(config);
            break;

        case ZERO:
            zoomInitial(config);
            break;

        case SHIFTZERO:
            zoomIn(config);
            break;

        case F5:
            resetClock(config, digits);
            break;
        case F11:
            fullScreenToggle(window);
            break;
    }
}




/*  event key down  */

void keyDownEvent(SDL_Event event, KeyDownEvent *keydownevent) {

    // https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html
    switch (event.key.keysym.sym) {
        case SDLK_SPACE: {
            *keydownevent = SPACE;
        } 
        break;

        case SDLK_KP_PLUS:

        case SDLK_EQUALS: {
            *keydownevent = EQUALS;
        } 
        break;

        case SDLK_KP_MINUS:

        case SDLK_MINUS: {
            *keydownevent = MINUS;
        } 
        break;

        case SDLK_KP_0:
        
        // in a spanish keyboard, 'equals key' is <shift+0> for zoom in
        case SDLK_0: {
            if (event.key.keysym.mod & KMOD_SHIFT) {
                *keydownevent = SHIFTZERO;
            }
            else 
                *keydownevent = ZERO;
        } 
        break;

        case SDLK_F5: {
            *keydownevent = F5;
        } 
        break;

        case SDLK_F11: {
            *keydownevent = F11;
        } 
        break;
    }
}


/*  MOUSE WHEEL EVENT */

typedef enum MouseWheelEvent{
    NONEWHEEL,
    WHEELUP,
    WHEELDOWN
} MouseWheelEvent;


void mouseWheelEvent(SDL_Event event, MouseWheelEvent *mousewheelevent) {
    if (SDL_GetModState() & KMOD_CTRL) {
        if (event.wheel.y > 0) {
            *mousewheelevent = WHEELUP;
        } 
        else if (event.wheel.y < 0) {
            *mousewheelevent = WHEELDOWN;
        }
        else
            *mousewheelevent = NONEWHEEL;
    }
}

void mouseWheelCompute(MouseWheelEvent mousewheelevent, Config *config) {
    switch(mousewheelevent) {
        case NONEWHEEL:
            break;

        case WHEELUP:
            config->user_scale += SCALE_FACTOR * config->user_scale;
            break;
        case WHEELDOWN:
            config->user_scale -= SCALE_FACTOR * config->user_scale;
            break;
    }
}



/* even loop    */
void eventLoop(int *quit,
               KeyDownEvent *keydownevent,
               MouseWheelEvent *mousewheelevent) {

    SDL_Event event = {0};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                *quit = 1;
            } break;

            case SDL_KEYDOWN: {
                keyDownEvent(event, keydownevent);

            } break;

            case SDL_MOUSEWHEEL: {
                mouseWheelEvent(event, mousewheelevent);
            } break;

            default: {
            }
        }
    }
}


void localTime(&config) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    config->displayed_time = tm->tm_sec
                   + tm->tm_min  * 60.0f
                   + tm->tm_hour * 60.0f * 60.0f;
}





// UPDATE

void wiggleCoolDown(Config *config) {
    if (config->wiggle_cooldown <= 0.0f) {
        config->wiggle_index++;
        config->wiggle_cooldown = WIGGLE_DURATION;
    }
    else {
        config->wiggle_cooldown -= DELTA_TIME;
    }
}

void updateTime(ProgramState *programstate) {
    if (!programstate->paused) {
        switch (programstate->mode) {
            case MODE_ASCENDING: {
                programstate->displayed_time += DELTA_TIME;
            } 
            break;
        
            case MODE_COUNTDOWN: {
                if (programstate->displayed_time > 1e-6) {
                    programstate->displayed_time -= DELTA_TIME;
                } 
                else {
                    programstate->displayed_time = 0.0f;
                    if (programstate->exit_after_countdown) {
                        quitSDL();
                    }
                }
            } 
            break;
        
            case MODE_CLOCK: {
                localTime(&programstate);
            } 
            break;
        }
    }
}

void updateWindowResizeAndZoomInOut(SDL_Window *window, Config *config) {
    // window width and height
    windowSize(window, &config->w, &config->h);
    
    // widow resize ratio
    fitScale(config->w, config->h, &config->fit_scale);
        
}


void updatePen(Config *config) {
    initial_pen(config->w,
                config->h,
                config->user_scale,
                config->fit_scale,
                &config->pen_x, 
                &config->pen_y);
    
}

// INFINITE LOOP
void infiniteLoop(Config *config) {

    /* sdl  */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *digits;
    initSDL(&window, &renderer, &digits);



    int quit = 0;
    while (!quit) {

        /*  events */
        KeyDownEvent keydownevent = NONEKEY;
        MouseWheelEvent mousewheelevent = NONEWHEEL;
        eventLoop(&quit, &keydownevent, &mousewheelevent);
        keyDownEventCompute(window, digits, config, keydownevent);
        mouseWheelCompute(mousewheelevent, config);
        
        /* time */
        char timestr[9];
        hoursMinutesSeconds(config, timestr);
        
        /* render */
        timeInWindowTitle(window, config, timestr);
        backgroundColour(renderer);
        textureColour(digits, config);
        clearRenderer(renderer);
        createRendering(renderer, digits, config, timestr);
        renderingToScreen(renderer);

        updateWindowResizeAndZoomInOut(window, config);
        updatePen(config);
        wiggleCoolDown(config);
        updateTime(config);

        SDL_Delay((int) floorf(DELTA_TIME * 1000.0f));

    }

    quitSDL();
}




/*  MAIN    */
int main(int argc, char **argv) {
    
    /* parsing */
    MainArguments mainarguments;
    mainArgumentsParser(argc, argv, &config, &mainarguments);
    
    /* initial state */
    ProgramState programstate;
    initialState(mainarguments, &programstate);
    
    infiniteLoop(&config);
    

    return 0;
}
