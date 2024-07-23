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
             float zoomscale, 
             float windowscale, 
             SDL_Rect *dst_rect,
             int charwidth,
             int charheight) {

   // RESIZING DIGIT 
   // transforms digit chosen form image
   // new dimensions
   const int effective_digit_width = (int) floorf((float) charwidth * zoomscale * windowscale);
   const int effective_digit_height = (int) floorf((float) charheight * zoomscale * windowscale);


   *dst_rect = (SDL_Rect){*pen_x,
                          pen_y,
                          effective_digit_width,
                          effective_digit_height};

    // new cartesian coordinates of next new digit
   *pen_x += effective_digit_width;
}














void createClockRendering(uint32_t imagepixel[],
                          size_t imagepixel_width,
                          size_t imagepixel_height,

                          size_t wiggle_index, 
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
                          int maincolorb,
                          int wigglecount) {


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



    // https://wiki.libsdl.org/SDL2/CategorySurface
    SDL_Surface* image_surface = createSurfaceFromRGB(imagepixel,
                                                      imagepixel_width,
                                                      imagepixel_height);

    secp(image_surface);
    
    SDL_Texture texture = createTexureFromSurface(renderer, image_surface);

    secp(texture);

        SDL_Rect src_rect;
        SDL_Rect dst_rect;
       

        for (int i = 0; i<8; ++i) {
            srcRect(timestr[i],
                    (wiggle_index + i)%wigglecount,
                    &src_rect);

            dstRect(&pen_x,
                    pen_y,
                    zoomscale, 
                    windowscale, 
                    &dst_rect,
                    charwidth,
                    charheight);

        /*  choosing subimage from and image    
        and resizing */
       // ADDS EACH NEW DIGIT TO RENDERER, ONE BY ONE
       SDL_RenderCopy(renderer, digits, src_rect, dst_rect);

    int r, g, b, a;
    // black background color 
    /*  notes: https://wiki.libsdl.org/SDL2/SDL_SetRenderDrawColor
    */
    SDL_SetRenderDrawColor(renderer, (Uint8)r , (Uint8)g, (Uint8)b, (Uint8)a);
    // texture colour, digits
    secc(SDL_SetTextureColorMod(texture, r, g, b));


        if (paused) {
            secc(SDL_SetTextureColorMod(texture, pausecolorr, pausecolorg, pausecolorb));
        }
        else {
            secc(SDL_SetTextureColorMod(texture, maincolorr, maincolorg, maincolorb));
        }


}






