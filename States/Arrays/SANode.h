#ifndef SANODE_H
#define SANODE_H

#pragma once

#include "SFML/Graphics.hpp"

#include <bits/stdc++.h>

struct SANode
{
    int val = 0;
    int index;
    float scale_x, scale_y, x, y, width, height;
    float standard_width = 40.f, standard_height = 40.f; 
    float side = 50.f;

    std::map<std::string, sf::Color> *Colors;
    std::vector<std::string> array_outlineColor;
    std::vector<std::string> array_fillColor;
    std::vector<std::string> array_label;
    std::vector<int> array_val;

    int idx_outlineColor = -1;
    int idx_fillColor = -1;
    int idx_label = -1;
    int idx_val = -1;

    sf::RectangleShape shape;

    sf::Text text;
    sf::Font *font;

    //Label
    std::string labelString;
    sf::Text labelText;
    sf::Color labelColor; // Yellow orange

    bool showNode = 1;
    bool showLabel = 1;

    SANode();
    SANode(float x, float y, float scale_x, float scale_y, int _val, std::string newLabelString, sf::Font *_font, std::map<std::string, sf::Color>* Colors);

    void reset();

    //Val
    void updateVal();
    void newVal(int new_val);
    void prevVal();
    void nextVal();

    //Color
    void setNormalColor();
    void newColor(std::string fillColor, std::string outlineColor);
    void updateColor();
    void prevColor();
    void nextColor();

    //Text & Label
    void updateText();
    void newLabel(std::string new_label);
    void updateLabel();
    void prevLabel();
    void nextLabel();

    //Render
    void render(sf::RenderTarget *target);
};

#endif