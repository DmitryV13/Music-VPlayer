#pragma once

#include <JuceHeader.h>
#include "Windows.h"
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/SongButtonItem.h"
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/DrawablePTunableButton.h"

class SongsButtonsListComponent : public juce::Component
{
private:
	std::string folderName = "Folder unspecified";
	std::string folderPath;

	int currentSong;

	std::vector<SongButtonItem*> songs;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SongsButtonsListComponent)
public:
	SongsButtonsListComponent(float width_, float height_);
	~SongsButtonsListComponent();

	void resized() override;

	void paint(juce::Graphics& g) override;

	void folderOnButtonClicked();

	void addSong(juce::File file);
};

