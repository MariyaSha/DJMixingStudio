/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 1 Mar 2022 2:31:09pm
    Author:  goaim

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
    public:

        DJAudioPlayer(AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        // AUDIO FILE LIFECYCLE
        //==============================================================================

        /** pre-load audio file */
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override; 
        /** handles audio file loaded in buffer */
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
        /** handles end of audio lifecycle */
        void releaseResources() override;

        //==============================================================================
   
        /** load URL of audio file and create a format reader source */
        void loadURL(URL audioURL);

        /** update audio file volume */
        void setGain(double gain);
        /** update audio file speed */
        void setSpeed(double ratio);
        /** update audio file position */
        void setPosition(double posInSecs);
        /** update audio file position relative to the length of the file (0 to 1) */
        void setPositionRelative(double pos);

        /** start playing audio */
        void start();
        /** stop playing audio */
        void stop();

        /** get the relative position of the playhead (between 0-1)*/
        double getPositionRelative();
        /** stores audio file length in seconds*/
        double lenInSeconds;

    private:
        // audio format manager object
        AudioFormatManager& formatManager;
        // pointer to reader source
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        // adutio transport and resampling source objects
        AudioTransportSource transportSource;
        ResamplingAudioSource resampleSource{&transportSource, false, 2};
};