// window width and height
void updateWindowResize(State *state) {
    windowSize(&state->w, &state->h);
}

// widow resize ratio
void updateWindowZoomInOut(State *state) {
    fitScale(state->w, state->h, &state->fit_scale);
}


/*  pre:    
    post:   cartesian coordinates
            position where rendering starts 
            to fit CHAR_COUNT characters 
            at user_scale*fit_scale scale
*/
void setPen(int w, 
            int h,
            float user_scale,
            float fit_scale, 

            int *pen_x,
            int *pen_y) {
    
    // character width after scaling 
    const int effective_digit_width = (int)floorf(
                                            (float)CHAR_WIDTH*user_scale*fit_scale
                                      );
    // character height after scaling
    const int effective_digit_height = (int)floorf(
                                            (float)CHAR_HEIGHT*user_scale*fit_scale
                                       );
    
    // position where rendering starts 
    *pen_x = w/2 - effective_digit_width*CHARS_COUNT/2;
    *pen_y = h/2 - effective_digit_height/2;
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





/*  zoom   */

void reiniciateZoom(State *state) {
    state->user_scale = 1.0f;
}

/* pre:
   post: zoom in, represente in 'user_scale'
*/
void zoomIn(State *state) {
    state->user_scale += SCALE_FACTOR*state->user_scale;
}

void zoomOut(State *state) {
    state->user_scale -= SCALE_FACTOR*state->user_scale;
}
