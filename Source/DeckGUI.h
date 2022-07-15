/*
  ==============================================================================

    DeckGUI.h
    Created: 1 Mar 2022 3:49:06pm
    Author:  goaim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "MusicLibrary.h"
#include "ColourChangeButton.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager & formatManagerToUse,
            AudioThumbnailCache & cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    /** enables file dragging from one component to another */
    bool isInterestedInFileDrag(const StringArray &files) override;

    /** enables file dropping from one component to another*/
    void filesDropped(const StringArray &files, int x, int y) override;

    /** responsible for the timer thread */
    void timerCallback() override;

    /** stores waveform colour */
    juce::Colour waveColour;

    /** instance of music library */
    MusicLibrary musicL;

    juce::Colour my_colour;
        
private:
    // a button that triggers a colourPicker
    ColourChangeButton colourChangeButton;

    // GUI button elements
    TextButton playButton{ "PLAY" };
    TextButton loadButton{ "LOAD TRACK" };
    TextButton muteButton{ "MUTE" };
    TextButton playlistLoadButton{ "LOAD FROM PLAYLIST" };



    double is_mute;

    // GUI Slider elements
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    // pointer to DJAudioPlayer object
    DJAudioPlayer* player;

    // waveform display object
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
