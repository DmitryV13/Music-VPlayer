#include "AnalyserComponent.h"

AnalyserComponent::AnalyserComponent()
    : forwardFFT(fftOrder),
    window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    setOpaque(true);
    setAudioChannels(0, 0);
    startTimerHz(30);
    setSize(700, 500);

    initialize();
}

AnalyserComponent::~AnalyserComponent() 
{
    shutdownAudio();
}

void AnalyserComponent::initialize()
{
    width = getLocalBounds().getWidth();
    height = getLocalBounds().getHeight();

    amplDataPrev.resize(static_cast<int>(scopeSize / sizeOfInterval));
    for (size_t i = 0; i < static_cast<int>(scopeSize / sizeOfInterval); i++)
    {
        amplDataPrev[i] = 0;
    }
}

void AnalyserComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) 
{
    if (bufferToFill.buffer->getNumChannels() > 0)
    {
        auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
            pushNextSampleIntoFifo(channelData[i]);
    }
}

void AnalyserComponent::fillBufferWithData(const float* data, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        pushNextSampleIntoFifo(data[i]);
    }
}

void AnalyserComponent::paint(juce::Graphics& g) 
{
    g.fillAll(juce::Colour::fromRGB(21, 23, 24));

    g.setColour(juce::Colour::fromRGB(53, 70, 213));
    g.drawRect(getLocalBounds());

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::white);
    drawFrame(g);
}

void AnalyserComponent::timerCallback() 
{
    if (nextFFTBlockReady)
    {
        calculateFFTForNextData();
        nextFFTBlockReady = false;
        repaint();
    }
}

void AnalyserComponent::pushNextSampleIntoFifo(float sample) noexcept
{
    if (fifoIndex == fftSize)            
    {
        if (!nextFFTBlockReady)          
        {
            juce::zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;         
}

void AnalyserComponent::calculateFFTForNextData()
{
    window.multiplyWithWindowingTable(fftData, fftSize);      


    forwardFFT.performFrequencyOnlyForwardTransform(fftData); 

    auto mindB = -100.0f;
    auto maxdB = 0.0f;

    float tmpIndex = 0;
    bool foundedStartI = false;
    bool foundedEndI = false;
    for (int i = 0; i < scopeSize; ++i)                      
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
        auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(fftData[fftDataIndex])
            - juce::Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);

        //if (!foundedStartI) {
        //    if (startFrequency > (fftDataIndex * (sampleRate / fftSize))) 
        //    {
        //        tmpIndex = i;
        //    }
        //    else 
        //    {
        //        startFIndex = i;
        //        foundedStartI = true;
        //    }
        //}
        //else if(!foundedEndI)
        //{
        //    if (endFrequency > (fftDataIndex * (sampleRate / fftSize)))
        //    {
        //        tmpIndex = i;
        //    }
        //    else
        //    {
        //        endFIndex = i;
        //        foundedEndI = true;
        //    }
        //}

        scopeData[i] = level;                                 
    }
    //float frequency = fftDataIndex * (sampleRate / fftSize);

}

void AnalyserComponent::drawFrame(juce::Graphics& g)
{
    for (int i = startFIndex; i <= endFIndex; i += sizeOfInterval)
    {
        rectRepresentation(g, i);
        //lineRepresentation(g, i);
    }
}

void AnalyserComponent::lineRepresentation(juce::Graphics& g, int i)
{
    g.drawLine({ (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                              juce::jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                      (float)juce::jmap(i,     0, scopeSize - 1, 0, width),
                              juce::jmap(scopeData[i],     0.0f, 1.0f, (float)height, 0.0f) });
}

void AnalyserComponent::rectRepresentation(juce::Graphics& g, int i)
{
    float maxAmplitude =
        (*std::max_element(scopeData + (i - 1), scopeData + (i - 1) + sizeOfInterval));

    float freqX = juce::jmap(i - 1, 0, scopeSize - 1, screenSpace, width - screenSpace);
    int index = 
        static_cast<int>(freqX / ((width - screenSpace * 2) / (scopeSize / sizeOfInterval)));

    if (amplDataPrev[index] > maxAmplitude) {
        amplDataPrev[index] -= 0.01;
        if (amplDataPrev[index] < 0.15) {
            amplDataPrev[index] = 0.15;
        }
        maxAmplitude = amplDataPrev[index];
    }
    else {
        amplDataPrev[index] = maxAmplitude;
    }

    float freqY = juce::jmap(maxAmplitude, 0.0f, 1.0f, (float)height, 0.0f);
    //debug.addLog(std::to_string(width));
    g.setColour(juce::Colour::fromRGB(53, 70, 213));
    g.drawRect(
        freqX,
        freqY,
        (float)((width - screenSpace * 2) / (scopeSize / sizeOfInterval)),
        (float)(height - freqY));
}

void AnalyserComponent::setSampleRate(double sRate)
{
    sampleRate = sRate;
}

void AnalyserComponent::setStartFrequency(double newFreq)
{
    startFrequency = newFreq;
}

void AnalyserComponent::setEndFrequency(double newFreq)
{
    endFrequency = newFreq;
}

void AnalyserComponent::resetFIndexes()
{
    startFIndex = 0;
    endFIndex = scopeSize - 1;
}

void AnalyserComponent::updateCurrentBounds()
{
    width = getLocalBounds().getWidth();
    height = getLocalBounds().getHeight();
}

void AnalyserComponent::attachFileState(bool* state)
{
    fileLoaded = state;
}

