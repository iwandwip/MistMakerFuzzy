/*
 *  output-module.h
 *
 *  output module header
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef OUTPUT_MODULE_H
#define OUTPUT_MODULE_H

#include "Arduino.h"

class DigitalOut {
      private:
        int _pin;
        unsigned long _onDelayTime;
        unsigned long _onDelayStart;
        bool _onDelayState;
        unsigned long _offDelayTime;
        unsigned long _offDelayStart;
        bool _offDelayState;
        bool _state;
        bool _reverse;

      public:
        DigitalOut();
        DigitalOut(int pin, bool isReverse = false);
        void init();
        void on();
        void off();
        void set(bool state);
        void toggle();
        void onDelay(unsigned long delay_time);
        void offDelay(unsigned long delay_time);
        void update();
        void reverse();
        void setPins(uint8_t pin, bool isReverse = false);
        bool getState();
};

#endif  // OUTPUT_MODULE_H