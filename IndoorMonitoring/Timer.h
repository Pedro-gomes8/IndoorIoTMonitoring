#ifndef __timer_h
#define __timer_h


class Timer{
  public:
    short seconds;
    Timer();
    Timer(short seconds); 

    // Initializes watchdog
    void init(short seconds);

    // Re-enables the timer
    void reset();

    // Sleep 
    void deepSleep();
};

#endif // __timer_h

