void reiniciateWiggleTime(State *state) {
    state->wiggle_cooldown = WIGGLE_DURATION;
}

void decreaseWiggleTime(State *state) {
    state->wiggle_cooldown -= DELTA_TIME;
}

void updateWiggleIndex(State *state) {
    state->wiggle_index++;
}
