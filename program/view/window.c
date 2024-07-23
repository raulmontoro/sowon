#include "window.h"
#include "renderer.h"

void initSDL(int windowwidth, 
             int windowheight,
             uint32_t imgagepixels[], 
             size_t imgagepixels_width,
             size_t imgagepixels_height) {

    secc(SDL_Init(SDL_INIT_VIDEO));
    secc(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"));

    createWindow(windowwidth, windowheight);

    createRenderer();

    createTexture(imagepixel, imagepixel__width, imagepixel__height);
}


void initializeWindow() {

    /*  sprite  */
    #define SPRITE_CHAR_WIDTH (300 / 2)
    #define SPRITE_CHAR_HEIGHT (380 / 2)
    
    #define CHAR_WIDTH (300 / 2)
    #define CHAR_HEIGHT (380 / 2)
    #define CHARS_COUNT 8
    
    #define TEXT_WIDTH (CHAR_WIDTH * CHARS_COUNT)
    #define TEXT_HEIGHT (CHAR_HEIGHT)
    
    /*  window  */
    #define TITLE_CAP 256
    
    #define SCALE_FACTOR 0.15f

    /* sdl  */
    /* window same size as rendering text */
    initSDL(TEXT_WIDTH,
            TEXT_HEIGHT,
            png,
            png_width,
            png_height);

}











SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

typedef struct WinConfig {
    char *title;
    int positionx;
    int positiony;
    int width;
    int height;
    SDL_WindowFlags flags;
} WinConfig;

void createWindow(int windowwidth, int windowheight) {

    WinConfig winconfig = {"sowon",
                           0,
                           0,
                           windowwidth,
                           windowheight,
                           SDL_WINDOW_RESIZABLE};

    window = SDL_CreateWindow(winconfig.title,
                              winconfig.positionx, 
                              winconfig.positiony, 
                              winconfig.width,
                              winconfig.height,
                              winconfig.flags);
    secp(window);
}


/*  pre:
    post:

    notes:  https://wiki.libsdl.org/SDL2/SDL_SetWindowTitle
*/
void windowTitle(const char *str) {
            SDL_SetWindowTitle(window, str);
}


void windowSize(int *w, int *h) {
    SDL_GetWindowSize(window, w, h);
}

void fullScreenToggle() {
    Uint32 window_flags;
    secc(window_flags = SDL_GetWindowFlags(window));

    if(window_flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        secc(SDL_SetWindowFullscreen(window, 0));
    } 
    else {
        secc(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP));
    }
}


/*  zoom   */
void reiniciateZoom(State *state) {
    state->zoomscale = 1.0f;
}

/* pre:
   post: zoom in, represente in 'zoomscale'
*/
void zoomIn(State *state) {
    state->zoomscale += SCALE_FACTOR*state->zoomscale;
}

void zoomOut(State *state) {
    state->zoomscale -= SCALE_FACTOR*state->zoomscale;
}

// window width and height
void updateWindowResize(State *state) {
    windowSize(&state->w, &state->h);
}





/*  fit scale   */

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

// widow resize ratio
void updateWindowZoomInOut(State *state) {
    fitScale(state->w, state->h, &state->fit_scale);
}













/*  pre:    
    post:   cartesian coordinates
            position where rendering starts 
            to fit CHAR_COUNT characters 
            at zoomscale*fit_scale scale
*/
void setPen(int w, 
            int h,
            float zoomscale,
            float fit_scale, 

            int *pen_x,
            int *pen_y) {
    
    // character width after scaling 
    const int effective_digit_width = (int)floorf(
                                            (float)CHAR_WIDTH*zoomscale*fit_scale
                                      );
    // character height after scaling
    const int effective_digit_height = (int)floorf(
                                            (float)CHAR_HEIGHT*zoomscale*fit_scale
                                       );
    
    // position where rendering starts 
    *pen_x = w/2 - effective_digit_width*CHARS_COUNT/2;
    *pen_y = h/2 - effective_digit_height/2;
}
