#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <Arduino.h>

#define COUNT_FALLING 0
#define COUNT_RISING 1
#define COUNT_BOTH 2

class DigitalIn {
       private:
        int btnPin;
        unsigned long debounceTime;
        unsigned long count;
        int countMode;

        int previousSteadyState;
        int lastSteadyState;
        int lastFlickerableState;

        unsigned long lastDebounceTime;

       public:
        DigitalIn(int pin);
        DigitalIn(int pin, int mode);
        void setDebounceTime(unsigned long time);
        int getState(void);
        int getStateRaw(void);
        bool isPressed(void);
        bool isReleased(void);
        void setCountMode(int mode);
        unsigned long getCount(void);
        void resetCount(void);
        void loop(void);
};

#endif  // DIGITAL_INPUT_H
