

#pragma once
#include <JuceHeader.h>
#include "Windows.h"
#include <iostream>
#include <fstream>
#include "../Builds/VisualStudio2022/Debug.h"

//==============================================================================
class AnalyserComponent : public juce::AudioAppComponent,
    private juce::Timer
{
public:
    AnalyserComponent();

    ~AnalyserComponent() override;

    //==============================================================================
    void initialize();
    void prepareToPlay(int, double) override {}
    void releaseResources() override {}

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    //==============================================================================

    void timerCallback() override;

    void pushNextSampleIntoFifo(float sample) noexcept;

    void setSampleRate(double sRate);
    void setStartFrequency(double newFreq);
    void setEndFrequency(double newFreq);

    void resetFIndexes();

    void updateCurrentBounds();

    void attachFileState(bool* state);

    void fillBufferWithData(const float* data, int numSamples);

    void paint(juce::Graphics& g) override;

    void calculateFFTForNextData();

    void drawFrame(juce::Graphics& g);

    void lineRepresentation(juce::Graphics& g, int i);

    void rectRepresentation(juce::Graphics& g, int i);

    enum
    {
        fftOrder = 11,            
        fftSize = 1 << fftOrder,  
        // num of total points
        scopeSize = 1000            
    };

private:
    int width;
    int height;

    bool* fileLoaded;

    juce::dsp::FFT forwardFFT;                    
    juce::dsp::WindowingFunction<float> window;  

    float fifo[fftSize];                          
    float fftData[2 * fftSize];                   
    int fifoIndex = 0;                            
    bool nextFFTBlockReady = false;               
    float scopeData[scopeSize]; 
    std::vector<float> amplDataPrev;

    double sampleRate;

    // num of points in an interval
    int sizeOfInterval = 25;
    // interval width in pixels
    int intervalWidth;
    // space
    int screenSpace = 10;

    float startFrequency = 0;
    float endFrequency = 0;
    int startFIndex = 0;
    int endFIndex = scopeSize - 1;

    //int fftOrder = 11;
    //int fftSize = 1 << fftOrder;
    //int scopeSize = 1000;
    Debug debug;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyserComponent)
};
