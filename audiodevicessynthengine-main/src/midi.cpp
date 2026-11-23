#include "midi.hpp"
#include "synth.hpp"

/**
 * Listen for midi events and accept control changes
 */
void HandleMidi(MidiEvent m)
{
    if(debug)
        pod.seed.PrintLine("MIDI Received!: %d, Type: %d", m.channel, (int)m.type);

    if (knobMode)
        return;

    if(m.type == MidiMessageType::ControlChange)
    {
        /**
         * Sensor 0 -> Channel 20
         * Sensor 1 -> Channel 21
         * Sensor 2 -> Channel 22
         */
        auto cc      = m.AsControlChange();
        auto control = cc.control_number;
        if(control >= 20 && control <= 22)
        {
            int index       = control - 20;
            pressure[index] = 1.0f - (cc.value / 127.0f);
        }

        if(debug)
            pod.seed.PrintLine(
                "Control Change: %d, Value: %d", control, cc.value);
    }
}