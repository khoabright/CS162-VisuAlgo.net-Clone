#ifndef AnimationDA_H
#define AnimationDA_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "DANode.h"
#include "CodeHighlight.h"
#include "Animation.h"

class AnimationDA : public Animation
{
private:
public:
    AnimationDA(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight);
    ~AnimationDA();

    // Initialize

    // Animation

    void addCodeLines(std::vector<int> codeLines); //do nothing but adding codeLines
    
    void changeNodeLabel(DANode *targetNode, std::string targetLabel, std::vector<int> codeLines);
    void changeNodeValue(DANode *targetNode, int newValue, std::vector<int> codeLines);

    /* Use color as string because we want to modified later */
    void highlightCurrentNode(DANode *targetNode, std::string targetLabel, std::string fillColor, std::string outlineColor, std::vector<int> codeLines);
    void moveNode(DANode *targetNode, float x1, float y1);
    void moveArray(std::vector<DANode *> arr, int arr_size, float start_x, float start_y, float distanceX, std::vector<int> codeLines);
    void hideThings(bool &show_arr, bool &show_new_label); /* hide these things */
    void showThings(bool &showArray, bool &showLabel, std::vector<int> codeLines);

    //Reset
    void updateNodePosition(std::vector<DANode *> arr, int arr_size, float start_x, float start_y, float distance);
    void newInstruction(std::vector<DANode *> arr, int arr_size, float start_x, float start_y, float distance);
};

#endif