/***********  RENDERER *************/

void createRenderer() {

    renderer = SDL_CreateRenderer(window, -1,
               SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    secp(renderer);
}


/***********  TEXTURE *************/

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




void clearRenderer() {
        SDL_RenderClear(renderer);
}


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
                    zoomscale, 
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

void renderingToScreen() {
        SDL_RenderPresent(renderer);
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






void colourBackground(int r, int g, int b, int a) {
    // black background color 
    SDL_SetRenderDrawColor(renderer, (Uint8)r , (Uint8)g, (Uint8)b, (Uint8)a);
}

void textureColour(int r, int g, int b) {
        // texture colour, digits
        secc(SDL_SetTextureColorMod(texture, r, g, b));
}

