#ifndef BUTTON_H
#define BUTTON_H

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <bits/stdc++.h>

enum button_states
{
    BTN_IDLE = 0,
    BTN_HOVER,
    BTN_ACTIVE
};

class Button
{
private:
    short unsigned buttonState;

    sf::Color textIdleColor;
    sf::Color textHoverColor;
    sf::Color textActiveColor;

    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;

public:
    sf::RectangleShape shape;
    sf::Texture texture;
    sf::Font *font;
    sf::Text text;
    bool disabled = false;
    bool instantTrigger = false;
    
    Button(float x, float y, float scale_x, float scale_y,
           sf::Font *font, std::string text, int textSize,
           sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
           sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
           bool visible_background = 0, float width = 0, float height = 0);
    ~Button();

    // Accessors
    bool isPressed();

    // Functions
    void updateText();
    void updateSFMLEvents(const float &dt, sf::Event& event, const sf::Vector2f mousePos);
    void update(bool &isHandCursor);

    void render(sf::RenderTarget *target);
};

#endif