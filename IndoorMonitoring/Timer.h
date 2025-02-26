#ifndef __timer_h
#define __timer_h


class Timer{
  public:
    short seconds;
    
    // Constructor
    Timer();
    
    // Constructor with an option to set the seconds to sleep.
    // Choices: 4 or 8 (default)
    Timer(short seconds); // Initializer

    // Initializes Deep Sleep Time
    void init(short seconds);

    // Re-enables the timer
    void reset();

    // Disables I2C and makes the microcontroller sleep for X seconds. 
    // Reenables I2C once it wakes up
    void deepSleep();
};

#endif // __timer_h

