/*
  ==============================================================================

    RecordingComponent.h
    Created: 8 Mar 2022 12:02:23pm
    Author:  goaim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DemoUtilities.h"
#include "AudioLiveScrollingDisplay.h"
#include "RecordingThumbnail.h"
#include "AudioRecorder.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class RecordingComponent  : public juce::Component,
                            public TextEditor::Listener
{
public:
    RecordingComponent();
    ~RecordingComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** pointer to trackLobrary.txt */
    std::unique_ptr<juce::File> trackFile;

private:
    // stores file name of user recording
    juce::String fileName;

    // if this PIP is running inside the demo runner, we'll use the shared device manager instead
#ifndef JUCE_DEMO_RUNNER
    AudioDeviceManager audioDeviceManager;
#else
    AudioDeviceManager& audioDeviceManager{ getSharedAudioDeviceManager(1, 0) };
#endif

    
    LiveScrollingAudioDisplay liveAudioScroller;
    RecordingThumbnail recordingThumbnail;
    AudioRecorder recorder{ recordingThumbnail.getAudioThumbnail() };

    // recording component widgets
    TextEditor recordingName;
    TextButton recordButton{ "record" };
    TextButton loadToPlaylistButton{ "load to playlist" };

    // stores most recent recording
    File lastRecording;

    /** handles start of recording */
    void startRecording();
    /** handles end of recording */
    void stopRecording();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordingComponent)
};
