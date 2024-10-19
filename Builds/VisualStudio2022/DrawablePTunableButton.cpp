#include "DrawablePTunableButton.h"

DrawablePTunableButton::DrawablePTunableButton(
	const juce::String& name, const DrawableButton::ButtonStyle buttonStyle)
	: MyDrawableButton(name, buttonStyle)
{
	setSize(40, 40);
}

DrawablePTunableButton::~DrawablePTunableButton()
{
}

void DrawablePTunableButton::paint(juce::Graphics& g)
{
	float x = getLocalBounds().getTopLeft().getX();
	float y = getLocalBounds().getTopLeft().getY();
	float w = getLocalBounds().getWidth();
	float h = getLocalBounds().getHeight();

	g.setColour(backgroundColor);
	g.fillRoundedRectangle(x, y, w, h, cornerSize);

	g.setColour(borderColor);
	g.drawRoundedRectangle(x, y, w, h, cornerSize, 2);

	//juce::Rectangle<float> newArea3(w / 2, h / 2, w / 2, h / 2);
	//g.drawImage(dImgs[0]->getImage(), newArea3);
}

void DrawablePTunableButton::setOutlineColor(juce::Colour color)
{
	borderColor = color;
}

void DrawablePTunableButton::setBackgroundColor(juce::Colour color)
{
	backgroundColor = color;
}

void DrawablePTunableButton::setCornerSize(float size)
{
	cornerSize = size;
}
