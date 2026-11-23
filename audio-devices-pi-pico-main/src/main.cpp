#include <Arduino.h>
#include "util.hpp"

#define SerialMidi Serial2

/**
 * Setup midi connection
 */
void setup()
{
    SerialMidi.setTX(4);
    SerialMidi.setRX(5);
    SerialMidi.begin(31250);
}

/**
 * The smoothing factor for the pressure values.
 */
const float SMOOTHING_FACTOR = 0.5f;

float pins[3] = {A0, A1, A2};
int midi_channels[3] = {20, 21, 22};

float filtered[3] = {0.0f, 0.0f, 0.0f};
float pressure[3] = {0.0f, 0.0f, 0.0f};

// Hard code ranges for now
float min_bounds[3] = {130.0f, 40.0f, 50.0f};
float max_bounds[3] = {800.0f, 750.0f, 400.0f};

/**
 * Reads all pressure values and smooths them also
 */
void read_pressures()
{
    for (int i = 0; i < 3; i++)
    {
        pressure[i] = analogRead(pins[i]);
        filtered[i] += SMOOTHING_FACTOR * (pressure[i] - filtered[i]);
    }
}

/**
 * Sends all pressure values over the midi channels
 */
void send_pressures_over_midi()
{
    for (int i = 0; i < 3; i++)
    {
        // Map to midi
        uint8_t midiPressure = map_safe(
            (int)filtered[i],
            (int)min_bounds[i],
            (int)max_bounds[i],
            0,
            127);

        sendCC(midi_channels[i], midiPressure);

        Serial.print("raw[" + String(i) + "]: " + String(pressure[i]) + ", ");
        Serial.print("filtered[" + String(i) + "]: " + String(filtered[i]) + ", ");
        Serial.println("midi[" + String(i) + "]: " + String(midiPressure));
    }
    Serial.println("--------------------------------------------------");
}

/**
 * Main loop
 */
void loop()
{
    read_pressures();
    send_pressures_over_midi();
    delay(10);
}

// tip goes to TX, ring goes to power 3.3v