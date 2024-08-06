typedef enum Mode {
    MODE_CLOCK,
    MODE_STOPWATCH,
    MODE_COUNTDOWN,
} Mode;

typedef struct Arguments {
    int flag_m;
    int flag_t;
    int hours;
    int minutes;
    int seconds;
    Mode mode;
} Arguments;
