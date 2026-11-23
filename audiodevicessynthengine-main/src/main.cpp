#include "synth.hpp"
#include "midi.hpp"
#include "controls.hpp"

/**
 * The main entry point of the synth engine.
 */
int main(void)
{
    // Initialize pod
    pod.Init();
    pod.SetAudioBlockSize(4);
    pod.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    pod.StartAdc();

    // Initialize the synth
    InitSynth(pod.AudioSampleRate());

    // Start the audio callback
    pod.StartAudio(AudioCallback);

    if (debug) {
        pod.seed.StartLog(false);
    }

    MidiUartHandler::Config midi_config;
    pod.midi.Init(midi_config);
    pod.midi.StartReceive();

    // Main loop
    while(true)
    {
        // Listen to MIDI
        pod.midi.Listen();

        while(pod.midi.HasEvents())
        {
            HandleMidi(pod.midi.PopEvent());
        }

        // Handle controls
        pod.ProcessAllControls();
        HandleControls();
    }
}