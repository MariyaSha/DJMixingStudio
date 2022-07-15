/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

#pragma once


//==============================================================================
/* TAKEN AND MODIFIED FROM THE JUCE AUDIO RECORDING DEMO
   This component scrolls a continuous waveform showing the audio that's
   coming into whatever audio inputs this object is connected to.
*/
class LiveScrollingAudioDisplay : public AudioVisualiserComponent,
    public AudioIODeviceCallback
{
public:
    LiveScrollingAudioDisplay() : AudioVisualiserComponent(1)
    {
        setSamplesPerBlock(256);
        setBufferSize(1024);
    }

    /** before audio playback starts */
    void audioDeviceAboutToStart(AudioIODevice*) override
    {
        clear();
    }

    /** audio playback stops */
    void audioDeviceStopped() override
    {
        clear();
    }

    /** handling audio input/output as it plays */
    void audioDeviceIOCallback(const float** inputChannelData, int numInputChannels,
        float** outputChannelData, int numOutputChannels,
        int numberOfSamples) override
    {
        for (int i = 0; i < numberOfSamples; ++i)
        {
            float inputSample = 0;

            for (int chan = 0; chan < numInputChannels; ++chan)
                if (const float* inputChannel = inputChannelData[chan])
                    inputSample += inputChannel[i];  // find the sum of all the channels

            // boost levels to see the waveform more vividly
            inputSample *= 6.5f;

            pushSample(&inputSample, 1);
        }

        // clear the output buffers just in case
        for (int j = 0; j < numOutputChannels; ++j)
            if (float* outputChannel = outputChannelData[j])
                zeromem(outputChannel, (size_t)numberOfSamples * sizeof(float));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LiveScrollingAudioDisplay)
};
