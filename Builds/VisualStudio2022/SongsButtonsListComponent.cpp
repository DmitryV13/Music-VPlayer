#include "SongsButtonsListComponent.h"

SongsButtonsListComponent::SongsButtonsListComponent(float width_, float height_)
    :playingIndexB(-1), pressedIndexB(-1), pressedIndexI(-1), buttonPressed(false), itemPressed(false)
{
    setSize(width_, height_);
    container = std::make_unique<Component>();
    addAndMakeVisible(container.get());
    container.get()->setBounds(0, 62, width_, height_);

    // viewport
    addAndMakeVisible(viewport);
    viewport.setScrollBarsShown(false, false, true, false);
    viewport.setViewedComponent(container.get(), true);
}

SongsButtonsListComponent::~SongsButtonsListComponent()
{
}

void SongsButtonsListComponent::resized()
{
    if (container) {
        float x = getLocalBounds().getTopLeft().getX();
        float y = getLocalBounds().getTopLeft().getY();
        container.get()->setBounds(
            x, 
            62, 
            getLocalBounds().getWidth(), 
            10 + (80 * songs.size()));

        int yPosition = 10; 
        for (int i = 0; i < songs.size(); ++i)
        {
            songs[i]->setBounds(10, yPosition, 240, 70);
            yPosition += 80; 
        }
    }

    viewport.setBounds(juce::Rectangle<int>(0, 62, 260, getLocalBounds().getHeight() - 72));
}

void SongsButtonsListComponent::paint(juce::Graphics& g)
{
    g.setOpacity(1.0f);
    g.fillAll(juce::Colour::fromRGB(19, 21, 23));//19, 21, 23

    // border
    //g.setColour(juce::Colour::fromRGB(53, 70, 213));
    
    float x = getLocalBounds().getTopLeft().getX();
    float y = getLocalBounds().getTopLeft().getY();
    float w = getLocalBounds().getWidth();
    float h = 60;

    juce::ColourGradient gradient(
        juce::Colour::fromRGB(53, 70, 213), 
        x, y,                               
        juce::Colour::fromRGB(82, 223, 199), // juce::Colour::fromRGB(100, 149, 237)
        x + w, y + h,                       
        false);                             // false означает, что это не радиальный градиент

    g.setGradientFill(gradient);
    
    g.fillRect(x, y, w, h);

    g.setColour(juce::Colour::fromRGB(0, 0, 0));
    juce::Font oldF = g.getCurrentFont();
    juce::Font newF = g.getCurrentFont();
    newF.setBold(true);
    newF.setHeight(26);
    g.setFont(newF);
    juce::Rectangle<int> newArea1(15, 0, w - 30, h );
    g.drawText(folderName, newArea1, juce::Justification::centred, true);
    g.setFont(oldF);

    container.get()->paint(g);
}

void SongsButtonsListComponent::release()
{
    folderName = "Folder unspecified";
    playingIndexB = -1;
    pressedIndexB = -1;
    pressedIndexI = -1;

    for (int i = 0; i < songs.size(); i++)
    {
        delete songs[i];
    }
    songs.clear();

}

std::string SongsButtonsListComponent::getSongPath(int i)
{
    if (i<0 || i> songs.size() - 1) {
        return "";
    }
    else {
        return songs[i]->getSongPath();
    }
}

std::vector<int> SongsButtonsListComponent::getIndexes()
{
    std::vector<int> indexes;
    indexes.push_back(playingIndexB);
    indexes.push_back(pressedIndexB);
    indexes.push_back(pressedIndexI);
    return indexes;
}

void SongsButtonsListComponent::updateIndexes()
{
    if (playingIndexB >= 0) {
        songs[playingIndexB]->unpressItem();
    }
    if (pressedIndexI >= 0) {
        playingIndexB = pressedIndexI;
        pressedIndexB = pressedIndexI;
    }
    else {
        playingIndexB = pressedIndexB;
    }
    pressedIndexI = -1;
    songs[playingIndexB]->activateColor();
}

void SongsButtonsListComponent::virtualClick()
{
    songs[pressedIndexB]->playButton->clicked();
}

void SongsButtonsListComponent::changeItemNormalImageDefault()
{
    songs[pressedIndexB]->playButton->changeNormalImageDefault();
}

bool SongsButtonsListComponent::isButtonPressed()
{
    return buttonPressed;
}

bool SongsButtonsListComponent::isItemPressed()
{
    return itemPressed;
}

void SongsButtonsListComponent::resetPressedFlags()
{
    buttonPressed = false;
    itemPressed = false;
}

bool SongsButtonsListComponent::addSong(juce::File file)
{
    std::string fileName = file.getFileNameWithoutExtension().toStdString();
    for (int i = 0; i < songs.size(); i++)
    {
        if (fileName == songs[i]->getSongFullName()) return false;
    }
    SongButtonItem* tmp = new SongButtonItem(10, 170, 240, 70, file);
    songs.push_back(tmp);
    tmp->onSongClicked([this]() {this->onSongPlayButtonClicked();});
    tmp->onItemClicked([this]() {this->onSongItemClicked();});
    container.get()->addAndMakeVisible(tmp);
    resized();
    return true;
}

void SongsButtonsListComponent::onSongPlayButtonClicked()
{
    bool songPlayButtonPressed = false;
    for (int i = 0; i < songs.size(); i++)
    {
        if (i == playingIndexB) continue;
        songPlayButtonPressed = songs[i]->isSongPlayButtonPressed();
        if (songPlayButtonPressed) {
            pressedIndexB = i;
        }
    }
    buttonPressed = true;
    triggerOnSongButtonClicked();
}

void SongsButtonsListComponent::onSongItemClicked()
{
    bool songItemPressed = false;
    for (int i = 0; i < songs.size(); i++)
    {
        if (i == playingIndexB) continue;
        songItemPressed = songs[i]->isItemPressed();
        if (songItemPressed) {
            pressedIndexI = i;
        }
    }
    itemPressed = true;
    triggerOnSongButtonClicked();
}

void SongsButtonsListComponent::setFolderName(std::string folder)
{
    folderName = folder;
}

void SongsButtonsListComponent::onSongIClicked(std::function<void()> handler)
{
    onSongIClickedHandler = handler;
}

void SongsButtonsListComponent::triggerOnSongButtonClicked()
{
    if (onSongIClickedHandler != nullptr)
    {
        onSongIClickedHandler();
    }
}

juce::Component* SongsButtonsListComponent::getListContainer()
{
    return container.get();
}
