#include "DataStructure.h"

// Initializer functions
void DataStructure::initVariables()
{
    /* scale sizes */
    this->runButtonDistance *= this->scale_x;
    this->runButtonX *= this->scale_x;
    this->runButtonY *= this->scale_y;
    this->runButtonSizeX *= this->scale_x;
    this->runButtonSizeY *= this->scale_y;

    this->insButtonDistanceY *= this->scale_y;
    this->insButtonX *= this->scale_x;
    this->insButtonY *= this->scale_y;
    this->insButtonSizeX *= this->scale_x;
    this->insButtonSizeY *= this->scale_y;

    this->speedX *= scale_x;
    this->speedY *= scale_y;
    this->speedWidth *= scale_x;
    this->speedHeight *= scale_y;
    this->characterSize *= scale_x * scale_y;

    this->stepX *= scale_x;
    this->stepY *= scale_y;
}

void DataStructure::initBackground()
{
    this->background.setSize(sf::Vector2f(this->window->getSize().x, this->window->getSize().y));
    this->DataStructureThemeStandard();
}

void DataStructure::initButtonTextures()
{
    this->nextButtonTexture.loadFromFile("Resources/Images/nextButton.png");
    this->prevButtonTexture.loadFromFile("Resources/Images/prevButton.png");
    this->lastButtonTexture.loadFromFile("Resources/Images/lastButton.png");
    this->firstButtonTexture.loadFromFile("Resources/Images/firstButton.png");
    this->pauseButtonTexture.loadFromFile("Resources/Images/pauseButton.png");
    this->playButtonTexture.loadFromFile("Resources/Images/playButton.png");
    this->replayButtonTexture.loadFromFile("Resources/Images/replayButton.png");

    this->speedMinusTexture.loadFromFile("Resources/Images/minus-48.png");
    this->speedPlusTexture.loadFromFile("Resources/Images/plus-48.png");

    this->codeLeftArrow.loadFromFile("Resources/Images/code-arrow-left.png");
    this->codeRightArrow.loadFromFile("Resources/Images/code-arrow-right.png");
}

void DataStructure::initFonts()
{
    if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
    {
        throw("ERROR::MAIN MENU STATE::COULD NOT LOAD FONT");
    }
}

void DataStructure::initKeybinds()
{
    std::ifstream ifs("Config/mainMenuState_keybinds.ini");

    if (ifs.is_open())
    {
        std::string key = "";
        std::string key2 = "";

        while (ifs >> key >> key2)
        {
            this->keybinds[key] = this->supportedKeys->at(key2);
        }
    }

    ifs.close();
}

void DataStructure::initText()
{
    //Speed
    auto SpeedInit = [&]() {
        speedText.setString("1x");
        speedText.setCharacterSize(1.5 * this->characterSize);
        speedText.setFont(this->font);
    };

    //Input warning
    auto InputWarningInit = [&]()
    {
        inputWarning.setString("");
        inputWarning.setCharacterSize(this->characterSize);
        inputWarning.setPosition(this->buttons["Theme"]->shape.getPosition().x,
                                 this->buttons["Theme"]->shape.getPosition().y - this->buttons["Theme"]->shape.getGlobalBounds().height - 10 * scale_y);
        inputWarning.setFont(this->font);
        inputWarning.setStyle(inputWarning.Bold);
        inputWarning.setString("");
    };

    auto InputGuideInit = [&]()
    {
        inputGuide.setString("");
        inputGuide.setCharacterSize(this->characterSize);
        inputGuide.setPosition(this->buttons["Theme"]->shape.getPosition().x,
                                 this->buttons["Theme"]->shape.getPosition().y - 2 * this->buttons["Theme"]->shape.getGlobalBounds().height - 20 * scale_y);
        inputGuide.setFont(this->font);
        inputGuide.setStyle(inputGuide.Bold);
        inputGuide.setString("");
    };

    // Step
    auto stepInit = [&]() {
        stepText.setString("Step: 0 / 0");
        stepText.setPosition(stepX, stepY);
        stepText.setCharacterSize(1.3 * characterSize);
        stepText.setFont(this->font);
        stepText.setStyle(stepText.Bold);
    };

    SpeedInit();
    InputWarningInit();
    InputGuideInit();
    stepInit();
    this->updateTexts();
}

