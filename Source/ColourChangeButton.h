/*
  ==============================================================================

    ColourChangeButton.h
    Created: 9 Mar 2022 9:22:20am
    Author:  goaim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ColourChangeButton  : public TextButton,
                            public ChangeListener
{
public:
    ColourChangeButton();
    ~ColourChangeButton() override;

    /** triggers colour picker to pop */
    void clicked() override;

    using TextButton::clicked;
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /** sets waveform colour */
    juce::Colour currentColour;

private:

    juce::Colour c_teal = juce::Colour(43, 200, 197);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourChangeButton)
};
