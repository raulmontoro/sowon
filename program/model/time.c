#include <time.h>
#include <math.h>

/*  rendering frames  */
#define FPS 60
#define DELTA_TIME (1.0f / FPS)

void getLocalTime(float *seconds) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    *seconds = tm->tm_sec +
               tm->tm_min*60.0f +
               tm->tm_hour*60.0f*60.0f;
}


void setDisplayTime(State *state) {
    state->displaytime = 0.0f;
}

void increaseTime(State *state) {
    state->displaytime += DELTA_TIME;
}

void decreaseTime(State *state) {
    state->displaytime -= DELTA_TIME;
}
