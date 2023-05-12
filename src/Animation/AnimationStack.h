#ifndef AnimationStack_H
#define AnimationStack_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "StackNode.h"
#include "CodeHighlight.h"
#include "Animation.h"

class AnimationStack : public Animation
{
private:
public:
    AnimationStack(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight);
    ~AnimationStack();

    // Initialize

    // Animation

    void addCodeLines(std::vector<int> codeLines); //do nothing but adding codeLines

    void changeHead(StackNode *&head, StackNode *old_head, StackNode *targetNode, std::vector<int> codeLines); /* current lines of code for highlighting */
    void changeTail(StackNode *&tail, StackNode *old_tail, StackNode *targetNode, std::vector<int> codeLines);

    void showNode(StackNode *targetNode, std::string targetLabel, int &numberNode, std::vector<int> codeLines);
    void hideNode(StackNode *targetNode, int &numberNode, std::vector<int> codeLines);
    
    void changeNodeLabel(StackNode *targetNode, std::string targetLabel, std::vector<int> codeLines);
    void changeNodeValue(StackNode *targetNode, int newValue, std::vector<int> codeLines);

    void connectNodes(StackNode *targetNode, StackNode *nextNode, std::vector<int> codeLines);

    /* Use color as string because we want to modified later */
    void highlightCurrentNode(StackNode *targetNode, std::string targetLabel, std::string targetColor, std::vector<int> codeLines);
    void highlightPassedNode(StackNode *targetNode, std::string passedLabel, std::string currentLabel, std::string passedColor, std::string currentColor, std::vector<int> codeLines);

    void moveNode(StackNode *targetNode, float x1, float y1);
    void makeArrow(sf::CircleShape *node1, sf::CircleShape *node2, sf::RectangleShape *targetArrow);

    void Relayout(bool emptyList, StackNode *head, StackNode *tail, float start_x, float start_y, float distance, std::vector<int> codeLines);

    //Reset
    void updateNodePosition(StackNode *head, StackNode *tail, float start_x, float start_y, float distance);
    void newInstruction(StackNode *head, StackNode *tail, float start_x, float start_y, float distance);
};

#endif