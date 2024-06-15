#include <signal.h>
#include <time.h>


/*  REFERENCES 
    date and time c library 
    https://en.cppreference.com/w/c/chrono
    https://en.cppreference.com/w/c/chrono/timespec_get

    hardware timer, sigalarm
    https://stackoverflow.com/questions/3856841/using-hardware-timer-in-c
    
    getitimer()
    https://man7.org/linux/man-pages/man2/setitimer.2.html

    timer_create()
    https://man7.org/linux/man-pages/man2/timer_create.2.html

    timerfd_create()
    https://man7.org/linux/man-pages/man2/timerfd_create.2.html

    $ man 7 sigevent 
    $ man 7 signal

    example
    https://stackoverflow.com/questions/49860625/send-signal-at-specific-time-in-linux-c
*/


/*  waiting for a signal to be caught
        paused()
            suspends execution until any signal is caught

        sigsuspend()
*/

/*  accepting a signal synchronously
        sigtimedwait()
*/



/*  signal handler
    catching the signal
        changing the default disposition of a signal:
            Term, 
            Ign, 
            Core, 
            Stop, 
            Cont
*/
sigaction


/******************* CREATING TIMERS *******************/

/*  signal to send on timer expiration
*/
struct sigevent sevp = {
    SIGEV_SIGNAL,
    SIGUSR1,    // or SIGALARM
    NULL,
    NULL,
    NULL,
};







/*  pre:    clockid_t clockid
                CLOCK_REALTIME
            struct sigevent *sevp
                how the caller should be notified
                when the timer expires

                sevp.sigev
                    notify field
                        SIGEV_SIGNAL
                            upon timer expiration,
                            generates the signal sigev signo

            timer_t *timerid
                and new timer id in *timerid

    post:   creates
            a per-process interval timer

            return 
                on success  0
                and new timer id in *timerid

                on failure -1 and errno set to indicate error
*/
void createTimer(clockid_t clockid,
                 struct sigevent *sevp, 
                 timer_t *timerid) {
    timer_create(clockid, sevp, timerid);
}






/******************* SETTING TIMERS *******************/

/*  setting timers
    https://www3.physnet.uni-hamburg.de/physnet/Tru64-Unix/HTML/APS33DTE/DOCU_007.HTM#sig-and-timers-sec
*/

/*  tv_sec*1e9 ns + tv_nsec     nanoseconds
*/
typedef struct timespec {
    time_t tv_sec;  // Seconds
    long tv_nsec;   // Nanoseconds
} Timespec_t;

Timespec_t timespec = { 1,      
                        23e-9 
                      };


/*  both fields define one, only, period of time
    it_value
        0       timer disarmed
        non-0   timer armed
                timer first expiration time

    it_interval
        0       no reload timer
                one-shot timer
        non-0   periodic timer
    flag
        TIMER_ABSTIME
            makes timer an absolute timer
            difference with Epoch
        no-flag
            makes timer a relative timer
            it_interval used
*/
struct itimerspec {
    struct timespec it_interval;    // timer interval
    struct timespec it_value;       // initial expiration
};




/*  pre:
    post:
*/
void setTimer(timer_t timerid, 
              int flags, 
              const struct itimerspec *new_value,
              struct itimerspec *old_value) {

    timer_settime(
        timerid, 
        flags, 
        new_value,
        old_value);
}

void getTimer() {
    timer_gettime();
}

void destroyTimer() {
}







/******************* MAIN *******************/

int main() {
}
