#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()
    : state(Stopped)
{
    fileLoaded = new bool(false);

    // init for audio formats
    formatManager.registerBasicFormats();

    // adding change listener for AudioTransportSource 
    transportSource.addChangeListener(this);

    // timer
    startTimer(40);

    buttonsInit();

    //
    setSize(800, 600);

    // slider 
    addAndMakeVisible(songProgressBar);
    songProgressBar.attachSource(nullptr);
    songProgressBar.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    songProgressBar.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(53, 105, 213)); // Цвет кнопки ползунка
    songProgressBar.setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(40, 50, 70));// Цвет трека слайдера
    songProgressBar.setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGB(40, 50, 70)); // Цвет фона слайдера
    //songProgressBar.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blue);// Цвет круга ползунка
    //songProgressBar.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::green);// Цвет контура ползунка

    // song time progress bar
    songDurationComponent.attachFileState(fileLoaded);
    songDurationComponent.setCurrentPosition(0);
    addAndMakeVisible(songDurationComponent);

    // vis bar
    addAndMakeVisible(analyserComponent);
    juce::Rectangle<int> newArea2(10, buttonHeight, 1, 1);
    newArea2.setHeight(getHeight() - (buttonHeight * 4.0));
    newArea2.setWidth(getWidth() - 20);
    analyserComponent.setBounds(newArea2);
    analyserComponent.updateCurrentBounds();


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::buttonsInit()
{
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    openButton = new MyDrawableButton(
        "Open", 
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    openButton->setSize(buttonWidth, buttonHeight);
    openButton->addDrawableImage("imgs/fileNA-100.png", 0);
    openButton->addDrawableImage("imgs/fileA-100.png", 1);
    openButton->resetImages();
    addAndMakeVisible(openButton);
    openButton->onClick = [this] { openButtonClicked(); };
    openButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(40, 50, 70));

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    playButton = new MyDrawableButton(
        "Play", 
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    playButton->setSize(buttonWidth, buttonHeight);
    playButton->addClikedImage("imgs/play-100.png");
    playButton->addClikedImage("imgs/stop-100.png");
    playButton->changeClickedChangingState(true);
    addAndMakeVisible(playButton);
    playButton->onClick = [this] { playButtonClicked(); };
    playButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(53, 105, 213));
    playButton->setEnabled(false);

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    sReplayButton = new MyDrawableButton(
        "Replay", 
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    sReplayButton->setSize(buttonWidth, buttonHeight);
    sReplayButton->addDrawableImage("imgs/replay-100.png", 0);
    sReplayButton->resetImages();
    addAndMakeVisible(sReplayButton);
    sReplayButton->onClick = [this] { sReplayButtonClicked(); };
    sReplayButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(40, 50, 70));
    sReplayButton->setEnabled(false);
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    closeButton = new MyDrawableButton(
        "Close",
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    closeButton->setSize(buttonWidth, buttonHeight);
    closeButton->addDrawableImage("imgs/close-100.png", 0);
    closeButton->resetImages();
    addAndMakeVisible(closeButton);
    closeButton->onClick = [this] { closeButtonClicked(); };
    closeButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(40, 50, 70));
    closeButton->setEnabled(false);
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource) {
        changeTransportSource();
    }
}

void MainComponent::clearDataForNextSource()
{
    *fileLoaded = false;
    transportSource.setSource(nullptr);
    playButton->changeNormalImageDefault();
    playButton->setEnabled(false);
    changeState(Stopped);
    songName = "";
    songProgressBar.setValue(0);
    songDurationComponent.setCurrentPosition(0);
    songDurationComponent.setTotalLength(0);
}


void MainComponent::changeTransportSource()
{
    if (transportSource.isPlaying())
        changeState(Playing);
    else if ((state == Stopping) || (state == Playing))
        changeState(Stopped);
    else if (Pausing == state)
        changeState(Paused);
}

