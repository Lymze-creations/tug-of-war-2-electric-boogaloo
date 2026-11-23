#include "controls.hpp"
#include "synth.hpp"
#include "util.hpp"

/**
 * Handles the controls of the daisy pod
 */
void HandleControls()
{
    if(pod.button1.RisingEdge())
        knobMode = !knobMode;

    if(knobMode)
    {
        pressure[0] = pod.knob1.Process();
        pressure[1] = pod.knob2.Process();

        float diff = pod.encoder.Increment() * 0.1f;
        pressure[2] += diff;
        pressure[2] = clamp(pressure[2], 0.0f, 1.0f);
    }
    else
    {
        // Control secondary
        secondary[0] = pod.knob1.Process();
        secondary[1] = pod.knob2.Process();

        // Encoder controls freq
        float diff = pod.encoder.Increment() * 5.0f;
        baseFreq += diff;
        baseFreq = clamp(baseFreq, 100.0f, 1000.0f);
    }

    // Update led
    Color color;
    if(knobMode)
        color.Init(Color::RED);
    else
        color.Init(Color::GREEN);
    pod.led1.SetColor(color);
    pod.UpdateLeds();
}
