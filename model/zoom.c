/*  zoom   */

void reiniciateZoom(State *state) {
    state->user_scale = 1.0f;
}

/* pre:
   post: zoom in, represente in 'user_scale'
*/
void zoomIn(State *state) {
    state->user_scale += SCALE_FACTOR*state->user_scale;
}

void zoomOut(State *state) {
    state->user_scale -= SCALE_FACTOR*state->user_scale;
}
