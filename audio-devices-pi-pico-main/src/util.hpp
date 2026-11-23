#ifndef UTIL_HPP
#define UTIL_HPP

#include <Arduino.h>

#define SerialMidi Serial2

/**
 * Sends a midi control change message over the midi connection
 *
 * @param cc The channel number
 * @param value The value of the control change
 */
void sendCC(uint8_t cc, uint8_t value);

/**
 * Maps an integer value from one range to another, but clamps the output to the output range.
 */
int map_safe(int x, int in_min, int in_max, int out_min, int out_max);

#endif