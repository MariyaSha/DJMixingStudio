/*
  ==============================================================================

    AudioRecorder.cpp
    Created: 8 Mar 2022 12:41:03am
    Author:  goaim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioRecorder.h"

//==============================================================================
AudioRecorder::AudioRecorder(AudioThumbnail& thumbnailToUpdate)
    : thumbnail(thumbnailToUpdate)
{
    // start collecting microphone input
    backgroundThread.startThread();

}

AudioRecorder::~AudioRecorder()
{
    stop();
}

void AudioRecorder::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    // no visuals
}

void AudioRecorder::resized()
{
    // no visuals
}

void AudioRecorder::startRecording(const File& file)
{
    stop();

    if (sampleRate > 0)
    {
        // Create an OutputStream for the brand new file of the recording
        file.deleteFile();

        if (auto fileStream = std::unique_ptr<FileOutputStream>(file.createOutputStream()))
        {
            // create a WAV writer object
            WavAudioFormat wavFormat;

            if (auto writer = wavFormat.createWriterFor(fileStream.get(), sampleRate, 1, 16, {}, 0))
            {
                fileStream.release(); 

                // write the data to disk
                threadedWriter.reset(new AudioFormatWriter::ThreadedWriter(writer, backgroundThread, 32768));

                // reset recording thumbnail
                thumbnail.reset(writer->getNumChannels(), writer->getSampleRate());
                nextSampleNum = 0;

                // swap the active writer pointer with audio callback
                const ScopedLock sl(writerLock);
                activeWriter = threadedWriter.get();
            }
        }
    }
}

void AudioRecorder::stop()
{
    {
        const ScopedLock sl(writerLock);
        activeWriter = nullptr;
    }

    // delete the writer object.
    threadedWriter.reset();
}

bool AudioRecorder::isRecording() const
{
    return activeWriter.load() != nullptr;
}

void AudioRecorder::audioDeviceAboutToStart(AudioIODevice* device)
{
    sampleRate = device->getCurrentSampleRate();
}

void AudioRecorder::audioDeviceStopped()
{
    sampleRate = 0;
}

void AudioRecorder::audioDeviceIOCallback(const float** inputChannelData, int numInputChannels,
    float** outputChannelData, int numOutputChannels,
    int numSamples)
{
    const ScopedLock sl(writerLock);

    if (activeWriter.load() != nullptr && numInputChannels >= thumbnail.getNumChannels())
    {
        activeWriter.load()->write(inputChannelData, numSamples);

        // Create an AudioBuffer to wrap the incoming data
        AudioBuffer<float> buffer(const_cast<float**> (inputChannelData), thumbnail.getNumChannels(), numSamples);
        thumbnail.addBlock(nextSampleNum, buffer, 0, numSamples);
        nextSampleNum += numSamples;
    }

    // clear the output buffers
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != nullptr)
            FloatVectorOperations::clear(outputChannelData[i], numSamples);
}