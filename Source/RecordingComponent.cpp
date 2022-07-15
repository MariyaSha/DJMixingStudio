/*
  ==============================================================================

    RecordingComponent.cpp
    Created: 8 Mar 2022 12:02:23pm
    Author:  goaim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RecordingComponent.h"
#include "DemoUtilities.h"
#include "AudioLiveScrollingDisplay.h"
#include "MusicLibrary.h"

//==============================================================================
RecordingComponent::RecordingComponent()
{
    setOpaque(true);
    addAndMakeVisible(liveAudioScroller);
    addAndMakeVisible(recordingThumbnail);

    // RECORD BUTTON STYLING
    addAndMakeVisible(recordButton);
    recordButton.setColour(TextButton::buttonColourId, Colour(0xffff5c5c));
    recordButton.setColour(TextButton::textColourOnId, Colours::black);
    recordButton.setColour(ComboBox::outlineColourId, juce::Colours::white);

    // RECORD BUTTON FUNCTIONALITY
    recordButton.onClick = [this]
    {
        if (recorder.isRecording())
            stopRecording();
        else
            startRecording();
    };

    // RECORDING NAME TEXTBOX STYLING
    addAndMakeVisible(recordingName);
    recordingName.setColour(TextEditor::backgroundColourId, Colour::fromString("#52567d"));
    recordingName.setColour(TextEditor::textColourId, Colour::fromString("#ffffff"));
    recordingName.setTextToShowWhenEmpty(CharPointer_UTF8("recording name"), Colour::fromString("#ffffff"));
    recordingName.addListener(this);

    // LOAD TO PLAYLIST BUTTON STYLING
    addAndMakeVisible(loadToPlaylistButton);
    loadToPlaylistButton.setColour(TextButton::buttonColourId, Colour::fromString("#2c2a4b"));
    loadToPlaylistButton.setColour(TextButton::buttonColourId, Colour::fromString("#52567d"));

    // LOAD TO PLAYLIST BUTTON FUNCTIONALITY
    loadToPlaylistButton.onClick = [this]
    {

       // load audio file
       File recording_file = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(fileName + ".wav");
       
       juce::String file_name{ recording_file.getFileName() };
       URL file_path{ recording_file };

       //juce::String file_size{ recording_file.getSize() };

       MusicLibrary musicL;


       musicL.updateMusicLibrary(
           file_name.toStdString(),
           std::to_string(recording_file.getSize()),
           recording_file.getSize() / 96030,
           file_path,
           0
       );

       // update playlist file
       musicL.updateMusicFile();

    };


#ifndef JUCE_DEMO_RUNNER
    RuntimePermissions::request(RuntimePermissions::recordAudio,
        [this](bool granted)
        {
            int numInputChannels = granted ? 2 : 0;
            audioDeviceManager.initialise(numInputChannels, 2, nullptr, true, {}, nullptr);
        });
#endif

    audioDeviceManager.addAudioCallback(&liveAudioScroller);
    audioDeviceManager.addAudioCallback(&recorder);

}

RecordingComponent::~RecordingComponent()
{
    audioDeviceManager.removeAudioCallback(&recorder);
    audioDeviceManager.removeAudioCallback(&liveAudioScroller);
}

void RecordingComponent::paint (juce::Graphics& g)
{
    // set background colour
    g.setColour(Colour::fromString("#000000"));
    g.fillRect(getLocalBounds());

    // DRAW APP LOGO
    // DJ part
    g.setFont(juce::Font("Ubuntu Condenced", 34.0f, juce::Font::bold));
    g.setColour(Colour::fromString("#febc27"));
    g.drawText("DJ", getLocalBounds().removeFromTop(50).removeFromRight(980), Justification::left, 2);
    // Audio Mixer part
    g.setFont(juce::Font("Lobster", 30.0f, juce::Font::italic));
    g.setColour(juce::Colours::white);
    g.drawText("Mixing Studio", getLocalBounds().removeFromTop(50).removeFromRight(940), Justification::left, 2);
}

void RecordingComponent::resized()
{

    double rowH = getHeight() / 9;
    double rowW = getWidth() / 6;

    recordButton.setBounds(rowW *3.5, 15, rowW * 0.5 - 15, rowH*1.25);
    recordingName.setBounds(rowW*4, 10, rowW-10, rowH * 1.75);
    loadToPlaylistButton.setBounds(rowW * 5, 10, rowW - 15, rowH*1.75);

    liveAudioScroller.setBounds(0, rowH*3, getWidth(), rowH*3);
    recordingThumbnail.setBounds(0, rowH * 6, getWidth(), rowH * 3);
}

void RecordingComponent::startRecording()
{
    if (!RuntimePermissions::isGranted(RuntimePermissions::writeExternalStorage))
    {
        SafePointer<RecordingComponent> safeThis(this);

        RuntimePermissions::request(RuntimePermissions::writeExternalStorage,
            [safeThis](bool granted) mutable
            {
                if (granted)
                    safeThis->startRecording();
            });
        return;
    }

#if (JUCE_ANDROID || JUCE_IOS)
    auto parentDir = File::getSpecialLocation(File::tempDirectory);
#else
    // save recording file in documents folder
    auto parentDir = File::getSpecialLocation(File::userDocumentsDirectory);
#endif

    if (recordingName.getText().length() == 0)
    {
        // default file name 
        fileName = "my_voice_recording";
    }
    else 
    {
        // user selected file name
        fileName = recordingName.getText();
    }
    lastRecording = parentDir.getNonexistentChildFile(fileName, ".wav");

    recorder.startRecording(lastRecording);

    recordButton.setButtonText("stop");
    recordingThumbnail.setDisplayFullThumbnail(false);
}

void RecordingComponent::stopRecording()
{
    recorder.stop();
    recordButton.setButtonText("record");

#if JUCE_CONTENT_SHARING
    SafePointer<RecordingComponent> safeThis(this);
    File fileToShare = lastRecording;

    ContentSharer::getInstance()->shareFiles(Array<URL>({ URL(fileToShare) }),
        [safeThis, fileToShare](bool success, const String& error)
        {
            if (fileToShare.existsAsFile())
                fileToShare.deleteFile();

            if (!success && error.isNotEmpty())
                NativeMessageBox::showAsync(MessageBoxOptions()
                    .withIconType(MessageBoxIconType::WarningIcon)
                    .withTitle("Sharing Error")
                    .withMessage(error),
                    nullptr);
        });
#endif

    lastRecording = File();
    recordingThumbnail.setDisplayFullThumbnail(true);
}


