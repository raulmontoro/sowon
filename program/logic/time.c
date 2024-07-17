void updateTime(State *state) {
    if (!state->paused) {
        switch (state->mode) {
            case MODE_STOPWATCH: {
                increaseTime(state);
            } 
            break;
        
            case MODE_COUNTDOWN: {
                if (state->displayed_time > 1e-6) {
                    decreaseTime(state);
                } 
                else {
                    setDisplayTime(state);
                    if (state->exit_count_down) {
                        quitSDL();
                    }
                }
            } 
            break;
        
            case MODE_CLOCK: {
                getLocalTime(state);
            } 
            break;
        }
    }
}
