/*
  ==============================================================================

    PlaylistComponent.h
    Created: 5 Mar 2022 10:08:51am
    Author:  goaim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "MusicLibrary.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
    public TableListBoxModel,
    public Button::Listener,
    public TextEditor::Listener,
    public Timer
{
public:
    PlaylistComponent(DJAudioPlayer* player,
                      AudioFormatManager & formatManagerToUse,
                      AudioThumbnailCache & cacheToUse);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** TableListBoxModel total number of rows*/
    int getNumRows() override;

    /** TableListBoxModel rows styling and content */
    void paintRowBackground(Graphics &, 
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected) override;

    /** TableListBoxModel cells styling and content */
    void paintCell(Graphics &,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    /** TableListBoxModel update cell components */
    Component* refreshComponentForCell(int rowNumber, 
                                       int columnId,
                                       bool isRoeSelected,
                                       Component *existingComponentToUpdate) override;

    /** implement button clicks */
    void buttonClicked(Button* btn) override;

    /** implement listener to changes in the searchbox */
    void textEditorTextChanged(TextEditor&) override;

    /** implement timer callback */
    void timerCallback() override;

    int searchRow;

    /** stores user search keyword*/
    std::string keyword;

    /** stores pointer to trackLibrary.txt*/
    std::unique_ptr<juce::File> trackFile;

private:
    // music library object
    MusicLibrary musicL;

    // playlist components
    TextEditor searchbox;
    TableListBox tableComponent;

    // player object
    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
