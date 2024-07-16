void getLocalTime(float *seconds) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    *seconds = tm->tm_sec
               + tm->tm_min  * 60.0f
               + tm->tm_hour * 60.0f * 60.0f;
}


void setDisplayTime(State *state) {
    state->displayed_time = 0.0f;
}

void increaseTime(State *state) {
    state->displayed_time += DELTA_TIME;
}

void decreaseTime(State *state) {
    state->displayed_time -= DELTA_TIME;
}
