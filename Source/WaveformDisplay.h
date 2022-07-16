/*
  ==============================================================================

    WaveformDisplay.h
    Created: 2 Mar 2022 3:04:05pm
    Author:  goaim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ColourChangeButton.h"

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** repaint component if change is detected */
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /** load URL of audio file */
    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

    /** stores waveform colour */
    juce::Colour waveColour;

    /** loads waveform colour in DeckGUI component*/
    void load_colour(juce::Colour clr);

private:

    juce::Colour c_teal = juce::Colour(43, 200, 197);
    juce::Colour c_mediumnavy = juce::Colour(44, 42, 75);
    juce::Colour c_darkernavy = juce::Colour(29, 27, 54);
    juce::Colour c_cream = juce::Colour(229, 229, 229);
    
    /** fetch user selected colour */
    //juce::Colour fetchSelection();

    AudioThumbnail audioThumb;
    // tracks wether a file is currently loaded on a deck
    bool fileLoaded;
    // tracks position of playhead
    double position;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
