#pragma once
#include <JuceHeader.h>

class TimeComponent : public juce::Component
{
public:
	TimeComponent();

	~TimeComponent() override;

	void setTotalLength(double length);
	void setCurrentPosition(double pos);

	void paint(juce::Graphics& g) override;



private:
	int totalHours;
	int totalMinutes;
	int totalSeconds;

	std::string sTotalLength = "00";
	int currentPosition;
	bool* fileLoaded;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeComponent)
};

