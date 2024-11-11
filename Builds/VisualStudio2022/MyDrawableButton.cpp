#include "MyDrawableButton.h"

MyDrawableButton::MyDrawableButton(
    const juce::String& name, const DrawableButton::ButtonStyle buttonStyle)
    : DrawableButton(name, buttonStyle)
    , index(0)
    , initialIndex(0)
    , clickedChangingState(false)
{
    dImgs.resize(8, nullptr);
}

void MyDrawableButton::addClikedImage(std::string path)
{
    juce::File* file = new juce::File(path);
    juce::Image tmpImage = juce::ImageCache::getFromFile(*file);
    juce::DrawableImage* tmpDImage = new juce::DrawableImage();
    tmpDImage->setImage(tmpImage);
    cDImgs.push_back(tmpDImage);
    delete file;
}

void MyDrawableButton::addDrawableImage(std::string path, int i)
{
    if (i >= 0 && i < dImgs.size())
    {
        juce::File* file = new juce::File(path);
        juce::Image tmpImage = juce::ImageCache::getFromFile(*file);
        juce::DrawableImage* tmpDImage = new juce::DrawableImage();
        tmpDImage->setImage(tmpImage);
        dImgs[i] = tmpDImage;
        delete file;
    }
}

void MyDrawableButton::clicked()
{
    if (clickedChangingState) {
        if (index + 1 < cDImgs.size())
        {
            index += 1;
        }
        else
        {
            index = 0;
        }
        dImgs[0] = cDImgs[index];
        resetImages();
    }
}

void MyDrawableButton::changeClickedChangingState(bool state)
{   
    clickedChangingState = state;
    if(clickedChangingState)
    {
        dImgs[0] = cDImgs[index];
        resetImages();
    }
}

bool MyDrawableButton::getClickedChangingState()
{
    return clickedChangingState;
}

void MyDrawableButton::resetImages()
{
    setImages(
        dImgs[0],
        dImgs[1],
        dImgs[2],
        dImgs[3],
        dImgs[4],
        dImgs[5],
        dImgs[6],
        dImgs[7]);
}

bool MyDrawableButton::isNormalImageInitial()
{
    return initialIndex == index;
}

void MyDrawableButton::changeNormalImageDefault()
{
    if (cDImgs.size() != 0) {
        index = 0;
        dImgs[0] = cDImgs[0];
        resetImages();
    }
}
