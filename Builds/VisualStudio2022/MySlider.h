#pragma once
#include <JuceHeader.h>

class MySlider : public juce::Slider
{
public:
	MySlider();
	~MySlider();

	void attachSource(juce::AudioTransportSource* source);
private:
	void mouseDrag(const juce::MouseEvent& event) override;



	juce::AudioTransportSource* transportSource;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MySlider);
};

