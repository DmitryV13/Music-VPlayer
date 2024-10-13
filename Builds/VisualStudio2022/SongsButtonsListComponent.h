#pragma once

#include <JuceHeader.h>
#include "Windows.h"
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/SongButtonItem.h"
#include "/Applications/Desktop/MV/FFTv1.0/Builds/VisualStudio2022/DrawablePTunableButton.h"

class SongsButtonsListComponent : public juce::Component
{
private:
	int playingIndex;
	int pressedIndex;

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

	std::string getSongPath(int i);
	std::pair<int, int> getIndexes();
	void updateIndexes();

	void virtualClick();

	void onSongClicked();
	void addSong(juce::File file);
	void setFolderName(std::string folder);

	void onSongIClicked(std::function<void()> handler);
	void triggerOnSongIClickedEvent();


	juce::Component* getListContainer();
};

