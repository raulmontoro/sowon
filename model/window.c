/********** WINDOW **********/

void updateWindowResizeAndZoomInOut(State *state) {
    // window width and height
    windowSize(&state->w, &state->h);
    
    // widow resize ratio
    fitScale(state->w, state->h, &state->fit_scale);
        
}