void MainComponent::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
        case Stopped:
            playButton->setButtonText("Play");
            sReplayButton->setButtonText("Stop");
            sReplayButton->setEnabled(false);
            playButton->changeNormalImageDefault();
            transportSource.setPosition(0.0);
            break;

        case Starting:
            transportSource.start();
            break;

        case Playing:
            playButton->setButtonText("Pause");
            sReplayButton->setButtonText("Stop");
            sReplayButton->setEnabled(true);
            break;

        case Pausing:
            transportSource.stop();
            break;

        case Paused:
            playButton->setButtonText("Resume");
            sReplayButton->setButtonText("Return to Zero");
            break;

        case Stopping:
            transportSource.stop();
            break;
        }
    }
}

void  MainComponent::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    else if (state == Playing)
        changeState(Pausing);
}

void MainComponent::sReplayButtonClicked()
{
    if (state == Paused)
        changeState(Stopped);
    else
        changeState(Stopping);
}

void MainComponent::voiceOffOnButtonClicked()
{
}

void MainComponent::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
        juce::File{},
        "*.wav;*.aif;*.aiff;*.mp3");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{})
            {
                //repaint();  //redundant

                auto* reader = formatManager.createReaderFor(file);

                if (reader != nullptr)
                {
                    clearDataForNextSource();
                    *fileLoaded = true;
                    songName = file.getFileName();
                    analyserComponent.attachFileState(fileLoaded);
                    analyserComponent.setSampleRate(reader->sampleRate);

                    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);  
                    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);      
                    playButton->setEnabled(true);
                    closeButton->setEnabled(true);
                    readerSource.reset(newSource.release());  
                    
                    songProgressBar.attachSource(&transportSource);
                    songProgressBar.setRange(0, transportSource.getLengthInSeconds(), 0.1);
                    songDurationComponent.setTotalLength(transportSource.getLengthInSeconds());
                }
            }
        });
}

void MainComponent::closeButtonClicked()
{
    clearDataForNextSource();
    closeButton->setEnabled(false);
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);
    analyserComponent.fillBufferWithData(
        bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample)
        , bufferToFill.numSamples);
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(29, 30, 34));

    if (!fileLoaded)   
        paintIfNoFileLoaded(g);
    else
        paintIfFileLoaded(g);


    g.setColour(juce::Colours::white);
    juce::Rectangle<int> newArea1(0, 0, getWidth(), buttonHeight);
    g.drawText(songName, newArea1, juce::Justification::centred, true);
    
}

void MainComponent::paintIfNoFileLoaded(juce::Graphics& g)
{  
}

void MainComponent::paintIfFileLoaded(juce::Graphics& g)
{
}

void MainComponent::timerCallback()
{
    if (*fileLoaded) {
        songProgressBar.setValue(transportSource.getCurrentPosition());
        songDurationComponent.setCurrentPosition(transportSource.getCurrentPosition());
    }
    repaint();
}

void MainComponent::resized()
{
    // buttons
    juce::Rectangle<int> windowA = getLocalBounds();
    int buttonNumber = 5;
    int buttonSpacing = 20;

    int cordX = 20;
    int cordY = windowA.getHeight() - (buttonHeight * 1.5);

    openButton->setBounds(cordX, cordY, buttonWidth, buttonHeight);
    cordX += buttonWidth + buttonSpacing;
    playButton->setBounds(cordX, cordY, buttonWidth, buttonHeight);
    cordX += buttonWidth + buttonSpacing;
    sReplayButton->setBounds(cordX, cordY, buttonWidth, buttonHeight);
    cordX += buttonWidth + buttonSpacing;
    closeButton->setBounds(cordX, cordY, buttonWidth, buttonHeight);
    cordX += buttonWidth + buttonSpacing;

    // timer
    songDurationComponent.setBounds(
        cordX,
        cordY + ((buttonHeight - 30) / 2), 
        120, 
        30
    );

    // analyser
    juce::Rectangle<int> newArea2(10, buttonHeight, 1, 1);
    newArea2.setHeight(getHeight() - (buttonHeight * 4.0));
    newArea2.setWidth(getWidth() - 20);
    analyserComponent.setCentrePosition(
        10 + (newArea2.getWidth() / 2),
        buttonHeight + (newArea2.getHeight() / 2));
    analyserComponent.updateCurrentBounds();

    // slider
    songProgressBar.setBounds(
        10, 
        getHeight() - (buttonHeight * 2.5), 
        getWidth() - 20, 
        30);
}