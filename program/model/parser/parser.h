/*  https://github.com/cofyc/argparse/tree/master
    https://cplusplus.com/reference/cstring

        -m clock
           stopwatch 
           countdown

        -t 1h30m00s
        -t 1h 30m 00s
        -t 01:30:00

        -mclock-t1h30m15s
*/





typedef enum Mode {
    CLOCK,
    STOPWATCH,
    COUNTDOWN,
} Mode;

typedef struct Arguments {
    int flag_m;
    int flag_t;
    int hours;
    int minutes;
    int seconds;
    Mode mode;
} Arguments;


void parser(int argc, char **argv, Arguments *a);
