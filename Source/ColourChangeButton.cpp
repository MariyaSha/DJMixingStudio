/*
  ==============================================================================

    ColourChangeButton.cpp
    Created: 9 Mar 2022 9:22:20am
    Author:  goaim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ColourChangeButton.h"

//==============================================================================
ColourChangeButton::ColourChangeButton()
    : TextButton("SELECT COLOUR"),
    // if no colour exists in the colour file - use teal
    currentColour(c_teal)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(10, 24);
    changeWidthToFitText();
  
}

ColourChangeButton::~ColourChangeButton()
{
}

void ColourChangeButton::clicked()
{
    if (this->getButtonText() == "SELECT COLOUR")
    {
        auto colourSelector = std::make_unique<ColourSelector>(ColourSelector::showAlphaChannel
            | ColourSelector::showColourAtTop
            | ColourSelector::editableColour
            | ColourSelector::showSliders
            | ColourSelector::showColourspace);

        colourSelector->setCurrentColour(findColour(TextButton::buttonColourId));
        colourSelector->addChangeListener(this);
        colourSelector->setColour(ColourSelector::backgroundColourId, Colours::transparentBlack);
        colourSelector->setSize(300, 400);

        CallOutBox::launchAsynchronously(std::move(colourSelector), getScreenBounds(), nullptr);
    }

}

void ColourChangeButton::changeListenerCallback(ChangeBroadcaster* source)
{
    if (auto* cs = dynamic_cast<ColourSelector*> (source))
    {
        setColour(TextButton::buttonColourId, cs->getCurrentColour());
        currentColour = findColour(TextButton::buttonColourId);
    }
}
