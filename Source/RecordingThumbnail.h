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
