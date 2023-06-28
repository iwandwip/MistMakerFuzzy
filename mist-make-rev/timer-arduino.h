/*
 *  timer-arduino.h
 *
 *  arduino timer lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef TIMER_ARDUINO_H
#define TIMER_ARDUINO_H

#include "Arduino.h"

class TimerArduino {
      public:
        TimerArduino();
        void setDuration(unsigned long interval);
        void start();
        bool isExpired();
        void reset();
        int getHours();
        int getMinutes();
        int getSeconds();
        unsigned long getDuration();

      private:
        unsigned long _startMillis;
        unsigned long _interval;  // in milliseconds
};

#endif