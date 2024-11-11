#include "TimeComponent.h"

TimeComponent::TimeComponent() 
    : totalHours(0), totalMinutes(0), totalSeconds(0), currentPosition(0)
{
    
}

TimeComponent::~TimeComponent()
{
}

void TimeComponent::setTotalLength(double length)
{
    int totalLength = static_cast<int>(length);

    totalHours = totalLength / 3600;
    totalMinutes = (totalLength % 3600) / 60;
    totalSeconds = (totalLength % 3600) % 60;

    std::string sTotalHours = ((totalHours == 0) ? "" : std::to_string(totalHours) + ":");
    std::string sTotalMinutes = ((totalMinutes == 0) ? "" : std::to_string(totalMinutes) + ":");
    std::string sTotalSeconds = ((totalSeconds / 10 == 0) ? "0" : "") + std::to_string(totalSeconds);

    sTotalLength = sTotalHours + sTotalMinutes + sTotalSeconds;
}

void TimeComponent::setCurrentPosition(double pos)
{
    currentPosition = static_cast<int>(pos);
}

void TimeComponent::paint(juce::Graphics& g)
{
    // border
    //g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 7.0, 2.0);

    //content
    std::string time;

    int hours = currentPosition / 3600;
    int minutes = (currentPosition % 3600) / 60;
    int seconds = (currentPosition % 3600) % 60;

    std::string sHours = ((hours == 0 && totalHours == 0) ? "" : std::to_string(hours) + ":");
    std::string sMinutes = ((minutes == 0 && totalMinutes == 0) ? "" : std::to_string(minutes) + ":");
    std::string sSeconds = ((seconds / 10 == 0) ? "0" : "") + std::to_string(seconds);


    time =
        sHours + sMinutes + sSeconds + " / " + sTotalLength;

    //g.setColour(juce::Colours::black);
    g.drawText(time, getLocalBounds(), juce::Justification::centred, true);

}
