#pragma once

#include <JuceHeader.h>
#include "Windows.h"
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/MyDrawableButton.h"

class SongButtonItem : public juce::Component
{
private:
	bool pressed;

	float width;
	float height;

	std::string path = "";
	std::string sName = "The fear the bloody boys yyejb iuirs rwug";
	std::string artist = "Unknown artist";

	juce::Image diskImage;

	juce::Colour sNameColor;
	juce::Colour sNameActive;
	juce::Colour sNameNActive;

	std::function<void()> onSongClickedHandler;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SongButtonItem)
public:
	SongButtonItem(float width_, float height_, juce::File file);
	SongButtonItem(float x, float y, float width_, float height_, juce::File file);
	~SongButtonItem();


	void resized() override;
	void paint(juce::Graphics& g) override;

	void activateColor();
	void disactivateColor();

	std::string getSongPath();
	bool isItemPressed();
	void unpressItem();

	void playOnButtonClicked();

	void onSongClicked(std::function<void()> handler);
	void triggerOnSongClickedEvent();

	MyDrawableButton* playButton;
};

