/*  voluntarily resizing
    zoom in and zoom out
    full screen
*/

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



/* sdl  */
/* window same size as rendering text */
initSDL(TEXT_WIDTH,
        TEXT_HEIGHT,
        png,
        png_width,
        png_height);

