#pragma once

#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/MyDrawableButton.h"

class DrawablePTunableButton : public MyDrawableButton
{
private:
	juce::Colour borderColor;
	juce::Colour backgroundColor;
	float cornerSize;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawablePTunableButton)
public:
	DrawablePTunableButton(const juce::String& name, const DrawableButton::ButtonStyle buttonStyle);
	~DrawablePTunableButton();

	void paint(juce::Graphics& g) override;
	void setOutlineColor(juce::Colour color);
	void setBackgroundColor(juce::Colour color);
	void setCornerSize(float size);
};

