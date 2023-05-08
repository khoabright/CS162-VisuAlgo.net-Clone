#ifndef ANIMATION_H
#define ANIMATION_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "CodeHighlight.h"

class Animation
{
private:
public:
    float animationTime = 1;
    bool doneStep = 1;
    std::vector<bool> startStep; /*If the function is first time triggered; Hardly have more than 5 animations at the same time */

    float eps = 1; /* approximately 1 pixel */
    sf::Clock animationClock;
    std::vector<std::vector<std::function<void()>>> instructions;
    std::map<std::string, sf::Color> *Colors;
    CodeHighlight *codeHighlight;

    int curIndex = 0; /* current processing animation index */
    int curChildIndex = 0;
    bool reverse = 0;
    // std::string curInstruction = "";
    float dt;

    Animation(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight);
    virtual ~Animation();

    // Running
    void pause(bool &doneAnimation, sf::Text *step);
    void first(bool &doneAnimation, sf::Text *step);
    void prev(bool &doneAnimation, sf::Text *step);
    void next(bool &doneAnimation, sf::Text *step);
    void last(bool &doneAnimation, sf::Text *step);
    void play(bool &doneAnimation, sf::Text *step);
    void replay(bool &doneAnimation, sf::Text *step);

    void resetStartStep();
    void triggerOneIns(int index); // trigger functions of the same step
    void finishStep();
    void activateStep();
};

#endif