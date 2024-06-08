#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>

#include "./digits.h"

#define FPS 60
#define DELTA_TIME (1.0f / FPS)
#define SPRITE_CHAR_WIDTH (300 / 2)
#define SPRITE_CHAR_HEIGHT (380 / 2)
#define CHAR_WIDTH (300 / 2)
#define CHAR_HEIGHT (380 / 2)
#define CHARS_COUNT 8
#define TEXT_WIDTH (CHAR_WIDTH * CHARS_COUNT)
#define TEXT_HEIGHT (CHAR_HEIGHT)
#define WIGGLE_COUNT 3
#define WIGGLE_DURATION (0.40f / WIGGLE_COUNT)
#define COLON_INDEX 10
#define MAIN_COLOR_R 220
#define MAIN_COLOR_G 220
#define MAIN_COLOR_B 220
#define PAUSE_COLOR_R 220
#define PAUSE_COLOR_G 120
#define PAUSE_COLOR_B 120
#define BACKGROUND_COLOR_R 24
#define BACKGROUND_COLOR_G 24
#define BACKGROUND_COLOR_B 24
#define SCALE_FACTOR 0.15f

#define TITLE_CAP 256

typedef enum {
    MODE_ASCENDING = 0,
    MODE_COUNTDOWN,
    MODE_CLOCK,
} Mode;

typedef struct Config {
    Mode mode;
    float displayed_time;
    int paused;
    int exit_after_countdown;
    size_t wiggle_index;
    float wiggle_cooldown;
    float user_scale;
    char prev_title[TITLE_CAP];
} Config;



// ERROR HANDLER

void secc(int code)
{
    if (code < 0) {
        fprintf(stderr, "SDL pooped itself: %s\n", SDL_GetError());
        abort();
    }
}

void secp(void *ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "SDL pooped itself: %s\n", SDL_GetError());
        abort();
    }
}






// width, height
void initial_pen(SDL_Window *window,
                 int *pen_x, int *pen_y, 
                 float user_scale, float *fit_scale) {
    
    // window size
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // width/height ratio
    float text_aspect_ratio = (float) TEXT_WIDTH / (float) TEXT_HEIGHT;
    float window_aspect_ratio = (float) w / (float) h;

    if(text_aspect_ratio > window_aspect_ratio) {
        *fit_scale = (float) w / (float) TEXT_WIDTH;
    } else {
        *fit_scale = (float) h / (float) TEXT_HEIGHT;
    }

    const int effective_digit_width = (int) floorf((float) CHAR_WIDTH * user_scale * *fit_scale);
    const int effective_digit_height = (int) floorf((float) CHAR_HEIGHT * user_scale * *fit_scale);

    *pen_x = w / 2 - effective_digit_width * CHARS_COUNT / 2;
    *pen_y = h / 2 - effective_digit_height / 2;
}




void render_digit_at(SDL_Renderer *renderer, 
                     SDL_Texture *digits, 
                     size_t digit_index,
                     size_t wiggle_index,
                     int *pen_x, int *pen_y,
                     float user_scale, float fit_scale) {

    const int effective_digit_width = (int) floorf((float) CHAR_WIDTH * user_scale * fit_scale);
    const int effective_digit_height = (int) floorf((float) CHAR_HEIGHT * user_scale * fit_scale);

    const SDL_Rect src_rect = {
        (int) (digit_index * SPRITE_CHAR_WIDTH),
        (int) (wiggle_index * SPRITE_CHAR_HEIGHT),
        SPRITE_CHAR_WIDTH,
        SPRITE_CHAR_HEIGHT
    };

    const SDL_Rect dst_rect = {
        *pen_x,
        *pen_y,
        effective_digit_width,
        effective_digit_height
    };
    SDL_RenderCopy(renderer, digits, &src_rect, &dst_rect);
    *pen_x += effective_digit_width;
}





float parse_time(const char *time) {
    float result = 0.0f;

    while (*time) {
        char *endptr = NULL;
        float x = strtof(time, &endptr);

        if (time == endptr) {
            fprintf(stderr, "`%s` is not a number\n", time);
            exit(1);
        }

        switch (*endptr) {
            case '\0':

            // seconds
            case 's':
                result += x;
                break;
            // minutes
            case 'm':
                result += x * 60.0f;
                break;
            // hours
            case 'h':
                result += x * 60.0f * 60.0f;
                break;

            default:
                fprintf(stderr, "`%c` is an unknown time unit\n", *endptr);
                exit(1);
        }

        time = endptr;
        if (*time) time += 1;
    }

    return result;
}

