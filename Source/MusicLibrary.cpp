/*
  ==============================================================================

    MusicLibrary.cpp
    Created: 5 Mar 2022 1:12:57pm
    Author:  goaim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MusicLibrary.h"
#include <fstream>
#include <vector>
#include <string>

//==============================================================================
MusicLibrary::MusicLibrary()
{
    selectionFile.reset((new juce::File(File::getCurrentWorkingDirectory().getChildFile("../../Source/selectedTrack.txt").getFullPathName())));
    
    // load tracks from file into public track vectors
    loadTracks();
}

MusicLibrary::~MusicLibrary()
{
}

void MusicLibrary::paint (juce::Graphics& g)
{
    // non-visual component
}

void MusicLibrary::resized()
{
    //non-visual component
}

void MusicLibrary::loadTracks()
{
    // load trackLibrary.txt
    my_file.reset(new juce::File(File::getCurrentWorkingDirectory().getChildFile("../../Source/trackLibrary.txt").getFullPathName()));
    juce::StringArray my_array;
    my_file->readLines(my_array);

    // vectorize file content and store in track vectors
    std::vector<std::string> tokens;
    for (int i = 0; i < my_array.size() - 1; ++i)
    {
        tokens = tokenize(my_array[i].toStdString(), ',');
        trackTitles.push_back(tokens[0]);
        trackSizes.push_back(tokens[2]);
        trackPath.push_back(URL{ tokens[1] });
        trackLength.push_back(std::stod(tokens[3]));
    }
}

std::vector<std::string> MusicLibrary::tokenize(std::string csvRow, char sep)
{
    // find out how many separators are found in the string
    unsigned int count = std::count(csvRow.begin(), csvRow.end(), sep);

    // stores the tokenized string
    std::vector<std::string> row_tokens;

    // create a string instance of the separator char
    std::string sep_str{ sep };

    for (unsigned int i = 0; i <= count; ++i)
    {
        size_t target = csvRow.find(sep_str);
        std::string sliced;
        sliced = csvRow.substr(0, target);
        row_tokens.push_back(sliced);

        // remove the sliced part
        csvRow = csvRow.substr(target + 1, -1);
    }

    return row_tokens;
}

void MusicLibrary::updateMusicLibrary(
        std::string title,
        std::string size,
        double length,
        URL path,
        int idx)
{
    trackTitles.at(idx) = title;
    trackPath.at(idx) = path;
    trackSizes.at(idx) = size;
    trackLength.at(idx) = length;
}

void MusicLibrary::updateMusicFile()
{
    my_file->replaceWithText(""); //remove entire content
    for (int i = 0; i < playlistSize ; ++i)
    {
        my_file->appendText(trackTitles[i] + "," + (trackPath[i].toString(true)).toStdString() + "," + trackSizes[i] + "," + std::to_string(trackLength[i]) + "\n");
    }
}

void MusicLibrary::updateCurrentSelection(URL audioFile)
{
    currentSelection = audioFile;
}

void MusicLibrary::updateSelectionFile(URL selectionURL)
{
    selectionFile->replaceWithText("");
    selectionFile->appendText(selectionURL.toString(true));
}

URL MusicLibrary::fetchSelection()
{
    juce::StringArray fileContent;
    selectionFile->readLines(fileContent);
    return fileContent[0];
}