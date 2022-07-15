/*
  ==============================================================================

    MusicLibrary.h
    Created: 5 Mar 2022 1:12:57pm
    Author:  goaim

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


//==============================================================================
/*
*/
class MusicLibrary  : public juce::Component

{
    public:
        MusicLibrary();
        ~MusicLibrary() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        /** tokenizes row values for trackLibrary.txt */
        std::vector<std::string> tokenize(std::string csvRow, char sep);

        /** load current playlist tracks from trackLibrary.txt into the playlistComponent*/
        void loadTracks();

        /** update track vectors to be displayed in playlistComponent */
        void updateMusicLibrary(
            std::string title,
            std::string size,
            double length,
            URL path,
            int idx
        );

        /** update trackLibrary.txt with current playlist tracks */
        void updateMusicFile();

        /** update selectedTrack.txt with the current track selected in the playlist*/
        void updateSelectionFile(URL selectionURL);

        /** fetch the current track selection from selectedTrack.txt*/
        URL fetchSelection();

        // track vectors displayed in playlistComponent
        std::vector<std::string> trackTitles;
        std::vector<std::string> trackSizes;
        std::vector<double> trackLength;
        std::vector<URL> trackPath;

        // pointers to selection file and trackLibrary
        std::unique_ptr<juce::File> my_file;
        std::unique_ptr<juce::File> selectionFile;

        // stores current selection
        URL currentSelection;
        void updateCurrentSelection(URL audioFile);

    private:
        int playlistSize{7};

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicLibrary)
};
