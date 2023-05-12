#include "AnimationDA.h"

AnimationDA::AnimationDA(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight)
    : Animation(Colors, animationTime, codeHighlight)
{
    this->startStep.resize(5, 1);
    this->Colors = Colors;
    this->animationTime = animationTime;
    this->codeHighlight = codeHighlight;
}

AnimationDA::~AnimationDA()
{
}

// Initialize

// AnimationDA

void AnimationDA::addCodeLines(std::vector<int> codeLines)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;
    if (this->reverse) codeHighlight->prev_currentLines();
    else codeHighlight->next_currentLines(codeLines);
}

void AnimationDA::changeNodeLabel(DANode *targetNode, std::string targetLabel, std::vector<int> codeLines)
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

void AnimationDA::changeNodeValue(DANode *targetNode, int newValue, std::vector<int> codeLines)
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

void AnimationDA::highlightCurrentNode(DANode *targetNode, std::string targetLabel, std::string fillColor, std::string outlineColor, std::vector<int> codeLines)
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

void AnimationDA::moveNode(DANode *targetNode, float x1, float y1)
{
    if (this->reverse && this->doneStep)
    {   
        targetNode->prevPos();
        return;
    }

    if (!this->reverse && this->doneStep) {
        targetNode->newPos(sf::Vector2f(x1, y1));
        targetNode->nextPos();
        return;
    }

    if (this->startStep[curChildIndex]) this->startStep[curChildIndex] = 0;

    float x0 = targetNode->x;
    float y0 = targetNode->y;
    
    if (reverse) {
        x0 = targetNode->array_pos[targetNode->idx_pos - 1].x;
        y0 = targetNode->array_pos[targetNode->idx_pos - 1].y;
    }
    float sx = x1 - x0;
    float sy = y1 - y0;

    float x = x0 + sx / this->animationTime * this->dt;
    float y = y0 + sy / this->animationTime * this->dt;

    targetNode->shape.setPosition(x, y);
}

void AnimationDA::moveArray(std::vector<DANode *> arr, int arr_size, float start_x, float start_y, float distanceX, std::vector<int> codeLines)
{
    if (this->reverse && this->doneStep) {
        codeHighlight->prev_currentLines();
    }
    if (!this->reverse && this->startStep[curChildIndex]) {
        codeHighlight->next_currentLines(codeLines);
    }

    for (int i = 0; i < arr.size(); ++i) {
        float x = start_x + i * distanceX;
        float y = start_y;

        if (this->startStep[curChildIndex] && reverse) {
            arr[i]->prevColor();
            arr[i]->prevLabel();
        }
        if (this->startStep[curChildIndex] && !reverse)
        {
            if (i < arr_size)
                arr[i]->newColor("normalFillColor", "newColor");
            else
                arr[i]->newColor("normalFillColor", "emptyColor");

            arr[i]->newLabel(std::to_string(i));

            arr[i]->nextLabel();
            arr[i]->nextColor();
        }

        if (this->startStep[curChildIndex])
        {
            moveNode(arr[i], x, y);
            this->startStep[curChildIndex] = 1;
            /* Because moveNode will set startStep = 0 */
        }

        else
            moveNode(arr[i], x, y);
    }
    if (this->startStep[curChildIndex]) this->startStep[curChildIndex] = 0;
}

void AnimationDA::hideThings(bool &show_arr, bool &show_new_label)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse) {
        show_arr = 1;
        show_new_label = 1;
    }
    else {
        show_arr = 0;
        show_new_label = 0;
    }
}

void AnimationDA::showThings(bool &showArray, bool &showLabel, std::vector<int> codeLines)
{   
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse) {
        codeHighlight->prev_currentLines();
        showArray = 0;
        showLabel = 0;
    }
    else {
        codeHighlight->next_currentLines(codeLines);
        showArray = 1;
        showLabel = 1;
    }
    return;
}

//Reset
void AnimationDA::updateNodePosition(std::vector<DANode *> arr, int arr_size, float start_x, float start_y, float distance)
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

void AnimationDA::newInstruction(std::vector<DANode *> arr, int arr_size, float start_x, float start_y, float distance)
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
