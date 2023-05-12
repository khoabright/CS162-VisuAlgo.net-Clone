#ifndef AnimationDLL_H
#define AnimationDLL_H

#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "DLLNode.h"
#include "CodeHighlight.h"
#include "Animation.h"

class AnimationDLL : public Animation
{
private:
public:
    AnimationDLL(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight); 
    ~AnimationDLL();

    // Initialize

    // AnimationDLL

    void addCodeLines(std::vector<int> codeLines); // do nothing but adding codeLines

    void changeHead(DLLNode *&head, DLLNode *old_head, DLLNode *targetNode, std::vector<int> codeLines); /* current lines of code for highlighting */
    void changeTail(DLLNode *&tail, DLLNode *old_tail, DLLNode *targetNode, std::vector<int> codeLines);

    void showNode(DLLNode *targetNode, std::string targetLabel, int &numberNode, std::vector<int> codeLines);
    void hideNode(DLLNode *targetNode, int &numberNode, std::vector<int> codeLines);

    void changeNodeLabel(DLLNode *targetNode, std::string targetLabel, std::vector<int> codeLines);
    void changeNodeValue(DLLNode *targetNode, int newValue, std::vector<int> codeLines);

    void connectNext(DLLNode *targetNode, DLLNode *nextNode, std::vector<int> codeLines); 
    void connectPrev(DLLNode *targetNode, DLLNode *prevNode, std::vector<int> codeLines); 

    /* Use color as string because we want to modified later */
    void highlightCurrentNode(DLLNode *targetNode, std::string targetLabel, std::string targetColor, std::vector<int> codeLines);
    void highlightPassedNode(DLLNode *targetNode, std::string passedLabel, std::string currentLabel, std::string passedColor, std::string currentColor, std::vector<int> codeLines);

    void moveNode(DLLNode *targetNode, float x1, float y1);
    void makeArrow(sf::CircleShape *node1, sf::CircleShape *node2, sf::RectangleShape *targetArrow);

    void Relayout(bool emptyList, DLLNode *head, DLLNode *tail, float start_x, float start_y, float distance, std::vector<int> codeLines);

    // Reset
    void updateNodePosition(DLLNode *head, DLLNode *tail, float start_x, float start_y, float distance);
    void newInstruction(DLLNode *head, DLLNode *tail, float start_x, float start_y, float distance);
};

#endif