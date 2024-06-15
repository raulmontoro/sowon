// window width and height
void updateWindowResize(State *state) {
    windowSize(&state->w, &state->h);
}

// widow resize ratio
void updateWindowZoomInOut(State *state) {
    fitScale(state->w, state->h, &state->fit_scale);
}

