#include "SongsButtonsListComponent.h"

SongsButtonsListComponent::SongsButtonsListComponent(float width_, float height_)
    :playingIndex(-1), pressedIndex(-1)
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

std::string SongsButtonsListComponent::getSongPath(int i)
{
    if (i<0 || i> songs.size() - 1) {
        return "";
    }
    else {
        return songs[i]->getSongPath();
    }
}

std::pair<int, int> SongsButtonsListComponent::getIndexes()
{
    return std::pair<int, int>(playingIndex, pressedIndex);
}

void SongsButtonsListComponent::updateIndexes()
{
    if (playingIndex >= 0) {
        songs[playingIndex]->unpressItem();
    }
    playingIndex = pressedIndex;
    songs[playingIndex]->activateColor();
}

void SongsButtonsListComponent::virtualClick()
{
    songs[pressedIndex]->playButton->clicked();
}

void SongsButtonsListComponent::addSong(juce::File file)
{
    SongButtonItem* tmp = new SongButtonItem(10, 170, 240, 70, file);
    songs.push_back(tmp);
    tmp->onSongClicked([this]() {this->onSongClicked();});
    container.get()->addAndMakeVisible(tmp);
    resized();
}

void SongsButtonsListComponent::onSongClicked()
{
    bool itemPressed = false;
    for (int i = 0; i < songs.size(); i++)
    {
        if (i == playingIndex) continue;
        itemPressed = songs[i]->isItemPressed();
        if (itemPressed) {
            pressedIndex = i;
        }
    }
    triggerOnSongIClickedEvent();
}

void SongsButtonsListComponent::setFolderName(std::string folder)
{
    folderName = folder;
}

void SongsButtonsListComponent::onSongIClicked(std::function<void()> handler)
{
    onSongIClickedHandler = handler;
}

void SongsButtonsListComponent::triggerOnSongIClickedEvent()
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
