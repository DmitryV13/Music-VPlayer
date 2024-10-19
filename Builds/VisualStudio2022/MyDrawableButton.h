#pragma once
#include <JuceHeader.h>

class MyDrawableButton : public juce::DrawableButton
{
protected:
	std::vector<juce::DrawableImage*> dImgs;
	std::vector<juce::DrawableImage*> cDImgs; 
	int index;
	const int initialIndex;
	bool clickedChangingState;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyDrawableButton)
public:
	MyDrawableButton(const juce::String& name, const DrawableButton::ButtonStyle buttonStyle);

	void addClikedImage(std::string path);
	void addDrawableImage(std::string path, int i);
	void clicked() override;


	void changeClickedChangingState(bool state);
	bool getClickedChangingState();
	bool isNormalImageInitial();

	void changeNormalImageDefault();

	void resetImages();
};

