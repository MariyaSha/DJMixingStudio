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
        
    // colours
    juce::Colour c_black = juce::Colour(0, 0, 0);
    juce::Colour c_white = juce::Colour(255, 255, 255);
    juce::Colour c_teal = juce::Colour(43, 200, 197);
    juce::Colour c_sunflower = juce::Colour(254, 188, 39);
    juce::Colour c_darknavy = juce::Colour(26, 17, 35);
    juce::Colour c_mediumnavy = juce::Colour(44, 42, 75);
    juce::Colour c_lightnavy = juce::Colour(82, 86, 125);


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
