#ifndef AnimationSA_H
#define AnimationSA_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "SANode.h"
#include "CodeHighlight.h"
#include "Animation.h"

class AnimationSA : public Animation
{
private:
public:
    AnimationSA(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight);
    ~AnimationSA();

    // Initialize

    // Animation

    void addCodeLines(std::vector<int> codeLines); //do nothing but adding codeLines
    
    void changeNodeLabel(SANode *targetNode, std::string targetLabel, std::vector<int> codeLines);
    void changeNodeValue(SANode *targetNode, int newValue, std::vector<int> codeLines);

    /* Use color as string because we want to modified later */
    void highlightCurrentNode(SANode *targetNode, std::string targetLabel, std::string fillColor, std::string outlineColor, std::vector<int> codeLines);

    //Reset
    void updateNodePosition(std::vector<SANode *> arr, int arr_size, float start_x, float start_y, float distance);
    void newInstruction(std::vector<SANode *> arr, int arr_size, float start_x, float start_y, float distance);
};

#endif