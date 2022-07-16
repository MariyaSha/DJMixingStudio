/*
  ==============================================================================

    RecordingThumbnail.h
    Created: 8 Mar 2022 12:52:33am
    Author:  goaim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RecordingThumbnail  : public juce::Component,
                            private ChangeListener
{
public:
    RecordingThumbnail();
    ~RecordingThumbnail() override;

    AudioThumbnail& getAudioThumbnail();

    void setDisplayFullThumbnail(bool displayFull);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // colours
    juce::Colour c_mediumnavy = juce::Colour(44, 42, 75);
    juce::Colour c_teal = juce::Colour(43, 200, 197);
    juce::Colour c_white = juce::Colour(255, 255, 255);

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbnailCache{ 10 };
    AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };

    bool displayFullThumb = false;

    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        if (source == &thumbnail)
            repaint();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordingThumbnail)
};
