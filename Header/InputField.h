#ifndef INPUTFIELD_H
#define INPUTFIELD_H

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#pragma once

class InputField
{
private:
    // General variables
    sf::RenderWindow *window;
    float scale_x, scale_y;
    float standard_x = 100.f, standard_y = 50.f;
    float standard_characterSize_ratio = 40.f / 50.f;

    // Shape variables
    sf::RectangleShape shape;

    // Font variables
    sf::Font *font;

    // Input text variables
    std::string inputString;
    sf::Text inputText;
    sf::Clock typeClock;

    // Message variables
    sf::Text message; // false input alert

    // Cursor
    sf::RectangleShape cursor;
    sf::Clock blinkClock;
    bool showCursor = true;

public:
    InputField(float x, float y, float width, float height, float scale_x, float scale_y,
               sf::Font *font, int textSize,
               sf::Color textColor, sf::Color shapeOutlineColor, sf::Color shapeFillColor);
    ~InputField();

    bool disabled = false;

    void handleTextEntered(sf::Uint32 unicode);

    void updateSFMLEvents(const float &dt, sf::Event& event, std::string &return_value , bool& hitEnter);
    void update(const sf::Vector2f mousePos);
    void render(sf::RenderTarget *target);
};

#endif