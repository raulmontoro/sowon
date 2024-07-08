void wiggleCoolDown(State *state) {
    if (state->wiggle_cooldown <= 0.0f) {
        state->wiggle_index++;
        state->wiggle_cooldown = WIGGLE_DURATION;
    }
    else {
        state->wiggle_cooldown -= DELTA_TIME;
    }
}
