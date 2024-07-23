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
            state->zoomscale += SCALE_FACTOR * state->zoomscale;
            break;

        case WHEELDOWN:
            state->zoomscale -= SCALE_FACTOR * state->zoomscale;
            break;
    }
}
