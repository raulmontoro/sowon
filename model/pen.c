void updatePen(State *state) {
    initial_pen(state->w,
                state->h,
                state->user_scale,
                state->fit_scale,
                &state->pen_x, 
                &state->pen_y);
    
}
