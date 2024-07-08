/************************************************ FRAME LOOP */
void frameLoop(State *state) {

    int quit = 0;
    while (!quit) {

        /*  time string 00:00:00\0 
        */
        char timestr[9];
        hoursMinutesSeconds(&state, timestr);
        
        
        /* render */
        timeTitle(state.prev_title, TITLE_CAP, timestr, state);




        backgroundColour(BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 255);
        
        if (state.paused) {
            textureColour(PAUSE_COLOR_R, PAUSE_COLOR_G, PAUSE_COLOR_B);
        }
        else {
            textureColour(MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B);
        }

        clearRenderer();
        createRendering(state.wiggle_index, 
                        state.pen_x, 
                        state.pen_y, 
                        state.fit_scale, 
                        state.user_scale, 
                        state.paused, 
                        timestr,
                        CHAR_WIDTH,
                        CHAR_HEIGHT,
                        PAUSE_COLOR_R,
                        PAUSE_COLOR_G,
                        PAUSE_COLOR_B,
                        MAIN_COLOR_R,
                        MAIN_COLOR_G,
                        MAIN_COLOR_B,
                        WIGGLE_COUNT);

        renderingToScreen();
        


        /*  events */
        Event event = NONE;
        eventLoop(&quit, &event);
        eventCompute(initstate, &state, event);


        /* update   */
        updateWindowResizeAndZoomInOut(&state);
        updatePen(&state);
        wiggleCoolDown(&state);
        updateTime(&state);
        
        delayInfiniteLoop(DELTA_TIME);

    }

    quitSDL();
}
