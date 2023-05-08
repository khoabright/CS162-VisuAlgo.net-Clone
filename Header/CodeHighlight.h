#ifndef CODEHIGHLIGHT_H
#define CODEHIGHLIGHT_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

class CodeHighlight
{
private:
    sf::Font font;

    float scale_x;
    float scale_y;

    sf::RectangleShape background;
    sf::RectangleShape introBackground;

    float background_x = 860;
    float background_y = 450;
    float background_width = 370;
    float background_height = 300;

    float lineDistance = 30;
    float text_start_x = 870;
    float text_start_y = 495;
    int characterSize = 17;

    sf::Color characterIntroColor = sf::Color(240, 180, 10, 255); // yellow orange
    sf::Color characterColor = sf::Color::White;
    sf::Color highlightColor = sf::Color::Black;
    sf::Color backgroundColor = sf::Color(217, 81, 60); // Red orange
    sf::Color introBackgroundColor = sf::Color(75, 101, 186); // Blue

    std::map<std::string, sf::Color> *Colors;


public:
    std::vector<int> currentLines; // current lines to highlight
    std::vector<std::string> codeStrings;
    std::vector<sf::Text> codes;
    std::vector<sf::RectangleShape> highlightBlock;
    sf::Text introText;
    bool isHiding = 1;


    CodeHighlight(float scale_x, float scale_y, std::map<std::string, sf::Color> *Colors);
    ~CodeHighlight();

    void reset();
    void switchPanel();

    void updateTexts();
    void formatText(sf::Text *text, int idx);

    void resetHighlight();

    void updateHighlight();
    void update();

    void render(sf::RenderTarget *target);
};

#endif