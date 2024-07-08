/*  event key down  compute */
void eventCompute(State *initstate,
                       State *state, 
                       Event event) {

    switch(clockevent) {
        case NONE:
            break;

        case SPACE:
            pauseToggle(state);
            break;

        case EQUALS:
            zoomInitial(state);
            break;

        case MINUS:
            zoomOut(state);
            break;

        case ZERO:
            zoomInitial(state);
            break;

        case SHIFTZERO:
            zoomIn(state);
            break;

        case F5:
            state = initstate;
            break;
        case F11:
            fullScreenToggle();
            break;

        case WHEELUP:
            state->user_scale += SCALE_FACTOR * state->user_scale;
            break;

        case WHEELDOWN:
            state->user_scale -= SCALE_FACTOR * state->user_scale;
            break;
    }
}



