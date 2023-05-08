#ifndef AnimationSLL_H
#define AnimationSLL_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "SLLNode.h"
#include "CodeHighlight.h"
#include "Animation.h"

class AnimationSLL : public Animation
{
private:
public:
    AnimationSLL(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight);
    ~AnimationSLL();

    // Initialize

    // Animation

    void addCodeLines(std::vector<int> codeLines); //do nothing but adding codeLines

    void changeHead(SLLNode *&head, SLLNode *old_head, SLLNode *targetNode, std::vector<int> codeLines); /* current lines of code for highlighting */
    void changeTail(SLLNode *&tail, SLLNode *old_tail, SLLNode *targetNode, std::vector<int> codeLines);

    void showNode(SLLNode *targetNode, std::string targetLabel, int &numberNode, std::vector<int> codeLines);
    void hideNode(SLLNode *targetNode, int &numberNode, std::vector<int> codeLines);
    
    void changeNodeLabel(SLLNode *targetNode, std::string targetLabel, std::vector<int> codeLines);
    void changeNodeValue(SLLNode *targetNode, int newValue, std::vector<int> codeLines);

    void connectNodes(SLLNode *targetNode, SLLNode *nextNode, std::vector<int> codeLines);

    /* Use color as string because we want to modified later */
    void highlightCurrentNode(SLLNode *targetNode, std::string targetLabel, std::string targetColor, std::vector<int> codeLines);
    void highlightPassedNode(SLLNode *targetNode, std::string passedLabel, std::string currentLabel, std::string passedColor, std::string currentColor, std::vector<int> codeLines);

    void moveNode(SLLNode *targetNode, float x1, float y1);
    void makeArrow(sf::CircleShape *node1, sf::CircleShape *node2, sf::RectangleShape *targetArrow);

    void Relayout(bool emptyList, SLLNode *head, SLLNode *tail, float start_x, float start_y, float distance, std::vector<int> codeLines);

    //Reset
    void updateNodePosition(SLLNode *head, SLLNode *tail, float start_x, float start_y, float distance);
    void newInstruction(SLLNode *head, SLLNode *tail, float start_x, float start_y, float distance);
};

#endif