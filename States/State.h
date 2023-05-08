#ifndef STATE_H
#define STATE_H

#pragma once

#include "Entity.h"

class State
{

private:

protected:
    std::mt19937_64 randomize;

    std::stack<State*>* states;

    sf::RenderWindow *window;
    float scale_x, scale_y, scale_xy;

    std::map<std::string, int>* supportedKeys;
    std::map<std::string, int> keybinds;
    bool quit;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //Resources
    std::vector<sf::Texture> textures;

    //Functions
    virtual void initKeybinds() = 0;

public:
    State(sf::RenderWindow *window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
    virtual ~State();

    const bool &getQuit() const;

    virtual void endState();
    virtual void updateMousePositions();
    virtual void updateInput(const float &dt) = 0;
    virtual void updateSFMLEvents(const float &dt, sf::Event& event) = 0;
    virtual void update(const float &dt) = 0;
    virtual void render(sf::RenderTarget *target = NULL) = 0;
};

#endif