void DataStructure::initButtons()
{
    // Run buttons
    auto RunButtons = [&]()
    {
        this->buttons["First"] = new Button(
            this->runButtonX + 0 * this->runButtonDistance, this->runButtonY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->runButtonSizeX, this->runButtonSizeY);
        this->buttons["First"]->shape.setTexture(&this->firstButtonTexture);
        this->buttons["First"]->instantTrigger = true;


        this->buttons["Prev"] = new Button(
            this->runButtonX + 1 * this->runButtonDistance, this->runButtonY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->runButtonSizeX, this->runButtonSizeY);
        this->buttons["Prev"]->shape.setTexture(&this->prevButtonTexture);
        this->buttons["Prev"]->instantTrigger = true;


        this->buttons["Pause"] = new Button(
            this->runButtonX + 2 * this->runButtonDistance, this->runButtonY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->runButtonSizeX, this->runButtonSizeY);
        this->buttons["Pause"]->shape.setTexture(&this->pauseButtonTexture);
        this->buttons["Pause"]->instantTrigger = true;


        this->buttons["Play"] = new Button(
            this->runButtonX + 2 * this->runButtonDistance, this->runButtonY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->runButtonSizeX, this->runButtonSizeY);
        this->buttons["Play"]->shape.setTexture(&this->playButtonTexture);
        this->buttons["Play"]->disabled = true;
        this->buttons["Play"]->instantTrigger = true;

        this->buttons["Replay"] = new Button(
            this->runButtonX + 2 * this->runButtonDistance, this->runButtonY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->runButtonSizeX, this->runButtonSizeY);
        this->buttons["Replay"]->shape.setTexture(&this->replayButtonTexture);
        this->buttons["Replay"]->disabled = true;
        this->buttons["Replay"]->instantTrigger = true;


        this->buttons["Next"] = new Button(
            this->runButtonX + 3 * this->runButtonDistance, this->runButtonY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->runButtonSizeX, this->runButtonSizeY);
        this->buttons["Next"]->shape.setTexture(&this->nextButtonTexture);
        this->buttons["Next"]->instantTrigger = true;


        this->buttons["Last"] = new Button(
            this->runButtonX + 4 * this->runButtonDistance, this->runButtonY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->runButtonSizeX, this->runButtonSizeY);
        this->buttons["Last"]->shape.setTexture(&this->lastButtonTexture);
        this->buttons["Last"]->instantTrigger = true;
    };

    // Instructions
    auto InstructionButtons = [&]()
    {   
        this->buttons["Theme"] = new Button(
            insButtonX, insButtonY + 0 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Theme", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);

        this->buttons["Initialize"] = new Button(
            insButtonX, insButtonY + 1 * insButtonDistanceY , scale_x, scale_y,
            &this->font, "Initialize", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);

        this->buttons["Add"] = new Button(
            insButtonX, insButtonY + 2 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Insert", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);

        this->buttons["Delete"] = new Button(
            insButtonX, insButtonY + 3 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Delete", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);

        this->buttons["Update"] = new Button(
            insButtonX, insButtonY + 4 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Update", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);

        this->buttons["Search"] = new Button(
            insButtonX, insButtonY + 5 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Search", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);

        this->buttons["Quit"] = new Button(
            insButtonX, insButtonY + 6 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Quit", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);
    };

    //Speed
    auto SpeedButtons = [&]() {
        this->buttons["SpeedMinus"] = new Button(
            this->speedX - this->speedWidth * 2, this->speedY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->speedWidth, this->speedHeight);
        this->buttons["SpeedMinus"]->shape.setTexture(&this->speedMinusTexture);
        this->buttons["SpeedMinus"]->instantTrigger = true;


        this->buttons["SpeedPlus"] = new Button(
            this->speedX + this->speedWidth * 2, this->speedY, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
            sf::Color(255, 255, 255, 125), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
            1, this->speedWidth, this->speedHeight);
        this->buttons["SpeedPlus"]->shape.setTexture(&this->speedPlusTexture);
        this->buttons["SpeedPlus"]->instantTrigger = true;
    };

    //CodeButton
    auto CodeButton = [&]() {
        this->buttons["CodeSwitch"] = new Button(
            860 + 370 + 5, 450 + 300 / 2 - 60 / 2, scale_x, scale_y,
            &this->font, "", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, 40, 60);
        this->buttons["CodeSwitch"]->shape.setTexture(&this->codeLeftArrow);
        this->buttons["CodeSwitch"]->instantTrigger = true;
    };

    RunButtons();
    InstructionButtons();
    SpeedButtons();
    CodeButton();
}

void DataStructure::DataStructureThemeStandard()
{
    this->background.setFillColor(sf::Color::Black);
    this->Colors["normalFillColor"] = sf::Color::Black;
    this->Colors["normalOutlineColor"] = sf::Color::White;
    this->Colors["passedColor"] = sf::Color(255, 165, 0, 255);  // orange
    this->Colors["currentColor"] = sf::Color(255, 165, 0, 255); // orange
    this->Colors["reachColor"] = sf::Color(51, 153, 255, 255);  // skyBlue
    this->Colors["newColor"] = sf::Color(0, 102, 0, 255);
    this->Colors["emptyColor"] = sf::Color(0, 102, 0, 128);
    this->Colors["deleteColor"] = sf::Color(255, 0, 0, 255); // Red
    this->Colors["warningColor"] = sf::Color(240, 180, 10, 255); // yellow orange
    this->Colors["guideColor"] = sf::Color::White;
    this->Colors["characterColor"] = sf::Color::White;
    this->Colors["labelColor"] = sf::Color(240, 180, 10, 255); // Yellow orange;
    this->Colors["speedColor"] = sf::Color::White;
    this->Colors["stepColor"] = sf::Color::White;

    this->updateColor();
}

void DataStructure::DataStructureThemeSky()
{
    this->background.setFillColor(sf::Color::White);
    this->background.setTexture(&this->skyTexture, 1);

    // this->Colors["normalFillColor"] = sf::Color(75, 186, 186); // soft blue
    this->Colors["normalFillColor"] = sf::Color::White; 
    this->Colors["normalOutlineColor"] = sf::Color::Black;
    this->Colors["passedColor"] = sf::Color(255, 165, 0, 255);  // orange
    this->Colors["currentColor"] = sf::Color(255, 165, 0, 255); // orange
    this->Colors["reachColor"] = sf::Color(86, 230, 70);  // Green
    this->Colors["newColor"] = sf::Color(150, 70, 230, 255); // soft Violet
    this->Colors["emptyColor"] = sf::Color(150, 70, 230, 60);
    this->Colors["deleteColor"] = sf::Color(223, 213, 11 , 255); // Dark yellow
    this->Colors["warningColor"] = sf::Color::Red; 
    this->Colors["guideColor"] = sf::Color(255, 128, 0); // orange
    this->Colors["characterColor"] = sf::Color::Black;
    this->Colors["labelColor"] = sf::Color::Red; 
    this->Colors["speedColor"] = sf::Color::White;
    this->Colors["stepColor"] = sf::Color::White;

    this->updateColor();
}

void DataStructure::DataStructureThemeDesert()
{
    this->background.setFillColor(sf::Color::White);
    this->background.setTexture(&this->desertTexture, 1);

    this->Colors["normalFillColor"] = sf::Color::White;
    this->Colors["normalOutlineColor"] = sf::Color(153, 217, 234);
    this->Colors["passedColor"] = sf::Color(86, 230, 70);  // Green
    this->Colors["currentColor"] = sf::Color(86, 230, 70);  // Green
    this->Colors["reachColor"] = sf::Color(51, 153, 255, 255);  // skyBlue
    this->Colors["newColor"] = sf::Color(232, 232, 0, 255); // dark blue
    this->Colors["emptyColor"] = sf::Color(232, 232, 0, 60); // light blue
    this->Colors["deleteColor"] = sf::Color(163, 73, 164); // Violet
    this->Colors["warningColor"] = sf::Color(255, 128, 0); // orange
    this->Colors["guideColor"] = sf::Color(163, 73, 164); // Violet
    this->Colors["characterColor"] = sf::Color::Black;
    this->Colors["labelColor"] = sf::Color(163, 73, 164); // Violet
    this->Colors["speedColor"] = sf::Color::White;
    this->Colors["stepColor"] = sf::Color::White;

    this->updateColor();
}

void DataStructure::initTextures()
{
    this->skyTexture.loadFromFile("Resources/Images/skyBackground.png");
    this->desertTexture.loadFromFile("Resources/Images/desertBackground.png");
}

void DataStructure::initColor()
{
    this->Colors["normalFillColor"] = sf::Color::Black;
    this->Colors["normalOutlineColor"] = sf::Color::White;
    this->Colors["passedColor"] = sf::Color(255, 165, 0, 255);  // orange
    this->Colors["currentColor"] = sf::Color(255, 165, 0, 255); // orange
    this->Colors["reachColor"] = sf::Color(51, 153, 255, 255);  // skyBlue
    this->Colors["newColor"] = sf::Color(0, 102, 0, 255);
    this->Colors["emptyColor"] = sf::Color(0, 102, 0, 128);
    this->Colors["deleteColor"] = sf::Color(255, 0, 0, 255); // Red
    this->Colors["warningColor"] = sf::Color(240, 180, 10, 255); // yellow orange
    this->Colors["guideColor"] = sf::Color::White;
    this->Colors["characterColor"] = sf::Color::White;
    this->Colors["labelColor"] = sf::Color(240, 180, 10, 255); // Yellow orange;
    this->Colors["speedColor"] = sf::Color::White;
    this->Colors["stepColor"] = sf::Color::White;
}

DataStructure::DataStructure(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states)
    : State(window, supportedKeys, states)
{
    this->initTextures();
    this->initColor();
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initKeybinds();
    this->initButtonTextures();
    this->initButtons();
    this->initText();
    this->updateColor();
}

DataStructure::~DataStructure()
{
    // std::cout << "Ending DataStructure\n";
    for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it)
    {
        delete it->second;
    }

    for (auto it = this->childButtons.begin(); it != this->childButtons.end(); ++it)
    {
        for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
            delete it1->second;
    }

    for (auto it = this->inputFields.begin(); it != this->inputFields.end(); ++it)
    {
        for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
            delete it1->second;
    }
    
    delete codeHighlight;
}

void DataStructure::updateInput(const float &dt)
{
}

void DataStructure::updateCursor()
{
    // for Buttons
    isHandCursor = false;
    for (auto &it : this->buttons)
    {
        it.second->update(isHandCursor);
    }

    // for Child Buttons
    for (auto &it : this->childButtons[this->choosingButton])
    {
        it.second->update(isHandCursor);
    }

    /*  Show Mouse Cursor */
    sf::Cursor cursor;
    if (isHandCursor)
    {
        cursor.loadFromSystem(sf::Cursor::Hand);
    }
    else
    {
        cursor.loadFromSystem(sf::Cursor::Arrow);
    }

    this->window->setMouseCursor(cursor);
}

void DataStructure::updateTexts()
{
    speedText.setPosition(this->speedX + this->speedWidth / 2 - speedText.getGlobalBounds().width / 2,
                          this->speedY - speedText.getGlobalBounds().height / 2);
}

void DataStructure::updateColor()
{
    inputWarning.setFillColor(Colors.at("warningColor"));
    inputGuide.setFillColor(Colors.at("guideColor"));
    stepText.setFillColor(Colors.at("stepColor"));
    speedText.setFillColor(Colors.at("speedColor"));
}

void DataStructure::renderTexts(sf::RenderTarget *target)
{
    target->draw(inputWarning);
    target->draw(inputGuide);
    target->draw(speedText);
    target->draw(stepText);
}

void DataStructure::renderButtons(sf::RenderTarget *target)
{
    /* Render all buttons */
    for (auto &it : this->buttons)
    {
        it.second->render(target);
    }
}


// General button functions

/* Run Buttons */
void DataStructure::button_first()
{   
    this->newStepTriggered = 0;
    this->animation->finishStep();

    this->buttons["Pause"]->disabled = 1;
    this->buttons["Replay"]->disabled = 1;
    this->buttons["Play"]->disabled = 0;
    
    this->doneAnimation = 0;
    this->runState = "first";
    this->choosingButton = "";
}

void DataStructure::button_prev()
{
    this->newStepTriggered = 0;
    this->animation->finishStep();

    this->buttons["Pause"]->disabled = 1;
    this->buttons["Replay"]->disabled = 1;
    this->buttons["Play"]->disabled = 0;

    this->doneAnimation = 0;
    this->runState = "prev";
    this->choosingButton = "";
}

void DataStructure::button_pause()
{   
    this->newStepTriggered = 0;
    this->animation->finishStep();

    this->buttons["Pause"]->disabled = 1;
    this->buttons["Replay"]->disabled = 1;
    this->buttons["Play"]->disabled = 0;

    this->runState = "pause";
    this->choosingButton = "";
}

void DataStructure::button_play()
{   
    this->newStepTriggered = 0;
    this->animation->finishStep();
    this->doneAnimation = 0;

    this->buttons["Play"]->disabled = 1;
    this->buttons["Replay"]->disabled = 1;
    this->buttons["Pause"]->disabled = 0;

    this->runState = "play";
    this->choosingButton = "";
}

void DataStructure::button_replay()
{   
    this->newStepTriggered = 0;
    this->animation->finishStep();

    /* Reverse all animations first */
    bool trash = 0;
    this->animation->first(trash, &this->stepText);
    this->animation->finishStep();

    this->doneAnimation = 0;

    this->animation->curIndex = 0;
    this->animation->reverse = 0;

    /* The same as Play */
    this->buttons["Play"]->disabled = 1;
    this->buttons["Replay"]->disabled = 1;
    this->buttons["Pause"]->disabled = 0;

    this->runState = "play";
    this->choosingButton = "";
}

void DataStructure::button_next()
{   
    this->newStepTriggered = 0;
    this->buttons["Pause"]->disabled = 1;
    this->buttons["Replay"]->disabled = 1;
    this->buttons["Play"]->disabled = 0;

    this->animation->finishStep();
    this->doneAnimation = 0;
    this->runState = "next";
    this->choosingButton = "";
}

void DataStructure::button_last()
{
    this->newStepTriggered = 0;
    this->buttons["Pause"]->disabled = 1;
    this->buttons["Replay"]->disabled = 1;
    this->buttons["Play"]->disabled = 0;

    this->animation->finishStep();
    this->doneAnimation = 0;
    this->runState = "last";
    this->choosingButton = "";
}

void DataStructure::button_SpeedMinus()
{
    this->newStepTriggered = 0;
    if (this->currentSpeed == 1) return;
    --this->currentSpeed;
    this->speedText.setString(std::to_string(this->currentSpeed) + "x");
    this->animation->animationTime = (float)(maxSpeed * speedGap) / currentSpeed;
    this->choosingButton = "";
    return;
}

void DataStructure::button_SpeedPlus()
{
    this->newStepTriggered = 0;
    if (this->currentSpeed == maxSpeed) return;
    ++this->currentSpeed;
    this->speedText.setString(std::to_string(this->currentSpeed) + "x");
    this->animation->animationTime = (float)(maxSpeed * speedGap) / currentSpeed;
    this->choosingButton = "";
    return;
}

// Code switch
void DataStructure::button_CodeSwitch()
{   
    this->newStepTriggered = 0;
    this->codeHighlight->switchPanel();
    if (this->codeHighlight->isHiding) {
        this->buttons["CodeSwitch"]->shape.setTexture(&this->codeLeftArrow);
    }
    else {
        this->buttons["CodeSwitch"]->shape.setTexture(&this->codeRightArrow);
    }
    this->choosingButton = "";
    return;
}