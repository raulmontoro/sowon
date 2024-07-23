/**************** RENDERER */


void createRenderer();
void clearRenderer();
void createRendering(size_t wiggle_index, 
                     int pen_x,
                     int pen_y,
                     float fit_scale,
                     float zoomscale,
                     int paused,
                     char timestr[9],
                     int charwidth,
                     int charheight,
                     int pausecolorr,
                     int pausecolorg,
                     int pausecolorb,
                     int maincolorr,
                     int maincolorg,
                     int maincolorb);
void renderingToScreen();




/**************** TEXTURE */


/*  pre:    array of rgb pixels png
    post:

    notes:  https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
*/
// https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
void createTexture(uint32_t png[], int pngwidth, int pngheight);




/**************** COLOUR */

/*  pre:
    post:

    notes: https://wiki.libsdl.org/SDL2/SDL_SetRenderDrawColor
*/
void backgroundColour(int r, int g, int b, int a);
void textureColour(int r, int g, int b);




