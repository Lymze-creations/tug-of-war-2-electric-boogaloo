#pragma once

#include "daisy_pod.h"
#include "daisysp.h"
#include "daisysp-lgpl.h"

using namespace daisy;
using namespace daisysp;

/**
 * Whether or not to log debug info.
 */
extern bool debug;
extern bool knobMode;

/**
 * The daisy pod object
 */
extern DaisyPod pod;

/**
 * The pressure values from the sensors
 */
extern float pressure[3];

/**
 * Secondary controls, mapped to knobs
 * Knob 1 -> secondary[0]
 * Knob 2 -> secondary[1]
 */
extern float secondary[2];

/**
 * Base frequency of the oscillators
 */
extern float baseFreq;

/**
 * Handles the audio callback
 */
void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size);

/**
 * Initializes the synth and all the components
 * 
 * @param sampleRate The sample rate of the daisy
 */
void InitSynth(float sampleRate);
