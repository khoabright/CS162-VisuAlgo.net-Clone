#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#pragma once
#include "State.h"
#include "SLL.h"
#include "DLL.h"
#include "CLL.h"
#include "SA.h"
#include "DA.h"
#include "Stack.h"
#include "Queue.h"
#include "Button.h"

class MainMenuState : public State
{
private:
    //Variables
    sf::Texture backgroundTexture;
    sf::RectangleShape background;
    sf::Font font;

    std::map<std::string, Button*> buttons;
    bool isHandCursor = false;

    float start_x = 300;
    float start_y = 300;
    float distance_x = 400;
    float distance_y = 120;
    int characterSize = 35;

    // Functions
    void initVariables();
    void initBackground();
    void initFonts();
    void initKeybinds();
    void initButtons();

public:
    MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State*>* states);
    virtual ~MainMenuState();

    // Functions
    void updateInput(const float &dt);
    void updateCursor();
    void updateButtons();
    void updateSFMLEvents(const float &dt, sf::Event& event);
    void update(const float &dt);
    void renderButtons(sf::RenderTarget* target = NULL);
    void render(sf::RenderTarget *target = NULL);
};

#endif