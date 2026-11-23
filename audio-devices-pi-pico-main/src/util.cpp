#include "util.hpp"

void sendCC(uint8_t cc, uint8_t value)
{
    SerialMidi.write(0xB0);
    SerialMidi.write(cc);
    SerialMidi.write(value);
}

int map_safe(int x, int in_min, int in_max, int out_min, int out_max)
{
    if (x < in_min)
        return out_min;
    if (x > in_max)
        return out_max;
    return map(x, in_min, in_max, out_min, out_max);
}