void createWindow(SDL_Window **window) {
    *window = SDL_CreateWindow(
                     "sowon",
                     0, 0, TEXT_WIDTH, TEXT_HEIGHT,
                     SDL_WINDOW_RESIZABLE);
    secp(window);
}

void createRenderer(SDL_Window *window, SDL_Renderer **renderer) {
    *renderer = SDL_CreateRenderer(window, -1,
                 SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    secp(renderer);
}



SDL_Texture *createTextureFromFile(SDL_Renderer *renderer)
{
   SDL_Surface* image_surface;
    image_surface =  SDL_CreateRGBSurfaceFrom(
                        png,
                        (int) png_width,
                        (int) png_height,
                        32,
                        (int) png_width * 4,
                        0x000000FF,
                        0x0000FF00,
                        0x00FF0000,
                        0xFF000000);

    secp(image_surface);
    
    SDL_Texture *digits = SDL_CreateTextureFromSurface(renderer, image_surface);

    secp(digits);
    secc(SDL_SetTextureColorMod(digits, MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B));
    return digits;
}

int main(int argc, char **argv) {
    secc(SDL_Init(SDL_INIT_VIDEO));
    secc(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"));

    // CREATE WINDOW
    SDL_Window *window;
    createWindow(&window);

    

    // CREATE RENDERER
    SDL_Renderer *renderer;
    createRenderer(window, &renderer);

    // CREATE DIGITS TEXTURE
    SDL_Texture *digits = createTextureFromFile(renderer);


   




    // configuration 
    Config config = {MODE_ASCENDING,  0.0f, 0, 0, 0, WIGGLE_DURATION, 1.0f, "hello world"};

    
    // initial configuration
    // Mode mode = MODE_ASCENDING;
    // float displayed_time = 0.0f;
    // int paused = 0;
    // int exit_after_countdown = 0;
    // 
    // size_t wiggle_index = 0;
    // float wiggle_cooldown = WIGGLE_DURATION;
    // float user_scale = 1.0f;
    // char prev_title[TITLE_CAP];


    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-p") == 0) {
            config.paused = 1;
        } else if (strcmp(argv[i], "-e") == 0) {
            config.exit_after_countdown = 1;
        } else if (strcmp(argv[i], "clock") == 0) {
            config.mode = MODE_CLOCK;
        } else {
            config.mode = MODE_COUNTDOWN;
            config.displayed_time = parse_time(argv[i]);
        }
    }



    int quit = 0;
    while (!quit) {

        if (config.paused) {
            secc(SDL_SetTextureColorMod(digits, PAUSE_COLOR_R, PAUSE_COLOR_G, PAUSE_COLOR_B));
        } else {
            secc(SDL_SetTextureColorMod(digits, MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B));
        }

        // INPUT BEGIN //////////////////////////////
        SDL_Event event = {0};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = 1;
                } break;

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                    case SDLK_SPACE: {
                        config.paused = !config.paused;
                    } break;

                    case SDLK_KP_PLUS:
                    case SDLK_EQUALS: {
                        config.user_scale += SCALE_FACTOR * config.user_scale;
                    } break;

                    case SDLK_KP_MINUS:
                    case SDLK_MINUS: {
                        config.user_scale -= SCALE_FACTOR * config.user_scale;
                    } break;

                    case SDLK_KP_0:
                    case SDLK_0: {
                        config.user_scale = 1.0f;
                    } break;

                    case SDLK_F5: {
                        config.displayed_time = 0.0f;
                        config.paused = 0;
                        for (int i = 1; i < argc; ++i) {
                            if (strcmp(argv[i], "-p") == 0) {
                                config.paused = 1;
                            } else {
                                config.displayed_time = parse_time(argv[i]);
                            }
                        }
                        if (config.paused) {
                            secc(SDL_SetTextureColorMod(digits, PAUSE_COLOR_R, PAUSE_COLOR_G, PAUSE_COLOR_B));
                        } else {
                            secc(SDL_SetTextureColorMod(digits, MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B));
                        }
                    } break;

                    case SDLK_F11: {
                        Uint32 window_flags;
                        secc(window_flags = SDL_GetWindowFlags(window));
                        if(window_flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                            secc(SDL_SetWindowFullscreen(window, 0));
                        } else {
                            secc(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP));
                        }
                    } break;
                    }
                } break;

                case SDL_MOUSEWHEEL: {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        if (event.wheel.y > 0) {
                            config.user_scale += SCALE_FACTOR * config.user_scale;
                        } else if (event.wheel.y < 0) {
                            config.user_scale -= SCALE_FACTOR * config.user_scale;
                        }
                    }
                } break;

                default: {}
            }
        }

        // INPUT END //////////////////////////////

        // RENDER BEGIN //////////////////////////////
        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 255);
        SDL_RenderClear(renderer);
        {
            int pen_x, pen_y;
            float fit_scale = 1.0;
            initial_pen(window, &pen_x, &pen_y, config.user_scale, &fit_scale);

            const size_t t = (size_t) ceilf(fmaxf(config.displayed_time, 0.0f));

            // TODO: support amount of hours >99
            const size_t hours = t / 60 / 60;
            render_digit_at(renderer, digits, hours / 10,   config.wiggle_index      % WIGGLE_COUNT, &pen_x, &pen_y, config.user_scale, fit_scale);
            render_digit_at(renderer, digits, hours % 10,  (config.wiggle_index + 1) % WIGGLE_COUNT, &pen_x, &pen_y, config.user_scale, fit_scale);
            render_digit_at(renderer, digits, COLON_INDEX,  config.wiggle_index      % WIGGLE_COUNT, &pen_x, &pen_y, config.user_scale, fit_scale);

            const size_t minutes = t / 60 % 60;
            render_digit_at(renderer, digits, minutes / 10, (config.wiggle_index + 2) % WIGGLE_COUNT, &pen_x, &pen_y, config.user_scale, fit_scale);
            render_digit_at(renderer, digits, minutes % 10, (config.wiggle_index + 3) % WIGGLE_COUNT, &pen_x, &pen_y, config.user_scale, fit_scale);
            render_digit_at(renderer, digits, COLON_INDEX,  (config.wiggle_index + 1) % WIGGLE_COUNT, &pen_x, &pen_y, config.user_scale, fit_scale);

            const size_t seconds = t % 60;
            render_digit_at(renderer, digits, seconds / 10, (config.wiggle_index + 4) % WIGGLE_COUNT, &pen_x, &pen_y, config.user_scale, fit_scale);
            render_digit_at(renderer, digits, seconds % 10, (config.wiggle_index + 5) % WIGGLE_COUNT, &pen_x, &pen_y, config.user_scale, fit_scale);

            char title[TITLE_CAP];
            snprintf(title, sizeof(title), "%02zu:%02zu:%02zu - sowon", hours, minutes, seconds);
            if (strcmp(config.prev_title, title) != 0) {
                SDL_SetWindowTitle(window, title);
            }
            memcpy(title, config.prev_title, TITLE_CAP);
        }
        SDL_RenderPresent(renderer);
        // RENDER END //////////////////////////////

        // UPDATE BEGIN //////////////////////////////
        if (config.wiggle_cooldown <= 0.0f) {
            config.wiggle_index++;
            config.wiggle_cooldown = WIGGLE_DURATION;
        }
        config.wiggle_cooldown -= DELTA_TIME;

        if (!config.paused) {
            switch (config.mode) {
            case MODE_ASCENDING: {
                config.displayed_time += DELTA_TIME;
            } break;
            case MODE_COUNTDOWN: {
                if (config.displayed_time > 1e-6) {
                    config.displayed_time -= DELTA_TIME;
                } else {
                    config.displayed_time = 0.0f;
                    if (config.exit_after_countdown) {
                        SDL_Quit();
                        return 0;
                    }
                }
            } break;
            case MODE_CLOCK: {
                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                config.displayed_time = tm->tm_sec
                               + tm->tm_min  * 60.0f
                               + tm->tm_hour * 60.0f * 60.0f;
            } break;
            }
        }
        // UPDATE END //////////////////////////////

        SDL_Delay((int) floorf(DELTA_TIME * 1000.0f));
    }

    SDL_Quit();

    return 0;
}
