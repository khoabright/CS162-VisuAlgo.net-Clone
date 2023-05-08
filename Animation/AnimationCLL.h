#ifndef AnimationCLL_H
#define AnimationCLL_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "CLLNode.h"
#include "CodeHighlight.h"
#include "Animation.h"

class AnimationCLL : public Animation
{
private:
public:
    AnimationCLL(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight);
    ~AnimationCLL();

    // Initialize

    // Animation

    void addCodeLines(std::vector<int> codeLines); //do nothing but adding codeLines

    void changeHead(CLLNode *&head, CLLNode *old_head, CLLNode *targetNode, std::vector<int> codeLines); /* current lines of code for highlighting */
    void changeTail(CLLNode *&tail, CLLNode *old_tail, CLLNode *targetNode, std::vector<int> codeLines);

    void showNode(CLLNode *targetNode, std::string targetLabel, int &numberNode, std::vector<int> codeLines);
    void hideNode(CLLNode *targetNode, int &numberNode, std::vector<int> codeLines);
    
    void changeNodeLabel(CLLNode *targetNode, std::string targetLabel, std::vector<int> codeLines);
    void changeNodeValue(CLLNode *targetNode, int newValue, std::vector<int> codeLines);

    void connectNodes(CLLNode *targetNode, CLLNode *nextNode, std::vector<int> codeLines);

    /* Use color as string because we want to modified later */
    void highlightCurrentNode(CLLNode *targetNode, std::string targetLabel, std::string fillColor, std::string outlineColor, std::vector<int> codeLines);
    void highlightPassedNode(CLLNode *targetNode, std::string passedLabel, std::string currentLabel, std::string passedColor, std::string currentColor, std::vector<int> codeLines);

    void moveNode(CLLNode *targetNode, float x1, float y1);
    void makeArrow(sf::CircleShape *node1, sf::CircleShape *node2, sf::RectangleShape *targetArrow);

    void Relayout(bool emptyList, CLLNode *head, int numberNode, float start_x, float start_y, float distance_x, float distance_y, std::vector<int> codeLines);

    //Reset
    void updateNodePosition(CLLNode *head, int numberNode, float start_x, float start_y, float distance_x, float distance_y);
    void newInstruction(CLLNode *head, int numberNode, float start_x, float start_y, float distance_x, float distance_y);
};

#endif