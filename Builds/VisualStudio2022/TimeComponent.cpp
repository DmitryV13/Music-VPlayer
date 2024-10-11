#include "TimeComponent.h"

TimeComponent::TimeComponent() : totalLength(0), currentPosition(0)
{
    
}

TimeComponent::~TimeComponent()
{
}

void TimeComponent::setTotalLength(double length)
{
    totalLength = static_cast<int>(length);
}

void TimeComponent::setCurrentPosition(double pos)
{
    currentPosition = static_cast<int>(pos);
}

void TimeComponent::attachFileState(bool* state)
{
    fileLoaded = state;
}

void TimeComponent::paint(juce::Graphics& g)
{
    //g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 7.0, 2.0);
    std::string time;
    int totalHours = totalLength / 3600;
    int totalMinutes = (totalLength % 3600) / 60;
    int totalSeconds = (totalLength % 3600) % 60;

    int hours = currentPosition / 3600;
    int minutes = (currentPosition % 3600) / 60;
    int seconds = (currentPosition % 3600) % 60;

    time =
        ((hours == 0 && totalHours == 0) ? "" : std::to_string(hours) + ":") +
        ((minutes == 0 && totalMinutes == 0) ? "" : std::to_string(minutes) + ":") +
        std::to_string(seconds) + " / " +
        ((totalHours == 0) ? "" : std::to_string(totalHours) + ":") +
        ((totalMinutes == 0) ? "" : std::to_string(totalMinutes) + ":") +
        std::to_string(totalSeconds);

    //g.setColour(juce::Colours::black);
    g.drawText(time, getLocalBounds(), juce::Justification::centred, true);

}
