#include <SDL.h>


/*  sprite  */
#define SPRITE_CHAR_WIDTH (300 / 2)
#define SPRITE_CHAR_HEIGHT (380 / 2)




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







/***********  WINDOW *************/

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






/***********  RENDERER *************/

void createRenderer() {

    renderer = SDL_CreateRenderer(window, -1,
               SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    secp(renderer);
}








/***********  TEXTURE *************/

/*  pre:    array of rgb pixels png
    post:

    notes:  https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
*/
// https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
void createTexture(uint32_t imagepixel[],
                   size_t imagepixel_width,
                   size_t imagepixel_height) {

    // https://wiki.libsdl.org/SDL2/CategorySurface
    SDL_Surface* image_surface;
    image_surface =  SDL_CreateRGBSurfaceFrom(
                        imagepixel,
                        (int) imagepixel_width,
                        (int) imagepixel_height,
                        32,
                        (int) imagepixel_width * 4,
                        0x000000FF,
                        0x0000FF00,
                        0x00FF0000,
                        0xFF000000);

    secp(image_surface);
    
    texture = SDL_CreateTextureFromSurface(renderer, image_surface);

    secp(texture);
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



void textureColour(int r, int g, int b) {
        // texture colour, digits
            secc(SDL_SetTextureColorMod(texture, r, g, b));
}

void clearRenderer() {
        SDL_RenderClear(renderer);
}


void createRendering(size_t wiggle_index, 
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
                     int maincolorb,
                     int wigglecount) {

        SDL_Rect src_rect;
        SDL_Rect dst_rect;
       

        for (int i = 0; i<8; ++i) {
            srcRect(timestr[i],
                    (wiggle_index + i)%wigglecount,
                    &src_rect);

            dstRect(&pen_x,
                    pen_y,
                    user_scale, 
                    fit_scale, 
                    &dst_rect,
                    charwidth,
                    charheight);

            render_digit_at(renderer, 
                            texture, 
                            &src_rect,
                            &dst_rect);
        }   


        if (paused) {
            secc(SDL_SetTextureColorMod(texture, pausecolorr, pausecolorg, pausecolorb));
        }
        else {
            secc(SDL_SetTextureColorMod(texture, maincolorr, maincolorg, maincolorb));
        }


}


/*  pre:
    post:
    notes:  https://wiki.libsdl.org/SDL2/SDL_SetWindowTitle
*/
void windowTitle(const char *str) {
            SDL_SetWindowTitle(window, str);
}

void renderingToScreen() {
        SDL_RenderPresent(renderer);
}



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


void delayInfiniteLoop(float deltatime) {
    SDL_Delay((int) floorf(deltatime * 1000.0f));
}








/*  event key down  */

void keyDownEvent(SDL_Event event, ClockEvent *clockevent) {

    // https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html
    switch (event.key.keysym.sym) {
        case SDLK_SPACE: {
            *clockevent = SPACE;
        } 
        break;

        case SDLK_KP_PLUS:

        case SDLK_EQUALS: {
            *clockevent = EQUALS;
        } 
        break;

        case SDLK_KP_MINUS:

        case SDLK_MINUS: {
            *clockevent = MINUS;
        } 
        break;

        case SDLK_KP_0:
        
        // in a spanish keyboard, 'equals key' is <shift+0> for zoom in
        case SDLK_0: {
            if (event.key.keysym.mod & KMOD_SHIFT) {
                *clockevent = SHIFTZERO;
            }
            else 
                *clockevent = ZERO;
        } 
        break;

        case SDLK_F5: {
            *clockevent = F5;
        } 
        break;

        case SDLK_F11: {
            *clockevent = F11;
        } 
        break;
    }
}


void mouseWheelEvent(SDL_Event event, ClockEvent *clockevent) {
    if (SDL_GetModState() & KMOD_CTRL) {
        if (event.wheel.y > 0) {
            *clockevent = WHEELUP;
        } 
        else if (event.wheel.y < 0) {
            *clockevent = WHEELDOWN;
        }
        else
            *clockevent = NONE;
    }
}













/* even loop    */
void eventLoop(int *quit,
               Event *clockevent) {

    SDL_Event event = {0};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                *quit = 1;
            } break;

            case SDL_KEYDOWN: {
                keyDownEvent(event, clockevent);

            } break;

            case SDL_MOUSEWHEEL: {
                mouseWheelEvent(event, clockevent);
            } break;

            default: {
            }
        }
    }
}


void quitSDL() {
   SDL_Quit();
}
