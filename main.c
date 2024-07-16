#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdio.h>

/*  strtof()
    https://cplusplus.com/reference/cstdlib/strtof/
*/
#include <stdlib.h>

/*  https://cplusplus.com/reference/cstring
*/
#include <string.h>
#include <time.h>
#include <math.h>


/*  window
    initial window size
    text size
*/
#define TEXT_WIDTH (CHAR_WIDTH * CHARS_COUNT)
#define TEXT_HEIGHT (CHAR_HEIGHT)
#define TITLE_CAP 256
#define SCALE_FACTOR 0.15f

/*  sprite  */
#define SPRITE_CHAR_WIDTH (300 / 2)
#define SPRITE_CHAR_HEIGHT (380 / 2)

#define CHAR_WIDTH (300 / 2)
#define CHAR_HEIGHT (380 / 2)
#define CHARS_COUNT 8


/*  rendering frames  */
#define FPS 60
#define DELTA_TIME (1.0f / FPS)

/*  wiggle  */
#define WIGGLE_COUNT 3
#define WIGGLE_DURATION (0.40f / WIGGLE_COUNT)



int main(int argc, char **argv) {
    
    /* parsing */
    Arguments arguments;
    parseMainArguments(argc, argv, &arguments);
    
    /* initial state */
    State initstate;
    initialState(arguments, &initstate);

    /*  loop */    
    frameLoop(&initstate);
    
    return 0;
}
