/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
   
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(1000, 700);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
        && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted)  setAudioChannels(2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    // make all components visible
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(audioRecordingDemo);
    addAndMakeVisible(playlistComponent);

    // register audio formats
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // preload tracks
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player3.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // set mizer sources
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.addInputSource(&player3, false);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    player1.releaseResources();
    player2.releaseResources();
    player3.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    double rowH = getHeight() / 9;

    // top component
    audioRecordingDemo.setBounds(0, 0, getWidth(), rowH * 2);

    // centre components
    deckGUI1.setBounds(0, rowH * 2, getWidth()/2, rowH * 6);
    deckGUI2.setBounds(getWidth()/2, rowH * 2, getWidth()/2, rowH * 6);

    // bottom component
    playlistComponent.setBounds(0, rowH * 6, getWidth(), getHeight());
}

void MainComponent::buttonClicked(Button* btn)
{

}

void MainComponent::sliderValueChanged(Slider *sldr)
{
  
}

