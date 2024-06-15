void wiggleCoolDown(State *state) {
    if (state->wiggle_cooldown <= 0.0f) {
        reiniciateWiggleTime(state);
        updateWiggleTime(state);
    }
    else {
        decreaseWiggleTime(state);
    }
}

