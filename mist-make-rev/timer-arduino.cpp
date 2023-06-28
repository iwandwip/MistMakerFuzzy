/*
 *  timer-arduino.cpp
 *
 *  arduino timer c
 *  Created on: 2023. 4. 3
 */

#include "timer-arduino.h"

#define TICKS 100

TimerArduino::TimerArduino() {
        _startMillis = millis();
        _interval = 0;
}

void TimerArduino::setDuration(unsigned long interval) {
        _interval = interval * 1000;  // convert to milliseconds
}

void TimerArduino::start() {
        _startMillis = millis();
}

bool TimerArduino::isExpired() {
        return (millis() - _startMillis >= _interval);
}

void TimerArduino::reset() {
        _startMillis = millis();
}

int TimerArduino::getHours() {
        unsigned long elapsedTime = millis() - _startMillis;
        return (elapsedTime / 1000) / 3600;
}

int TimerArduino::getMinutes() {
        unsigned long elapsedTime = millis() - _startMillis;
        return ((elapsedTime / 1000) / 60) % 60;
}

int TimerArduino::getSeconds() {
        unsigned long elapsedTime = millis() - _startMillis;
        return (elapsedTime / 1000) % 60;
}

unsigned long TimerArduino::getDuration() {
        return _interval / 1000;  // convert back to seconds
}
