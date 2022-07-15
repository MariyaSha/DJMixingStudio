/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 2 Mar 2022 3:04:05pm
    Author:  goaim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse) :
    // use a 1000 points to plot the waveform
    audioThumb(1000, formatManagerToUse, cacheToUse),
    // no file is loaded to begin with
    fileLoaded(false),
    position(0)
{
    waveColour = Colour::fromString("#2bc8c5");
    
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    
    // outline around component
    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds(), 1);

    // set background colour
    g.setColour(Colour::fromString("#2c2a4b"));
    g.fillRect(getLocalBounds());

    if (fileLoaded){
        
        //draw playhead
        g.setColour(Colour::fromString("#1d1b36"));
        g.fillRect(position * getWidth(), 0, getWidth() / 20, getHeight());

        // draw waveform with the user selected colour
        g.setColour(waveColour);

        //draw waveform thumbnail
        audioThumb.drawChannel(
            g, 
            getLocalBounds(),
            0, //start
            audioThumb.getTotalLength(), //end
            0, //channel number (left channel)
            1.0f //vertical zoom factor
        );
    }
    else {
        // no file is detected yet
        g.setColour(Colour::fromString("#E5E5E5"));
        g.setFont(16.0f);
        g.drawText("file not yet loaded...", getLocalBounds(),
            juce::Justification::centred, true);
    }   
}

void WaveformDisplay::resized()
{
    // found in DeckGUI
}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded){
        repaint();
    }
    else {
        // nothing to update
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source) 
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    // repaint only is position changes
    if (pos != position)
    {
        position = pos; //store pos locally
        repaint();
    }
}


void WaveformDisplay::load_colour(juce::Colour clr)
{
    DBG("WaveformDisplay::load_colour");
    DBG(clr.toString());
    waveColour = clr;
}
