/*
  ==============================================================================

    AudioRecorder.h
    Created: 8 Mar 2022 12:41:03am
    Author:  goaim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AudioRecorder  : public juce::Component,
                       public AudioIODeviceCallback
{
public:
    AudioRecorder(AudioThumbnail& thumbnailToUpdate);
    ~AudioRecorder() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** start recording microphone input*/
    void startRecording(const File& file);
    /** stop recording microphone input and save it to brand new file in Documents */
    void stop();
    /** checks if the file is currently being recorded */
    bool isRecording() const;

    //==============================================================================
    /** indicates that the device is about to start calling back*/
    void audioDeviceAboutToStart(AudioIODevice* device) override;
    /** indicate that the device has stopped */
    void audioDeviceStopped() override;
    /** Processes a block of incoming and outgoing audio data */
    void audioDeviceIOCallback(const float** inputChannelData, int numInputChannels,
        float** outputChannelData, int numOutputChannels,
        int numSamples) override;

private:
    /** stores audio recording thumbnail*/
    AudioThumbnail& thumbnail;
    /** thread to write audio data to disk */
    TimeSliceThread backgroundThread{ "Audio Recorder Thread" };
    /** pointer to buffer the incoming data */
    std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter;

    /** set default sample rate */
    double sampleRate = 0.0;
    /** set default next sample rate */
    int64 nextSampleNum = 0;
    /** re-entrant mutex object */
    CriticalSection writerLock;
    /** active buffer for the incoming data */
    std::atomic<AudioFormatWriter::ThreadedWriter*> activeWriter{ nullptr };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecorder)
};
