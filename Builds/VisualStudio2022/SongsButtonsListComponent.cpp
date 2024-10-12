#include "SongsButtonsListComponent.h"

SongsButtonsListComponent::SongsButtonsListComponent(float width_, float height_)
{
    setSize(width_, height_);

}

SongsButtonsListComponent::~SongsButtonsListComponent()
{
}

void SongsButtonsListComponent::resized()
{

    int yPosition = 70; 
    for (int i = 0; i < songs.size(); ++i)
    {
        songs[i]->setBounds(10, yPosition, 240, 70);
        yPosition += 80; 
    }
}

void SongsButtonsListComponent::paint(juce::Graphics& g)
{
    g.setOpacity(1.0f);
    g.fillAll(juce::Colour::fromRGB(19, 21, 23));//19, 21, 23

    // border
    g.setColour(juce::Colour::fromRGB(53, 70, 213));

    float x = getLocalBounds().getTopLeft().getX();
    float y = getLocalBounds().getTopLeft().getY();
    float w = getLocalBounds().getWidth();
    float h = 60;
    
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
}

void SongsButtonsListComponent::folderOnButtonClicked()
{
}

void SongsButtonsListComponent::addSong(juce::File file)
{
    SongButtonItem* tmp = new SongButtonItem(10, 170, 240, 70, file);
    songs.push_back(tmp);
    addAndMakeVisible(tmp);
    resized();
}
