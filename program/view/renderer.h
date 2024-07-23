/**************** TEXTURE */


/*  pre:    array of rgb pixels png
    post:

    notes:  https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
*/
// https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
void createClockRendering(uint32_t png[], int pngwidth, int pngheight);






/**************** RENDERER */


void createRendering(size_t wiggle_index, 
                     int pen_x,
                     int pen_y,
                     float windowscale,
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








