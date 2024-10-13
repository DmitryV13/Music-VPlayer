#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()
    : state(Stopped), virtualSIClick(true)
{
    fileLoaded = new bool(false);

    // init for audio formats
    formatManager.registerBasicFormats();

    // adding change listener for AudioTransportSource 
    transportSource.addChangeListener(this);

    // timer
    startTimer(40);

    buttonsInit();


    list = new SongsButtonsListComponent(900, 100);
    addAndMakeVisible(list);
    list->onSongIClicked([this]() {this->updateOnSongListClicked(); });

    //
    setSize(windowWidth, windowHeight);//800

    // viewport
    //addAndMakeVisible(viewport);
    //viewport.setViewedComponent(list->getListContainer(), true);

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
    juce::Rectangle<int> newArea2(10, buttonHeight + 10, 1, 1);
    newArea2.setHeight(getHeight() - (buttonHeight * 4.0));// getHeight() - (buttonHeight * 4.0)
    newArea2.setWidth(780);// getWidth() - 20
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
    openButton->onClick = [this] { openOnButtonClicked(); };
    openButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(19, 21, 23));

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    folderButton = new MyDrawableButton(
        "Folder",
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    folderButton->setSize(buttonWidth, buttonHeight);
    folderButton->addDrawableImage("imgs/folder-100.png", 0);
    folderButton->resetImages();
    addAndMakeVisible(folderButton);
    folderButton->onClick = [this] { folderOnButtonClicked(); };
    folderButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(19, 21, 23));

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
    playButton->onClick = [this] { playOnButtonClicked(); };
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
    sReplayButton->onClick = [this] { sReplayOnButtonClicked(); };
    sReplayButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(40, 50, 70));
    sReplayButton->setEnabled(false);

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    sNextButton = new MyDrawableButton(
        "Next",
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    sNextButton->setSize(buttonWidth, buttonHeight);
    sNextButton->addDrawableImage("imgs/forward-100.png", 0);
    sNextButton->resetImages();
    addAndMakeVisible(sNextButton);
    sNextButton->onClick = [this] { sReplayOnButtonClicked(); };
    sNextButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(40, 50, 70));
    sNextButton->setEnabled(false);

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    sPreviousButton = new MyDrawableButton(
        "Previous",
        juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    sPreviousButton->setSize(buttonWidth, buttonHeight);
    sPreviousButton->addDrawableImage("imgs/backward-100.png", 0);
    sPreviousButton->resetImages();
    addAndMakeVisible(sPreviousButton);
    sPreviousButton->onClick = [this] { sReplayOnButtonClicked(); };
    sPreviousButton->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(40, 50, 70));
    sPreviousButton->setEnabled(false);
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
    sNextButton->setEnabled(false);
    sPreviousButton->setEnabled(false);
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

void  MainComponent::playOnButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    else if (state == Playing)
        changeState(Pausing);

    if (virtualSIClick) {
        list->virtualClick();
    }
}

void MainComponent::sReplayOnButtonClicked()
{
    if (state == Paused)
        changeState(Stopped);
    else
        changeState(Stopping);
}

void MainComponent::voiceOffOnButtonClicked()
{
}

void MainComponent::openOnButtonClicked()
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
           
                list->addSong(file);

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
                    sNextButton->setEnabled(true);
                    sPreviousButton->setEnabled(true);

                    readerSource.reset(newSource.release());  
                    
                    songProgressBar.attachSource(&transportSource);
                    songProgressBar.setRange(0, transportSource.getLengthInSeconds(), 0.1);
                    songDurationComponent.setTotalLength(transportSource.getLengthInSeconds());
                }
            }
        });
}

void MainComponent::folderOnButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Choose folder...",
        juce::File{},
        "", // Оставляем фильтр пустым, так как нам нужно выбрать папку
        true); // Последний аргумент - true, чтобы выбрать только папки

    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectDirectories; // Добавляем возможность выбора директорий

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto folder = fc.getResult(); // Получаем выбранную папку
            if (folder != juce::File{})
            {

                // Например, если вы хотите добавить все аудиофайлы из этой папки:
                juce::Array<juce::File> audioFiles = folder.findChildFiles(juce::File::findFiles, false, "*.wav;*.aif;*.aiff;*.mp3");
                
                if (audioFiles.size() != 0) {
                    list->setFolderName(folder.getFileName().toStdString());
                }

                for (auto& file : audioFiles)
                {
                    list->addSong(file);
                    
                }
            }
        });
}

void MainComponent::closeButtonClicked()
{
    clearDataForNextSource();
    closeButton->setEnabled(false);
}

void MainComponent::sNextOnButtonClicked()
{
}

void MainComponent::sPreviousOnButtonClicked()
{
}

void MainComponent::updateOnSongListClicked()
{
    std::pair<int, int> playingPressed = list->getIndexes();
    if (playingPressed.first != playingPressed.second) {
        list->updateIndexes();
        auto file = juce::File(list->getSongPath(playingPressed.second));
        if (file != juce::File{})
        {

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
                sNextButton->setEnabled(true);
                sPreviousButton->setEnabled(true);

                readerSource.reset(newSource.release());

                songProgressBar.attachSource(&transportSource);
                songProgressBar.setRange(0, transportSource.getLengthInSeconds(), 0.1);
                songDurationComponent.setTotalLength(transportSource.getLengthInSeconds());

            }
        }
    }
    virtualSIClick = false;
    playButton->clicked();
    playOnButtonClicked();
    virtualSIClick = true;
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
    // background
    g.fillAll(juce::Colour::fromRGB(29, 30, 34));

    // top menu rect
    g.setColour(juce::Colour::fromRGB(19, 21, 23));
    g.fillRect(juce::Rectangle<float>(0, 0, windowWidth, buttonHeight));

    // for future
    if (!fileLoaded)   
        paintIfNoFileLoaded(g);
    else
        paintIfFileLoaded(g);

    // song name
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
    juce::Rectangle<int> windowA = getLocalBounds();//37, 196, 242
    int buttonNumber = 5;
    int buttonSpacing = 20;

    int cordX = 20;
    int cordY = windowA.getHeight() - (buttonHeight * 1.5);

    openButton->setBounds(cordX, 10, buttonWidth - 20, buttonHeight - 20);
    folderButton->setBounds(cordX + buttonWidth - 20 + 10, 10, buttonWidth - 20, buttonHeight - 20);

    //cordX += buttonWidth + buttonSpacing;
    sPreviousButton->setBounds(cordX, cordY, buttonWidth, buttonHeight);
    cordX += buttonWidth + buttonSpacing;
    playButton->setBounds(cordX, cordY, buttonWidth, buttonHeight);
    cordX += buttonWidth + buttonSpacing;
    sNextButton->setBounds(cordX, cordY, buttonWidth, buttonHeight);
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
    newArea2.setWidth(780);//getWidth() - 20
    analyserComponent.setCentrePosition(
        10 + (newArea2.getWidth() / 2),
        buttonHeight + 10 + (newArea2.getHeight() / 2));
    analyserComponent.updateCurrentBounds();

    // slider
    songProgressBar.setBounds(
        10, 
        getHeight() - (buttonHeight * 2.3), 
        780,    // getWidth() - 20
        30);

    // viewport
   


    list->setBounds(getLocalBounds().getWidth() - 260, 0, 260, getLocalBounds().getHeight());
    //viewport.setBounds(juce::Rectangle<int>(800, 0, 260, 540));
}