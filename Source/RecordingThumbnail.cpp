/*
  ==============================================================================

    RecordingThumbnail.cpp
    Created: 8 Mar 2022 12:52:33am
    Author:  goaim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RecordingThumbnail.h"

//==============================================================================
RecordingThumbnail::RecordingThumbnail()
{
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);
}

RecordingThumbnail::~RecordingThumbnail()
{
    thumbnail.removeChangeListener(this);
}

void RecordingThumbnail::paint (juce::Graphics& g)
{
    // set background and overlay colours
    g.fillAll(c_mediumnavy);
    g.setColour(c_teal);

    if (thumbnail.getTotalLength() > 0.0)
    {
        // display audio live feed
        auto endTime = displayFullThumb ? thumbnail.getTotalLength()
            : jmax(30.0, thumbnail.getTotalLength());

        auto thumbArea = getLocalBounds();
        thumbnail.drawChannels(g, thumbArea.reduced(2), 0.0, endTime, 2.00f);
    }
    else
    {
        // no recording detected
        g.setColour(c_white);
        g.setFont(14.0f);
        g.drawFittedText("(No file recorded)", getLocalBounds(), Justification::centred, 2);
    }
}

void RecordingThumbnail::resized()
{
    // found in recordingComponent
}

AudioThumbnail& RecordingThumbnail::getAudioThumbnail()
{ 
    return thumbnail; 
}

void RecordingThumbnail::setDisplayFullThumbnail(bool displayFull)
{
    displayFullThumb = displayFull;
    repaint();
}

