/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 5 Mar 2022 10:08:51am
    Author:  goaim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player, 
                   AudioFormatManager& formatManagerToUse,
                   AudioThumbnailCache& cacheToUse)
    : player(_player),
      searchRow{0}
{

    trackFile.reset(new juce::File(File::getCurrentWorkingDirectory().getChildFile("../../Source/trackLibrary.txt").getFullPathName()));
    
    tableComponent.getHeader().addColumn("Track title", 1, 200);
    tableComponent.getHeader().addColumn("Size", 2, 200);
    tableComponent.getHeader().addColumn("Length", 3, 300);
    tableComponent.getHeader().addColumn("", 4, 100);
    tableComponent.getHeader().addColumn("", 5, 100);
    tableComponent.getHeader().addColumn("", 6, 100);

    tableComponent.setModel(this);

    tableComponent.setColour(ListBox::backgroundColourId, Colour::fromString("#2bc8c5"));

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchbox);
    searchbox.addListener(this);
    searchbox.setColour(TextEditor::backgroundColourId, Colour::fromString("#2c2a4b"));
    searchbox.setColour(TextEditor::textColourId, juce::Colours::lightgrey);
    searchbox.setTextToShowWhenEmpty(CharPointer_UTF8("search in playlist"), juce::Colours::lightgrey);

   

    startTimer(500);
}

PlaylistComponent::~PlaylistComponent()
{
    stopTimer();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight()/3.5);
    searchbox.setBounds(0, tableComponent.getHeight(), getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
    return musicL.trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected)
{
    if (rowIsSelected)
    {
        // track selected by user
        g.fillAll(Colour::fromString("#febc27"));
        musicL.updateSelectionFile(musicL.trackPath[rowNumber]);
    }
    else if (rowNumber == searchRow && keyword.length() == 0)
    {
        // search keyword cleared
        g.fillAll(Colour::fromString("#000000"));
        g.setColour(juce::Colours::white);
    }
    else if (rowNumber == searchRow && keyword.length() > 0)
    {
        // at least one search keyword char was entered by user
        // and at least one track is a match
        g.fillAll(Colour::fromString("#4E37B0"));
    }
    else {
        // no search keyword chars detected
        g.fillAll(Colour::fromString("#000000"));
        g.setColour(juce::Colours::white);
    }

    // draw playlist information fetched from MusicL

    g.drawText(
        musicL.trackTitles[rowNumber],
        5,
        0,
        width - 4,
        height,
        Justification::centredLeft,
        true);

    g.drawText(
        musicL.trackSizes[rowNumber],
        205,
        0,
        width - 4,
        height,
        Justification::centredLeft,
        true);

    g.drawText(
        std::to_string(musicL.trackLength[rowNumber]),
        405,
        0,
        width - 4,
        height,
        Justification::centredLeft,
        true);

    // evaluate wether trackLibrary file was updated
    trackFile.reset(new juce::File(File::getCurrentWorkingDirectory().getChildFile("../../Source/trackLibrary.txt").getFullPathName()));
    juce::StringArray my_array;
    trackFile->readLines(my_array);
    std::vector<std::string> tokens;

    tokens = musicL.tokenize(my_array[0].toStdString(), ',');

    if (musicL.trackTitles[0] == tokens[0])
    {
        // no recording loaded
        // nothing to update
    }
    else {
        // new recording was detected in trackLibrary.txt
        // update track vectors
        musicL.loadTracks();
        musicL.trackTitles[0] = tokens[0];
        musicL.trackSizes[0] = tokens[2];
        musicL.trackLength[0] = std::stod(tokens[3]);
        musicL.trackPath[0] = URL{tokens[1]};
        tableComponent.repaintRow(0);
        tableComponent.repaint();
    }
}

void PlaylistComponent::paintCell(Graphics & g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected)
{

}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                              int columnId,
                              bool isRoeSelected,
                              Component* existingComponentToUpdate)
{
    if (columnId == 4)
    {
        //this is where we have access to the button
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "play" };

            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            btn->setColour(TextButton::buttonColourId, Colour::fromString("#52567d"));
            btn->setColour(ComboBox::outlineColourId, Colour::fromString("#000000"));
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 5)
    {
        //this is where we have access to the button
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn2 = new TextButton{ "load" };

            String id{ "a" + std::to_string(rowNumber)};
            btn2->setComponentID(id);
            btn2->addListener(this);
            btn2->setColour(TextButton::buttonColourId, Colour::fromString("#52567d"));
            btn2->setColour(ComboBox::outlineColourId, Colour::fromString("#000000"));
            existingComponentToUpdate = btn2;
        }
    }
    if (columnId == 6)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn4 = new TextButton{ "clear" };

            String id{ std::to_string(rowNumber) };
            btn4->setComponentID(id);
            btn4->addListener(this);
            btn4->setColour(TextButton::buttonColourId, Colour::fromString("#52567d"));
            btn4->setColour(ComboBox::outlineColourId, Colour::fromString("#000000"));
            existingComponentToUpdate = btn4;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* btn)
{

    if (btn->getButtonText() == "load")
    {
        FileChooser chooser{ "select a file..." };
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{ chooser.getResult() });
            player->setGain(0.5);

            File trackFileToLoad;
            trackFileToLoad = chooser.getResult();

            std::string my_id;
            my_id = btn->getComponentID()[1];

            juce::String file_name{ trackFileToLoad.getFileName() };

            URL file_path{ chooser.getResult() };

            juce::String file_size{ trackFileToLoad.getSize()};

            // update playlist
            musicL.updateMusicLibrary(file_name.toStdString(),
                file_size.toStdString(),
                player->lenInSeconds,
                file_path.toString(false),
                std::stoi(my_id)
                );      

            // update playlist file
            musicL.updateMusicFile();

            repaint();
        }
    }
    else if (btn->getButtonText() == "stop")
    {
        btn->setColour(TextButton::buttonColourId, Colour::fromString("#52567d"));
        btn->setButtonText("play");
        player->stop();
    }
    else if (btn->getButtonText() == "play") {
        int id = std::stoi(btn->getComponentID().toStdString());
        btn->setColour(TextButton::buttonColourId, Colour::fromString("#febc27"));
        btn->setButtonText("stop");
        player->stop();
        player->loadURL(musicL.trackPath[id]);
        player->setGain(0.5);
        player->start();
    }
    else if (btn->getButtonText() == "clear")
    {
        int id = std::stoi(btn->getComponentID().toStdString());
        musicL.updateMusicLibrary(
            "Track" + std::to_string(id+1),
            "0.0",
            0.0,
            juce::URL (""),
            id
        );

        musicL.updateMusicFile();
        repaint();
    }
}


void PlaylistComponent::textEditorTextChanged(TextEditor&)
{
    keyword = searchbox.getText().toStdString();

    for (int i = 0; i < musicL.trackTitles.size() ; ++i)
    {
        if (musicL.trackTitles[i].substr(0, keyword.length()) == keyword)
        {
            searchRow = i;
            repaint();
            break;
        }
    } 
}

void PlaylistComponent::timerCallback()
{

}
