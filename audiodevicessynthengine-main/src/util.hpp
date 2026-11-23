#pragma once

/**
 * Initializes an oscillator with the given sample rate and waveform.
 * Uses the default amplitude of 1.0.
 * 
 * @param osc The oscillator to initialize.
 * @param sampleRate The sample rate to use for the oscillator.
 * @param waveform The waveform to set for the oscillator.
 */
inline void InitOscillator(Oscillator &osc, float sampleRate, int waveform)
{
    osc.Init(sampleRate);
    osc.SetWaveform(waveform);
    osc.SetAmp(1.0f);
}

/**
 * Clamps a value between a minimum and maximum range.
 * 
 * @param value The value to clamp.
 * @param min The minimum value to clamp to.
 * @param max The maximum value to clamp to.
 */
inline float clamp(float value, float min, float max)
{
    if(value < min)
        value = min;
    else if(value > max)
        value = max;
    return value;
}

/**
 * Scales a signal by a given depth.
 * 
 * @param signal The signal to scale.
 * @param depth The depth to scale by.
 */
inline float Scale(float signal, float depth)
{ return 1.0f + depth * signal; }

/**
 * Frequency modulates a base frequency with a modulation signal and an amount.
 * 
 * @param freq The base frequency to modulate.
 * @param modulation The modulation signal to apply.
 * @param amount The amount of modulation to apply.
 */
inline float FM(float freq, float modulation, float amount)
{ return freq + freq * modulation * amount; }

/**
 * Processes an oscillator with the given frequency and amplitude.
 * 
 * @param osc The oscillator to process.
 * @param freq The frequency to set for the oscillator.
 * @param amp The amplitude to set for the oscillator (default is 1.0).
 */
inline float ProcessOsc(Oscillator &osc, float freq, float amp = 1.0f)
{
    osc.SetFreq(freq);
    osc.SetAmp(amp);
    return osc.Process();
}

/**
 * Processes the reverb effect on the given signal and outputs the wet/dry mix to the output buffer.
 * 
 * @param reverb The reverb effect to process.
 * @param sig The input signal to process.
 * @param wetness The wet/dry mix amount (0.0 = dry, 1.0 = wet).
 * @param out The output buffer to write the processed signal to
 * @param i The index of the output buffer to write to.
 */
inline void ProcessReverb(ReverbSc                 &reverb,
                           float                     sig,
                           float                     wetness,
                           AudioHandle::OutputBuffer out,
                           size_t                    i)
{
    float wetL, wetR;
    reverb.Process(sig, sig, &wetL, &wetR);
    float dry = sig;

    out[0][i] = dry * (1.0f - wetness) + wetL * wetness;
    out[1][i] = dry * (1.0f - wetness) + wetR * wetness;
    return;
}
