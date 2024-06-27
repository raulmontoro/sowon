#include <stdint.h>
#include <stdlib.h>

void createWindow(int windowwidth, int windowheight);
void createRenderer();
void createTexture(uint32_t png[], int pngwidth, int pngheight);

void backgroundColour(int r, int g, int b, int a);
void textureColour(int r, int g, int b);
void clearRenderer();
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
                     int maincolorb);

void renderingToScreen();
void fullScreenToggle();
void delayInfiniteLoop();
void quitSDL();


void windowTitle(const char *str);
void renderingToScreen();
void initSDL(int windowwidth, 
             int windowheight,
             uint32_t png[], 
             size_t png_width,
             size_t png_height);
