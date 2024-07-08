typedef struct WinConfig {
    char *title;
    int positionx;
    int positiony;
    int width;
    int height;
    SDL_WindowFlags flags;
} WinConfig;

void createWindow(int windowwidth, int windowheight) {

    WinConfig winconfig = {"sowon",
                           0,
                           0,
                           windowwidth,
                           windowheight,
                           SDL_WINDOW_RESIZABLE};

    window = SDL_CreateWindow(winconfig.title,
                              winconfig.positionx, 
                              winconfig.positiony, 
                              winconfig.width,
                              winconfig.height,
                              winconfig.flags);
    secp(window);
}


/*  pre:
    post:

    notes:  https://wiki.libsdl.org/SDL2/SDL_SetWindowTitle
*/
void windowTitle(const char *str) {
            SDL_SetWindowTitle(window, str);
}


void windowSize(int *w, int *h) {
    SDL_GetWindowSize(window, w, h);
}

void fullScreenToggle() {
    Uint32 window_flags;
    secc(window_flags = SDL_GetWindowFlags(window));

    if(window_flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        secc(SDL_SetWindowFullscreen(window, 0));
    } 
    else {
        secc(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP));
    }
}


