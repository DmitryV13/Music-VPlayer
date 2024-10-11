#include "SpectrogramComponent.h"

SpectrogramComponent::SpectrogramComponent()
    : forwardFFT(fftOrder),
    spectrogramImage(juce::Image::RGB, 512, 512, true)
{
    setOpaque(true);
    setAudioChannels(2, 0);  // we want a couple of input channels but no outputs
    startTimerHz(60);
    setSize(700, 500);
}

SpectrogramComponent::~SpectrogramComponent()
{
    shutdownAudio();
}

void SpectrogramComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (bufferToFill.buffer->getNumChannels() > 0)
    {
        auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
            pushNextSampleIntoFifo(channelData[i]);
    }
}

void SpectrogramComponent::paint(juce::Graphics& g)
{
    //fft
    g.fillAll(juce::Colours::black);

    g.setOpacity(1.0f);

    g.drawImage(spectrogramImage, getLocalBounds().toFloat());
}

void SpectrogramComponent::timerCallback()
{
    if (nextFFTBlockReady)
    {
        drawNextLineOfSpectrogram();
        nextFFTBlockReady = false;
        repaint();
    }
}

void SpectrogramComponent::pushNextSampleIntoFifo(float sample) noexcept
{
    // if the fifo contains enough data, set a flag to say
    // that the next line should now be rendered..
    if (fifoIndex == fftSize)       // [8]
    {
        if (!nextFFTBlockReady)    // [9]
        {
            std::fill(fftData.begin(), fftData.end(), 0.0f);
            std::copy(fifo.begin(), fifo.end(), fftData.begin());
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[(size_t)fifoIndex++] = sample; // [9]
}

void SpectrogramComponent::drawNextLineOfSpectrogram()
{
    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeight = spectrogramImage.getHeight();

    // first, shuffle our image leftwards by 1 pixel..
    spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);         // [1]

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());                   // [2]

    // find the range of values produced, so we can scale our rendering to
    // show up the detail clearly
    auto maxLevel = juce::FloatVectorOperations::findMinAndMax(fftData.data(), fftSize / 2); // [3]

    for (auto y = 1; y < imageHeight; ++y)                                              // [4]
    {
        auto skewedProportionY = 1.0f - std::exp(std::log((float)y / (float)imageHeight) * 0.2f);
        auto fftDataIndex = (size_t)juce::jlimit(0, fftSize / 2, (int)(skewedProportionY * fftSize / 2));
        auto level = juce::jmap(fftData[fftDataIndex], 0.0f, juce::jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

        spectrogramImage.setPixelAt(rightHandEdge, y, juce::Colour::fromHSV(level, 1.0f, level, 1.0f)); // [5]
    }
}