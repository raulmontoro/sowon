#include "event.h"

/*  event key down  */

void keyDownEvent(SDL_Event event, ClockEvent *clockevent) {

    // https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html
    switch (event.key.keysym.sym) {
        case SDLK_SPACE: {
            *clockevent = SPACE;
        } 
        break;

        case SDLK_KP_PLUS:

        case SDLK_EQUALS: {
            *clockevent = EQUALS;
        } 
        break;

        case SDLK_KP_MINUS:

        case SDLK_MINUS: {
            *clockevent = MINUS;
        } 
        break;

        case SDLK_KP_0:
        
        // in a spanish keyboard, 'equals key' is <shift+0> for zoom in
        case SDLK_0: {
            if (event.key.keysym.mod & KMOD_SHIFT) {
                *clockevent = SHIFTZERO;
            }
            else 
                *clockevent = ZERO;
        } 
        break;

        case SDLK_F5: {
            *clockevent = F5;
        } 
        break;

        case SDLK_F11: {
            *clockevent = F11;
        } 
        break;
    }
}


void mouseWheelEvent(SDL_Event event, ClockEvent *clockevent) {
    if (SDL_GetModState() & KMOD_CTRL) {
        if (event.wheel.y > 0) {
            *clockevent = WHEELUP;
        } 
        else if (event.wheel.y < 0) {
            *clockevent = WHEELDOWN;
        }
        else
            *clockevent = NONE;
    }
}




/* even loop    */
void eventLoop(int *quit,
               Event *clockevent) {

    SDL_Event event = {0};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                *quit = 1;
            } break;

            case SDL_KEYDOWN: {
                keyDownEvent(event, clockevent);

            } break;

            case SDL_MOUSEWHEEL: {
                mouseWheelEvent(event, clockevent);
            } break;

            default: {
            }
        }
    }
}


