#pragma once

#include <JuceHeader.h>
#include "Windows.h"
#include <iostream>
#include <fstream>
#include "../Builds/VisualStudio2022/Debug.h"
#include "../Builds/VisualStudio2022/AnalyserComponent.h"
#include "../Builds/VisualStudio2022/MyDrawableButton.h"
#include "../Builds/VisualStudio2022/TimeComponent.h"
#include "../Builds/VisualStudio2022/MySlider.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
                      public juce::ChangeListener,
                      private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    void buttonsInit();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

    static constexpr auto fftOrder = 10;
    static constexpr auto fftSize = 1 << fftOrder;
private:
    enum TransportState
    {
        Stopped = 0,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void changeTransportSource();
    void clearDataForNextSource();

    void changeState(TransportState newState);

    void playButtonClicked();
    void sReplayButtonClicked();
    void openButtonClicked();
    void closeButtonClicked();
    void voiceOffOnButtonClicked();

    void paintIfNoFileLoaded(juce::Graphics& g);
    void paintIfFileLoaded(juce::Graphics& g);

    void timerCallback() override;

    // COMPONENTS
    AnalyserComponent analyserComponent;
    TimeComponent songDurationComponent;

    MyDrawableButton* openButton;
    MyDrawableButton* playButton;
    MyDrawableButton* sReplayButton;
    MyDrawableButton* closeButton;
    //juce::ToggleButton voiceOffOn;
    int buttonHeight = 60;
    int buttonWidth = 60;

    MySlider songProgressBar;

    // FOR FILES
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    juce::String songName = "";
    bool* fileLoaded;
 
    // 
    Debug debug;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)


};
