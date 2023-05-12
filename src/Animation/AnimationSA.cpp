#include "AnimationSA.h"

AnimationSA::AnimationSA(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight)
    : Animation(Colors, animationTime, codeHighlight)
{
    this->startStep.resize(5, 1);
    this->Colors = Colors;
    this->animationTime = animationTime;
    this->codeHighlight = codeHighlight;
}

AnimationSA::~AnimationSA()
{
}

// Initialize

// AnimationSA

void AnimationSA::addCodeLines(std::vector<int> codeLines)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;
    if (this->reverse) codeHighlight->prev_currentLines();
    else codeHighlight->next_currentLines(codeLines);
}

void AnimationSA::changeNodeLabel(SANode *targetNode, std::string targetLabel, std::vector<int> codeLines)
{   
    if (!this->startStep[curChildIndex]) return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse) {
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();
        return;
    }
    targetNode->newLabel(targetLabel);
    targetNode->nextLabel();
    codeHighlight->next_currentLines(codeLines);
}

void AnimationSA::changeNodeValue(SANode *targetNode, int newValue, std::vector<int> codeLines)
{   
    if (!this->startStep[curChildIndex]) return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse) {
        targetNode->prevVal();
        codeHighlight->prev_currentLines();
        return;
    }
    targetNode->newVal(newValue);
    targetNode->nextVal();
    codeHighlight->next_currentLines(codeLines);
}

void AnimationSA::highlightCurrentNode(SANode *targetNode, std::string targetLabel, std::string fillColor, std::string outlineColor, std::vector<int> codeLines)
{   
    if (!this->startStep[curChildIndex]) return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse) {
        targetNode->prevColor();
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();
        return;
    }
    codeHighlight->next_currentLines(codeLines);

    targetNode->newColor(fillColor, outlineColor);
    targetNode->newLabel(targetLabel);

    targetNode->nextColor();
    targetNode->nextLabel();
}

//Reset
void AnimationSA::updateNodePosition(std::vector<SANode *> arr, int arr_size, float start_x, float start_y, float distance)
{  
    for (int i = 0; i < arr.size(); ++i) {
        float x = start_x + i * distance;
        float y = start_y;
        arr[i]->shape.setPosition(x, y);
        arr[i]->x = x;
        arr[i]->y = y;
        arr[i]->labelString = std::to_string(i);
        if (i < arr_size) {
            arr[i]->newColor("normalFillColor", "newColor");
        }
        else {
            arr[i]->newColor("normalFillColor", "emptyColor");
        }
        arr[i]->nextColor();
        arr[i]->reset();
    } 
}

void AnimationSA::newInstruction(std::vector<SANode *> arr, int arr_size, float start_x, float start_y, float distance)
{   
    /* Already completed all animations */
    this->updateNodePosition(arr, arr_size, start_x, start_y, distance);

    this->resetStartStep();
    this->curIndex = 0;

    for (auto &ins : instructions) ins.clear();
    this->instructions.clear();

    this->animationClock.restart();
    this->codeHighlight->reset();
}
