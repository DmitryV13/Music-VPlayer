#include "SongButtonItem.h"

SongButtonItem::SongButtonItem(float width_, float height_, juce::File file)
    : width(width), height(height), pressed(false)
{
    path = file.getFullPathName().toStdString();
    juce::String fileName = file.getFileNameWithoutExtension();
    if (fileName.contains(" - "))
    {
        sName = fileName.fromLastOccurrenceOf(" - ", false, false).toStdString();
        artist = fileName.upToFirstOccurrenceOf(" - ", false, false).toStdString();
    }
    else
    {
        sName = fileName.toStdString();
        artist = "Unknown artist"; 
    }

    setBounds(0, 0, width, height);

    sNameNActive = juce::Colour::fromRGB(223, 223, 223);
    sNameActive = juce::Colour::fromRGB(53, 70, 213);

    playButton = new MyDrawableButton(
        "Play",
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    playButton->setSize(40, 40);
    playButton->addClikedImage("imgs/play-100.png");
    playButton->addClikedImage("imgs/stop-100.png");
    playButton->changeClickedChangingState(true);
    addAndMakeVisible(playButton);
    playButton->onClick = [this] { playOnButtonClicked(); };
    playButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(53, 70, 213));
   // playButton->setEnabled(false);

    //playButton->setInterceptsMouseClicks(true, false);
    playButton->setWantsKeyboardFocus(false);

    juce::File* diskFile = new juce::File("imgs/disk-100.png");
    diskImage = juce::ImageCache::getFromFile(*diskFile);
    delete diskFile;
}

SongButtonItem::SongButtonItem(float x, float y, float width_, float height_, juce::File file)
    : width(width), height(height), pressed(false)
{
    path = file.getFullPathName().toStdString();
    juce::String fileName = file.getFileNameWithoutExtension();
    if (fileName.contains(" - "))
    {
        sName = fileName.fromLastOccurrenceOf(" - ", false, false).toStdString();
        artist = fileName.upToFirstOccurrenceOf(" - ", false, false).toStdString();
    }
    else
    {
        sName = fileName.toStdString();
        artist = "Unknown artist";
    }

    setBounds(x, y, width, height);

    sNameNActive = juce::Colour::fromRGB(223, 223, 223);
    sNameActive = juce::Colour::fromRGB(53, 70, 213);
    sNameColor = sNameNActive;

    playButton = new MyDrawableButton(
        "Play",
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    playButton->setSize(40, 40);
    playButton->addClikedImage("imgs/play-100.png");
    playButton->addClikedImage("imgs/stop-100.png");
    playButton->changeClickedChangingState(true);
    addAndMakeVisible(playButton);
    playButton->onClick = [this] { playOnButtonClicked(); };
    playButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(53, 70, 213));

    //playButton->setInterceptsMouseClicks(true, false);
    playButton->setWantsKeyboardFocus(false);

    juce::File* diskFile = new juce::File("imgs/disk-100.png");
    diskImage = juce::ImageCache::getFromFile(*diskFile);
    delete diskFile;
}

SongButtonItem::~SongButtonItem()
{
    delete playButton;
}

void SongButtonItem::playOnButtonClicked()
{
    pressed = true;
    triggerOnSongClickedEvent();
}

void SongButtonItem::onSongClicked(std::function<void()> handler)
{
    onSongClickedHandler = handler;
}

void SongButtonItem::triggerOnSongClickedEvent()
{
    if (onSongClickedHandler != nullptr)
    {
        onSongClickedHandler();
    }
}

void SongButtonItem::resized()
{
    playButton->setBounds(15, 15, 40, 40);
}

void SongButtonItem::paint(juce::Graphics& g)
{
    g.setOpacity(1.0f);
    g.fillAll(juce::Colour::fromRGB(29, 30, 34));//29, 30, 34

    // border

    g.setColour(juce::Colour::fromRGB(53, 70, 213));
    float cornerSize = 10.0f; 

    float x = getLocalBounds().getTopLeft().getX();
    float y = getLocalBounds().getTopLeft().getY();
    float w = getLocalBounds().getWidth();
    float h = getLocalBounds().getHeight();

    g.drawRoundedRectangle(x, y, w, h, cornerSize, 1);

    // song name
    g.setColour(sNameColor);
    juce::Rectangle<int> newArea1(70, 15, getWidth() - 70, 40);
    g.drawText(sName, newArea1, juce::Justification::topLeft, true);

    // artist 
    g.setColour(juce::Colour::fromRGB(150, 150, 150));
    juce::Rectangle<int> newArea2(90, h / 2, getWidth() - 90, h / 2);
    g.drawText(artist, newArea2, juce::Justification::topLeft, true);

    // disk
    juce::Rectangle<float> newArea3(70, (h / 2) - 1, 15, 15);
    g.drawImage(diskImage, newArea3);
}

void SongButtonItem::activateColor()
{
    sNameColor = sNameActive;
}

void SongButtonItem::disactivateColor()
{
    sNameColor = sNameNActive;
}

std::string SongButtonItem::getSongPath()
{
    return path;
}

bool SongButtonItem::isItemPressed()
{
    return pressed;
}

void SongButtonItem::unpressItem()
{
    disactivateColor();
    playButton->changeNormalImageDefault();
    pressed = false;
}
