/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 1 Mar 2022 2:31:09pm
    Author:  goaim

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}
// AUDIO LIFECYCLE
//==============================================================================

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
{
    // birth of audio
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) 
{
    // life of audio
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources() 
{
    // death of audio
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

//==============================================================================

void DJAudioPlayer::loadURL(URL audioURL) 
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) 
    {
        //create audio format reader source
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));

        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        lenInSeconds = transportSource.getLengthInSeconds();
        readerSource.reset(newSource.release());
    }
    else
    {
        // something went wrong with the reader
        DBG("DJAudioPlayer::loadURL -> oops something went wrong in the file");
    }
}
void DJAudioPlayer::setGain(double gain) 
{
    if (gain < 0 || gain > 1.0)
    {
        // bad gain value was provided
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else {
        // update gain
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio) 
{
    if (ratio < 0 || ratio > 5.0)
    {
        // bad speed value was provided
        DBG("DJAudioPlayer::setSpeed ratio should be between 1 and 5");
    }
    else {
        // update speed
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs) 
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        // bad position value was provided
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    }
    else {
        // update position
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start() 
{
    transportSource.start();
}
void DJAudioPlayer::stop() 
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
