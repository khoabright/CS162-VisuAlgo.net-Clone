#ifndef GAME_H
#define GAME_H

#pragma once

#include "MainMenuState.h"

class Game
{
private:
    // Variables
    sf::RenderWindow *window;
    sf::Event sfEvent;
    std::vector<sf::VideoMode> videoModes;
    sf::ContextSettings windowSettings;
    bool fullscreen;

    sf::Clock dtClock;
    float dt;

    std::stack<State*> states;

    std::map<std::string, int> supportedKeys;

    // Initilization
    void initVariables();
    void initWindow();
    void initKeys();
    void initStates();

public:
    // Constructors/Destructors
    Game();
    virtual ~Game();

    sf::Texture backgroundTexture;
    sf::Texture skyTexture;
    sf::Texture desertTexture;
    
    // Functions:

    //Regular
    void endApplication();

    //Update
    void updateDt();
    void updateSFMLEvents();
    void update();

    //Render
    void render();

    //Core
    void run();
};

#endif