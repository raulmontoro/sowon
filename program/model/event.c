void eventClock(ClockEvent clockevent) {

    switch(clockevent) {
        case NONE:
            break;

        case R:
            zoomInitial(state);
            state = initstate;
            break;

        case P:
            pauseToggle(state);
            break;

        case SPACE:
            pauseToggle(state);
            break;

        case A:
            break;

        case D:
            break;

        case W:
            zoomIn(state);
            break;

        case S:
            zoomOut(state);
            break;

        case F:
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
