#include "MySlider.h"

MySlider::MySlider()
{
}

MySlider::~MySlider()
{
}

void MySlider::mouseDrag(const juce::MouseEvent& event)
{
	juce::Slider::mouseDrag(event);
	if (transportSource)
	{
		transportSource->setPosition(getValue());
	}
}

void MySlider::attachSource(juce::AudioTransportSource* source)
{
	transportSource = source;
}


