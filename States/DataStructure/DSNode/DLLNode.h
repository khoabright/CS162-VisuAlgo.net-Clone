#ifndef DLLNODE_H
#define DLLNODE_H

#pragma once

#include "SFML/Graphics.hpp"

#include <bits/stdc++.h>

struct DLLNode
{
    DLLNode *next, *prev;

    int val;
    int index;
    float scale_x, scale_y, x, y, width, height;
    float standard_width = 40.f, standard_height = 40.f; /* Dimension I used in 800x600 and radius = 40 */
    float x_center, y_center;
    float radius = 20.f;

    std::map<std::string, sf::Color> *Colors;
    std::vector<std::string> array_outlineColor;
    std::vector<std::string> array_fillColor;
    std::vector<std::string> array_label;
    std::vector<DLLNode*> array_next;
    std::vector<DLLNode*> array_prev;
    std::vector<int> array_val;
    std::vector<sf::Vector2f> array_pos;

    int idx_outlineColor = -1;
    int idx_fillColor = -1;
    int idx_label = -1;
    int idx_next = -1;
    int idx_prev = -1;
    int idx_val = -1;
    int idx_pos = -1;

    sf::CircleShape shape;
    sf::RectangleShape arrowNext, arrowPrev;
    sf::Texture arrowNext_img, arrowPrev_img;

    sf::Text text;
    sf::Font *font;

    //Label
    std::string labelString;
    sf::Text labelText;
    sf::Color labelColor; // Yellow orange

    bool showNode = 0;
    bool showArrowNext = 0;
    bool showArrowPrev = 0;
    bool showLabel = 1;
    bool isNewNode = 0;

    DLLNode();
    DLLNode(float x, float y, float scale_x, float scale_y, int _val, sf::Font *_font, std::map<std::string, sf::Color>* Colors);

    void reset();

    //Next
    void updateNext();
    void newNext(DLLNode* new_next);
    void prevNext();
    void nextNext();

    //Prev
    void updatePrev();
    void newPrev(DLLNode* new_prev);
    void prevPrev();
    void nextPrev();

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

    //Pos
    void updatePos();
    void newPos(sf::Vector2f new_pos);
    void prevPos();
    void nextPos();

    //Render
    void renderAnimation();
    void render(sf::RenderTarget *target);
};

#endif