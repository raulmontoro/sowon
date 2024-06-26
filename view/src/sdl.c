#include <SDL.h>
#include "../../digits.h"


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

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







/***********  SDL *************/


/*  sprite  */
#define SPRITE_CHAR_WIDTH (300 / 2)
#define SPRITE_CHAR_HEIGHT (380 / 2)




#define WIGGLE_COUNT 3
#define WIGGLE_DURATION (0.40f / WIGGLE_COUNT)













/***********  WINDOW *************/


typedef struct WinConfig {
    char *title;
    int positionx;
    int positiony;
    int width;
    int height;
    SDL_WindowFlags flags;
} WinConfig;

void createWindow(int windowwidth, int windowheight, SDL_Window *window) {

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



/***********  RENDERER *************/

void createRenderer(SDL_Window *window, SDL_Renderer *renderer) {


    renderer = SDL_CreateRenderer(window, -1,
               SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    secp(renderer);
}


/*  pre:    array of rgb pixels png
    post:

    notes:  https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
*/
// https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
SDL_Texture *createTexture(SDL_Renderer *renderer, 
                           uint32_t png[],
                           size_t png_width,
                           size_t png_height) {
    // https://wiki.libsdl.org/SDL2/CategorySurface
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
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image_surface);

    secp(texture);

    return texture;
}


void initSDL(int windowwidth, 
             int windowheight,
             uint32_t png[], 
             size_t png_width,
             size_t png_height) {

    secc(SDL_Init(SDL_INIT_VIDEO));
    secc(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"));

    createWindow(windowwidth, windowheight, window);

    createRenderer(window, renderer);

    texture = createTexture(renderer, png, png_width, png_height);
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
             SDL_Rect *dst_rect,
             int charwidth,
             int charheight) {

   // RESIZING DIGIT 
   // transforms digit chosen form image
   // new dimensions
   const int effective_digit_width = (int) floorf((float) charwidth * user_scale * fit_scale);
   const int effective_digit_height = (int) floorf((float) charheight * user_scale * fit_scale);


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



/*  pre:
    post:

    notes: https://wiki.libsdl.org/SDL2/SDL_SetRenderDrawColor
*/
void colourBackground(int r, int g, int b, int a) {
    // black background color 
    SDL_SetRenderDrawColor(renderer, (Uint8)r , (Uint8)g, (Uint8)b, (Uint8)a);
}



void textureColour(SDL_Texture *digits, int r, int g, int b) {
        // texture colour, digits
            secc(SDL_SetTextureColorMod(digits, r, g, b));
}

void clearRenderer(SDL_Renderer *renderer) {
        SDL_RenderClear(renderer);
}


void createRendering(SDL_Renderer *renderer, 
                     SDL_Texture *digits, 
                     size_t wiggle_index, 
                     int pen_x,
                     int pen_y,
                     float fit_scale,
                     float user_scale,
                     int paused,
                     char timestr[9],
                     int charwidth,
                     int charheight,
                     int pausecolorr,
                     int pausecolorg,
                     int pausecolorb,
                     int maincolorr,
                     int maincolorg,
                     int maincolorb) {

        SDL_Rect src_rect;
        SDL_Rect dst_rect;
       

        for (int i = 0; i<8; ++i) {
            srcRect(timestr[i],
                    (wiggle_index + i)%WIGGLE_COUNT,
                    &src_rect);

            dstRect(&pen_x,
                    pen_y,
                    user_scale, 
                    fit_scale, 
                    &dst_rect,
                    charwidth,
                    charheight);

            render_digit_at(renderer, 
                            digits, 
                            &src_rect,
                            &dst_rect);
        }   


        if (paused) {
            secc(SDL_SetTextureColorMod(digits, pausecolorr, pausecolorg, pausecolorb));
        }
        else {
            secc(SDL_SetTextureColorMod(digits, maincolorr, maincolorg, maincolorb));
        }


}









/*  pre:
    post:
    notes:  https://wiki.libsdl.org/SDL2/SDL_SetWindowTitle
*/
void windowTitle(const char *str, int strsize) {
            SDL_SetWindowTitle(window, str);
}



void renderingToScreen(SDL_Renderer *renderer) {
        SDL_RenderPresent(renderer);
}

