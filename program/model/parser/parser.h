typedef enum Mode {
    MODE_CLOCK,
    MODE_STOPWATCH,
    MODE_COUNTDOWN,
} Mode;

typedef struct Arguments {
    int flag_m;
    Mode mode;

    int flag_h;
    int hours;

    int flag_m;
    int minutes;

    int flag_s;
    int seconds;
} Arguments;
