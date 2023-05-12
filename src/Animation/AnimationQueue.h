#ifndef AnimationQueue_H
#define AnimationQueue_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "QueueNode.h"
#include "CodeHighlight.h"
#include "Animation.h"

class AnimationQueue : public Animation
{
private:
public:
    AnimationQueue(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight);
    ~AnimationQueue();

    // Initialize

    // Animation

    void addCodeLines(std::vector<int> codeLines); //do nothing but adding codeLines

    void changeHead(QueueNode *&head, QueueNode *old_head, QueueNode *targetNode, std::vector<int> codeLines); /* current lines of code for highlighting */
    void changeTail(QueueNode *&tail, QueueNode *old_tail, QueueNode *targetNode, std::vector<int> codeLines);

    void showNode(QueueNode *targetNode, std::string targetLabel, int &numberNode, std::vector<int> codeLines);
    void hideNode(QueueNode *targetNode, int &numberNode, std::vector<int> codeLines);
    
    void changeNodeLabel(QueueNode *targetNode, std::string targetLabel, std::vector<int> codeLines);
    void changeNodeValue(QueueNode *targetNode, int newValue, std::vector<int> codeLines);

    void connectNodes(QueueNode *targetNode, QueueNode *nextNode, std::vector<int> codeLines);

    /* Use color as string because we want to modified later */
    void highlightCurrentNode(QueueNode *targetNode, std::string targetLabel, std::string targetColor, std::vector<int> codeLines);
    void highlightPassedNode(QueueNode *targetNode, std::string passedLabel, std::string currentLabel, std::string passedColor, std::string currentColor, std::vector<int> codeLines);

    void moveNode(QueueNode *targetNode, float x1, float y1);
    void makeArrow(sf::CircleShape *node1, sf::CircleShape *node2, sf::RectangleShape *targetArrow);

    void Relayout(bool emptyList, QueueNode *head, QueueNode *tail, float start_x, float start_y, float distance, std::vector<int> codeLines);

    //Reset
    void updateNodePosition(QueueNode *head, QueueNode *tail, float start_x, float start_y, float distance);
    void newInstruction(QueueNode *head, QueueNode *tail, float start_x, float start_y, float distance);
};

#endif