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

