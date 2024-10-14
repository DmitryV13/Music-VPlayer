#pragma once

#include <JuceHeader.h>
#include "Windows.h"
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/MyDrawableButton.h"

class SongButtonItem : public juce::Component
{
private:
	bool pressedPB;
	bool pressedSI;

	float width;
	float height;

	std::string path = "";
	std::string fullSName = "";
	std::string sName = "The fear the bloody boys yyejb iuirs rwug";
	std::string artist = "Unknown artist";

	juce::Image diskImage;

	juce::Colour sNameColor;
	juce::Colour sNameActive;
	juce::Colour sNameNActive;

	std::function<void()> onSongClickedHandler;
	std::function<void()> onItemClickedHandler;

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
	std::string getSongFullName();
	bool isSongPlayButtonPressed();
	bool isItemPressed();
	void unpressItem();

	void mouseDown(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;

	void playOnButtonClicked();

	void onSongClicked(std::function<void()> handler);
	void triggerOnSongClickedEvent();

	void onItemClicked(std::function<void()> handler);
	void triggerOnItemClickedEvent();

	MyDrawableButton* playButton;
};

