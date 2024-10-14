#pragma once

#include <JuceHeader.h>
#include "Windows.h"
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/SongButtonItem.h"
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/DrawablePTunableButton.h"

class SongsButtonsListComponent : public juce::Component
{
private:
	int playingIndexB;
	int pressedIndexB;
	int pressedIndexI;

	bool buttonPressed;
	bool itemPressed;

	std::string folderName = "Folder unspecified";
	std::string folderPath;

	std::unique_ptr<Component> container = nullptr;
	std::vector<SongButtonItem*> songs;

	juce::Viewport viewport;

	std::function<void()> onSongIClickedHandler;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SongsButtonsListComponent)
public:
	SongsButtonsListComponent(float width_, float height_);
	~SongsButtonsListComponent();

	void resized() override;
	void paint(juce::Graphics& g) override;
	void release();

	std::string getSongPath(int i);
	std::vector<int> getIndexes();
	void updateIndexes();

	void virtualClick();
	void changeItemNormalImageDefault();

	bool isButtonPressed();
	bool isItemPressed();
	void resetPressedFlags();

	void onSongPlayButtonClicked();
	void onSongItemClicked();
	bool addSong(juce::File file);
	void setFolderName(std::string folder);

	void onSongIClicked(std::function<void()> handler);
	void triggerOnSongButtonClicked();


	juce::Component* getListContainer();
};

