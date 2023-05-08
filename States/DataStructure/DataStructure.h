#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#pragma once

#include "State.h"
#include "Button.h"
#include "InputField.h"
#include "AnimationSLL.h"
#include "AnimationDLL.h"
#include "AnimationCLL.h"
#include "AnimationSA.h"
#include "AnimationDA.h"
#include "AnimationStack.h"
#include "AnimationQueue.h"

class DataStructure : public State
{

private:

protected:
    //Variables
    sf::RectangleShape background;
    sf::Texture backgroundTexture;
    sf::Texture skyTexture;
    sf::Texture desertTexture;

    sf::Font font;

    //Colors

    //Text
    sf::Text inputWarning;
    sf::Text inputGuide;
    sf::Text speedText;
    sf::Text stepText;

    float stepX = 552;
    float stepY = 30;


    int currentSpeed = 1;
    float speedGap = 0.25;
    int maxSpeed = 8;

    float speedX = 600.f;
    float speedY = 645.f;
    float speedWidth = 20.f;
    float speedHeight = 20.f;
    int characterSize = 20;

    // Buttons
    float runButtonDistance = 50.f;
    float runButtonX = 492.f;
    float runButtonY = 700.f;
    float runButtonSizeX = 32.f;
    float runButtonSizeY = 32.f;

    float insButtonDistanceY = 30;
    float insButtonX = 50.f;
    float insButtonY = 540.f;
    float insButtonSizeX = 70.f;
    float insButtonSizeY = 30.f;

    sf::Texture nextButtonTexture;
    sf::Texture prevButtonTexture;
    sf::Texture lastButtonTexture;
    sf::Texture firstButtonTexture;
    sf::Texture pauseButtonTexture;
    sf::Texture playButtonTexture;
    sf::Texture replayButtonTexture;
    sf::Texture speedMinusTexture;
    sf::Texture speedPlusTexture;
    sf::Texture codeLeftArrow;
    sf::Texture codeRightArrow;

    std::map<std::string, Button*> buttons;
    std::map<std::string, std::map<std::string, Button*>> childButtons; 
    std::map<std::string, std::map<std::string, InputField*>> inputFields; 
    std::map<std::string, std::function<void()>> triggerFunction;

    // DS variables
    std::vector<bool> exist;
    Animation *animation;

    //Highlight code
    CodeHighlight *codeHighlight;

    bool isHandCursor = false;
    std::string choosingButton = "";
    std::string choosingChildButton = "";

    int valueFirst = 0, valueSecond = 0;  /* value seperated by comma in inputField */
    bool newStepTriggered = 0;
    bool doneAnimation = 1;
    std::string runState = "";

    // Functions
    
    void initColor();
    void initVariables();
    void initBackground();
    void initButtonTextures();
    void initFonts();
    void initKeybinds();
    void initText();
    void initButtons();
    
    //Color Theme
    void DataStructureThemeStandard();
    void DataStructureThemeSky();
    void DataStructureThemeDesert();
    std::map<std::string, sf::Color> Colors;

    void initTextures();
    virtual void initTriggerFunction() = 0;
    virtual void initChildButtons() = 0;
    virtual void initInputFields() = 0;
    virtual void initAnimation() = 0;

public:
    DataStructure(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State*>* states);
    virtual ~DataStructure();

    // Functions

    //Update
    void updateInput(const float &dt);
    void updateCursor();

    void updateTexts();
    void updateColor();

    virtual void updateSFMLEvents(const float &dt, sf::Event &event) = 0;
    virtual void update(const float &dt) = 0;
    
    //Render
    void renderTexts(sf::RenderTarget* target = NULL);
    void renderButtons(sf::RenderTarget* target = NULL);
    virtual void render(sf::RenderTarget *target = NULL) = 0;

    //General button functions
    //Run Buttons
    void button_first();
    void button_prev();
    void button_pause();
    void button_play();
    void button_replay();
    void button_next();
    void button_last();

    //Speed adjust
    void button_SpeedMinus();
    void button_SpeedPlus();

    //Code switch
    void button_CodeSwitch();
};

#endif