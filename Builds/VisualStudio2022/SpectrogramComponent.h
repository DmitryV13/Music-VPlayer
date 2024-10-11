
#pragma once
#include <JuceHeader.h>
#include "Windows.h"
#include <iostream>
#include <fstream>
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/Debug.h"

class SpectrogramComponent : public juce::AudioAppComponent,
                             private juce::Timer
{
public:
    SpectrogramComponent();

    ~SpectrogramComponent() override;

    //==============================================================================
    void prepareToPlay(int, double) override {}
    void releaseResources() override {}

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    //==============================================================================
    void paint(juce::Graphics& g) override;

    void timerCallback() override;

    void pushNextSampleIntoFifo(float sample) noexcept;

    void drawNextLineOfSpectrogram();

    static constexpr auto fftOrder = 10;               
    static constexpr auto fftSize = 1 << fftOrder;     

private:
    juce::dsp::FFT forwardFFT;                         
    juce::Image spectrogramImage;

    std::array<float, fftSize> fifo;                   
    std::array<float, fftSize * 2> fftData;            
    int fifoIndex = 0;                                 
    bool nextFFTBlockReady = false;                    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrogramComponent)
};


