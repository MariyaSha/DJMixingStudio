/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "RecordingComponent.h"
#include "PlaylistComponent.h"
#include "MusicLibrary.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent,
                      public Button::Listener,
                      public Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    // AUDIO FILE LIFECYCLE
    //==============================================================================
    /** pre-load audio file */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    /** handles audio file loaded in buffer */
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    /** handles end of audio lifecycle */
    void releaseResources() override;
    //==============================================================================

    void paint(Graphics& g) override;
    void resized() override;

    /** implement button::Listener */
    void buttonClicked(Button*) override; 

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

private:

    AudioFormatManager formatManager;

    // cache up to 100 file plots
    AudioThumbnailCache thumbnailCache{100};

    // players for decks
    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };
    // player for playlist tracks
    DJAudioPlayer player3{ formatManager };

    // GUI Deck objects
    DeckGUI deckGUI1{ &player1, formatManager, thumbnailCache};
    DeckGUI deckGUI2{ &player2, formatManager, thumbnailCache};

    MixerAudioSource mixerSource;

    // playlist and recording components
    PlaylistComponent playlistComponent{ &player3, formatManager, thumbnailCache };

    RecordingComponent audioRecordingDemo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
