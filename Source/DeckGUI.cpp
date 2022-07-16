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
    playButton.setColour(TextButton::buttonColourId, c_darknavy);
    playButton.setColour(TextButton::textColourOffId, c_white);
    playButton.setColour(ComboBox::outlineColourId, c_white);
    playButton.addListener(this);

    // SET COLOUR BUTTON STYLING
    addAndMakeVisible(colourChangeButton);
    colourChangeButton.setColour(TextButton::buttonColourId, c_darknavy);
    colourChangeButton.setColour(TextButton::textColourOffId, c_white);
    addAndMakeVisible(muteButton);
    colourChangeButton.addListener(this);

    // UPDATE COLOUR BUTTON STYLING
    muteButton.setColour(TextButton::buttonColourId, c_darknavy);
    muteButton.setColour(TextButton::textColourOffId, juce::Colours::white);
    muteButton.addListener(this);

    // VOLUME SLIDER STYLING
    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setSliderStyle(Slider::Rotary);
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    volSlider.setColour(Slider::rotarySliderFillColourId, c_sunflower);
    volSlider.setColour(Slider::rotarySliderOutlineColourId, c_mediumnavy);
    volSlider.setColour(Slider::thumbColourId, c_teal);
    volSlider.setRange(0.0, 1.0);
    volSlider.setPopupDisplayEnabled(false, true, this);

    // SPEEED SLIDER STYLING
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedSlider.setColour(Slider::rotarySliderFillColourId, c_sunflower);
    speedSlider.setColour(Slider::rotarySliderOutlineColourId, c_mediumnavy);
    speedSlider.setColour(Slider::thumbColourId, c_teal);
    speedSlider.setRange(0.0, 5.0);
    speedSlider.setPopupDisplayEnabled(false, true, this);

    // POSITION SLIDER STYLING
    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setSliderStyle(Slider::Rotary);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    posSlider.setColour(Slider::rotarySliderFillColourId, c_sunflower);
    posSlider.setColour(Slider::rotarySliderOutlineColourId, c_mediumnavy);
    posSlider.setColour(Slider::thumbColourId, c_teal);
    posSlider.setRange(0.0, 1.0);
    posSlider.setPopupDisplayEnabled(false, true, this);

    // WAVEFORM DISPLAY
    addAndMakeVisible(waveformDisplay);

    // LOAD BUTTON STYLING
    addAndMakeVisible(loadButton);
    loadButton.setColour(TextButton::buttonColourId, c_darknavy);
    loadButton.setColour(TextButton::textColourOffId, c_white);
    loadButton.addListener(this);

    // LOAD FROM PLAYLIST BUTTON STYLING
    addAndMakeVisible(playlistLoadButton);
    playlistLoadButton.setColour(TextButton::buttonColourId, c_darknavy);
    playlistLoadButton.setColour(TextButton::textColourOffId, c_white);
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
    g.setColour(c_black);
    g.fillRect(getLocalBounds());

    // draw an outline to separate both decks
    g.setColour(c_mediumnavy);
    g.drawRect(getLocalBounds(), 1);

    g.setFont(14.0f);
    g.setColour(c_white);

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
            btn->setColour(TextButton::buttonColourId, c_lightnavy);
        }
        else {
            // if button text = STOP
            player->stop();
            btn->setButtonText("PLAY");
            btn->setColour(TextButton::buttonColourId, c_darknavy);
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
            btn->setColour(TextButton::buttonColourId, c_darknavy);
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
            btn->setColour(TextButton::buttonColourId, c_darknavy);
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
