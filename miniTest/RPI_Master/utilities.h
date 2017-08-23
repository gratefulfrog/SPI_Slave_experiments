#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <time.h>

// define functions here for various forms of processing of the data coming from the boards


extern void delay(time_t millisecs);            /*!< emulates Arduino 'delay' function, but will not work if arg is too big!, use 'delaySeconds' instead */
extern void delayMicroseconds(long microsecs);  /*!< emulates Arduino 'delayMicroseconds' function */
extern void delaySeconds(time_t secs);          /*!< emulates Arduino 'delay' function, but for seconds not millis */
extern void delayFull(time_t secs, long micros); /*!< encapsulates the call to nanosleep, with seconds and micro-seconds */

#endif
