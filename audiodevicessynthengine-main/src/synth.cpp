#include "synth.hpp"
#include "util.hpp"

/**
 * Whether or not to log debug info.
 */
bool debug = false;
bool knobMode = false;

/**
 * The daisy pod object
 */
DaisyPod pod;

/**
 * Pressure values
 */
float pressure[3] = {0.0f, 0.0f, 0.0f};

/**
 * Secondary controls, mapped to knobs
 * Knob 1 -> secondary[0]
 * Knob 2 -> secondary[1]
 */
float secondary[2] = {0.0f, 0.0f};

/**
 * Base frequency of the oscillators
 */
float baseFreq = 130.81f; // C3

// Oscillators
Oscillator modulator1;
Oscillator modulator2;
Oscillator modulator3;

Oscillator carrier1;
Oscillator carrier2;
Oscillator carrier3;

// LFOs
Oscillator lfoLow;
Oscillator lfoMid;
Oscillator lfoHigh;

// Effects
Adsr     env;
Svf      filter;
Chorus   chorus;
ReverbSc reverb;

/**
 * Handles the audio callback and processes the audio signal.
 */
void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    // TODO: Currently, not too interesting if pressure[1] is zero, as it is the
    // final pass

    // Set up LFOs and filter
    lfoLow.SetFreq(0.5f + 2.0f * pressure[1]);
    filter.SetFreq(300.0f + pressure[0] * 4000.0f + pressure[2] * 1000.0f);

    // Set up reverb
    float wetness = 0.1f * Scale(pressure[1], 6.0f);
    // Detune the frequency based on pressure[1]
    float freq = baseFreq * Scale(pressure[1], -0.1f);
    float avg  = (pressure[0] + pressure[1] + pressure[2]) / 3.0f;

    for(size_t i = 0; i < size; i++)
    {
        float sig = 0.0f;

        float slowMod = lfoLow.Process();
        float midMod  = lfoMid.Process();
        float fastMod = lfoHigh.Process();

        // clang-format off

        // Frequency modulation synthesis
        float modulation1 = 
            ProcessOsc(modulator1, freq)
                * Scale(slowMod, 2.0f);
        float modulation2 = 
            ProcessOsc(modulator2, 
                    FM(
                        freq, 
                        modulation1, 
                        pressure[1]
                    )
                )
                * Scale(slowMod, 2.0f) 
                * Scale(midMod, 2.0f);
        float modulation3 = 
            ProcessOsc(modulator3, freq * pressure[2]);

        // TODO: Not mathematically equivalent!
        // carrier1.SetFreq(freq + freq * modulation2 * amount2
        //                  + freq * modulation3 * amount3);

        float output1 =
            ProcessOsc(carrier1,
                FM(
                    FM(
                        freq,
                        modulation2, 
                        pressure[0] // More dramatic
                    ),
                    modulation3, 
                    pressure[2] // Less dramatic
                ),
                0.6f * avg
                //  * (0.8f * pressure[0] + 0.2f * avg) // Weighted volume controls
            );

        float output2 = ProcessOsc(carrier2, 
            FM(
                freq * 0.5f * Scale(fastMod, 1.0f), 
                output1,
                pressure[2]
            ),
            0.4f * pressure[2]
        );

        float output3 = ProcessOsc(carrier3, 
            freq * 2.0f * Scale(fastMod, 1.0f) * Scale(pressure[0], 8.0f),
            // FM(^
            //     freq * 4.0f * Scale(fastMod, 2.0f), 
            //     output2,
            //     pressure[1]
            // ) * 2.0f,
            0.1f * pressure[1]
        ) * Scale(midMod, 1.0f);

        sig += output1;
        sig += output2 * secondary[0]; // Knob 1 controls volume of carrier2
        sig += output3 * secondary[1]; // Knob 2 controls volume of modulator3

        // clang-format on

        // Add effects
        filter.Process(sig);
        sig = filter.Low();
        sig = chorus.Process(sig);
        ProcessReverb(reverb, sig, wetness, out, i);
    }
}

/**
 * Initialize all components of the synth
 */
void InitSynth(float sampleRate)
{
    // Initialize oscillators
    InitOscillator(modulator1, sampleRate, Oscillator::WAVE_TRI);
    InitOscillator(modulator2, sampleRate, Oscillator::WAVE_SIN);
    InitOscillator(carrier1, sampleRate, Oscillator::WAVE_SIN);
    InitOscillator(carrier2, sampleRate, Oscillator::WAVE_SAW);
    InitOscillator(carrier3, sampleRate, Oscillator::WAVE_TRI);
    InitOscillator(modulator3, sampleRate, Oscillator::WAVE_SAW);

    // Initialize LFOs
    InitOscillator(lfoLow, sampleRate, Oscillator::WAVE_SIN);
    lfoLow.SetFreq(0.5f); // Slow cycle

    InitOscillator(lfoMid, sampleRate, Oscillator::WAVE_SIN);
    lfoMid.SetFreq(4.0f); // Faster

    InitOscillator(lfoHigh, sampleRate, Oscillator::WAVE_SIN);
    lfoHigh.SetFreq(8.0f); // Fastest

    // Envelope
    env.Init(sampleRate);
    env.SetAttackTime(0.3f);
    env.SetDecayTime(1.0f);
    env.SetSustainLevel(0.9f);
    env.SetReleaseTime(2.0f);

    // Filter
    filter.Init(sampleRate);
    filter.SetFreq(1000.0f);
    filter.SetRes(0.3f);

    // Chorus
    chorus.Init(sampleRate);
    chorus.SetLfoFreq(0.20f);
    chorus.SetLfoDepth(0.45f);
    chorus.SetDelayMs(20.0f);
    chorus.SetFeedback(0.1f);

    // Reverb
    reverb.Init(sampleRate);
    reverb.SetFeedback(0.8f);
    reverb.SetLpFreq(5000.0f);
}