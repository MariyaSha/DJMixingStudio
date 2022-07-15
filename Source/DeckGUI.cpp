/*
  ==============================================================================

    DeckGUI.cpp
    Created: 1 Mar 2022 3:49:06pm
    Author:  goaim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "MainComponent.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse
             ) : player(_player),
                 my_colour(juce::Colours::orange),
                 waveformDisplay(formatManagerToUse, cacheToUse)
{
    is_mute = false;
    // PLAY BUTTON STYLING
    addAndMakeVisible(playButton);
    playButton.setColour(TextButton::buttonColourId, Colour::fromString("#1a1123"));
    playButton.setColour(TextButton::textColourOffId, juce::Colours::white);
    playButton.setColour(ComboBox::outlineColourId, juce::Colours::white);
    playButton.addListener(this);

    // SET COLOUR BUTTON STYLING
    addAndMakeVisible(colourChangeButton);
    colourChangeButton.setColour(TextButton::buttonColourId, Colour::fromString("#1a1123"));
    colourChangeButton.setColour(TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(muteButton);
    colourChangeButton.addListener(this);

    // UPDATE COLOUR BUTTON STYLING
    muteButton.setColour(TextButton::buttonColourId, Colour::fromString("#1a1123"));
    muteButton.setColour(TextButton::textColourOffId, juce::Colours::white);
    muteButton.addListener(this);

    // VOLUME SLIDER STYLING
    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setSliderStyle(Slider::Rotary);
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    volSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromString("#febc27"));
    volSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromString("#2c2a4b"));
    volSlider.setColour(Slider::thumbColourId, Colour::fromString("#2bc8c5"));
    volSlider.setRange(0.0, 1.0);
    volSlider.setPopupDisplayEnabled(false, true, this);

    // SPEEED SLIDER STYLING
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromString("#febc27"));
    speedSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromString("#2c2a4b"));
    speedSlider.setColour(Slider::thumbColourId, Colour::fromString("#2bc8c5"));
    speedSlider.setRange(0.0, 5.0);
    speedSlider.setPopupDisplayEnabled(false, true, this);

    // POSITION SLIDER STYLING
    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setSliderStyle(Slider::Rotary);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    posSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromString("#febc27"));
    posSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromString("#2c2a4b"));
    posSlider.setColour(Slider::thumbColourId, Colour::fromString("#2bc8c5"));
    posSlider.setRange(0.0, 1.0);
    posSlider.setPopupDisplayEnabled(false, true, this);

    // WAVEFORM DISPLAY
    addAndMakeVisible(waveformDisplay);

    // LOAD BUTTON STYLING
    addAndMakeVisible(loadButton);
    loadButton.setColour(TextButton::buttonColourId, Colour::fromString("#1a1123"));
    loadButton.setColour(TextButton::textColourOffId, juce::Colours::white);
    loadButton.addListener(this);

    // LOAD FROM PLAYLIST BUTTON STYLING
    addAndMakeVisible(playlistLoadButton);
    playlistLoadButton.setColour(TextButton::buttonColourId, Colour::fromString("#1a1123"));
    playlistLoadButton.setColour(TextButton::textColourOffId, juce::Colours::white);
    playlistLoadButton.addListener(this);

    //initiallize timer thread and call it every 500 milliseconds
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    //stop timer when DeckGUI is destroyed
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    // slider area background
    g.setColour(Colour::fromString("#000000"));
    g.fillRect(getLocalBounds());

    // draw an outline to separate both decks
    g.setColour(Colour::fromString("#2c2a4b"));
    g.drawRect(getLocalBounds(), 1);

    g.setFont(14.0f);
    g.setColour(juce::Colours::white);

    // slider labels
    g.drawText ("vol.", getLocalBounds().removeFromLeft(165).removeFromTop(220),
         juce::Justification::centred, true); 

    g.drawText("speed", getLocalBounds().removeFromTop(220),
        juce::Justification::centred, true);

    g.drawText("pos.", getLocalBounds().removeFromRight(165).removeFromTop(220),
        juce::Justification::centred, true);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 9;
    double rowW = getWidth() / 3;

    // first row of buttons
    playButton.setBounds(0, 0, getWidth()/3, rowH / 1.75);
    colourChangeButton.setBounds(getWidth() / 3, 0, getWidth() / 3, rowH / 1.75);
    muteButton.setBounds(getWidth() / 1.5, 0, getWidth() / 3, rowH / 1.75);

    // second row of sliders
    volSlider.setBounds(0, rowH, rowW, rowH*2.25);
    speedSlider.setBounds(rowW, rowH , rowW, rowH * 2.25);
    posSlider.setBounds(rowW*2, rowH , rowW, rowH * 2.25);

    // third row of waveform display
    waveformDisplay.setBounds(0, rowH * 3.5, getWidth(), rowH * 2);

    // last row of buttons
    loadButton.setBounds(0, rowH * 5+18, getWidth() / 2, rowH/1.5);
    playlistLoadButton.setBounds(getWidth() / 2, rowH * 5+18, getWidth()/2, rowH/1.5);
}

void DeckGUI::buttonClicked(Button* btn)
{
    if (btn == &playButton)
    {
        // button switch from play to stop and vice versa
        if (btn->getButtonText() == "PLAY")
        {
            player->start();
            btn->setButtonText("STOP");
            btn->setColour(TextButton::buttonColourId, Colour::fromString("#52567d"));
        }
        else {
            // if button text = STOP
            player->stop();
            btn->setButtonText("PLAY");
            btn->setColour(TextButton::buttonColourId, Colour::fromString("#1a1123"));
        }
    }
    if (btn == &playlistLoadButton)
    {
        URL my_URL;
        my_URL = musicL.fetchSelection();
        // load current track selection into deck & display waveform
        player->loadURL(my_URL);
        waveformDisplay.loadURL(my_URL);
    }
    if (btn == &loadButton)
    {
        FileChooser chooser{"select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{ chooser.getResult() });
            waveformDisplay.loadURL(URL{ chooser.getResult() });
        }
    }
    if (btn == &colourChangeButton)
    {       
        if (btn->getButtonText() == "SELECT COLOUR")
        {
            btn->setButtonText("UPDATE");
        }
        else
        {
            btn->setButtonText("SELECT COLOUR");
            btn->setColour(TextButton::buttonColourId, Colour::fromString("#1a1123"));
        }

        // update colour
        waveColour = colourChangeButton.currentColour;
        waveformDisplay.load_colour(waveColour);

        repaint();
    }
    if (btn == &muteButton)
    {
        if (is_mute == false)
        {
            player->setGain(0);
            btn->setColour(TextButton::buttonColourId, juce::Colours::hotpink);
            is_mute = true;
        }
        else {
            player->setGain(0.5);
            btn->setColour(TextButton::buttonColourId, Colour::fromString("#1a1123"));
            is_mute = false;
        }
    }

}

void DeckGUI::sliderValueChanged(Slider* sldr)
{
     if (sldr == &volSlider)
     {
         player->setGain(sldr->getValue());
     }
     if (sldr == &speedSlider)
     {
         player->setSpeed(sldr->getValue());
     }
     if (sldr == &posSlider)
     {
         player->setPositionRelative(sldr->getValue());
     }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    // drop file on component
    if (files.size() == 1)
    {
        player->loadURL(URL{File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
    player->getPositionRelative());
}
