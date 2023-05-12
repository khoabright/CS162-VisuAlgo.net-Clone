#include "AnimationCLL.h"

AnimationCLL::AnimationCLL(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight)
    : Animation(Colors, animationTime, codeHighlight)
{
    this->startStep.resize(5, 1);
    this->Colors = Colors;
    this->animationTime = animationTime;
    this->codeHighlight = codeHighlight;
}

AnimationCLL::~AnimationCLL()
{
}

// Initialize

// AnimationCLL

void AnimationCLL::addCodeLines(std::vector<int> codeLines)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;
    if (this->reverse) codeHighlight->prev_currentLines();
    else codeHighlight->next_currentLines(codeLines);
}

void AnimationCLL::changeHead(CLLNode *&head, CLLNode *old_head, CLLNode *targetNode, std::vector<int> codeLines)
{   
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse)
    {
        head = old_head;
        head->prevLabel();
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();
        return;
    }

    head = targetNode;
    head->newLabel("head");

    if (old_head->labelString == "head/tail")
        old_head->newLabel("tail");
    else
        old_head->newLabel("");

    head->nextLabel();
    old_head->nextLabel();
    codeHighlight->next_currentLines(codeLines);

    return;
}

void AnimationCLL::changeTail(CLLNode *&tail, CLLNode *old_tail, CLLNode *targetNode, std::vector<int> codeLines)
{   
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse)
    {
        tail = old_tail;
        tail->prevLabel();
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();
        return;
    }

    tail = targetNode;
    tail->newLabel("tail");

    if (old_tail->labelString == "head/tail")
        old_tail->newLabel("head");
    else
        old_tail->newLabel("");
        
    tail->nextLabel();
    old_tail->nextLabel();
    codeHighlight->next_currentLines(codeLines);

    return;
}

void AnimationCLL::showNode(CLLNode *targetNode, std::string targetLabel, int &numberNode, std::vector<int> codeLines) 
{   
    /* For newNode only */

    targetNode->showNode = 1;
    assert(targetNode->isNewNode);

    if (this->reverse && this->doneStep) {
        --numberNode;
        targetNode->prevColor();
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();
    }
    if (!this->reverse && this->startStep[curChildIndex]) {
        ++numberNode;
        targetNode->newColor("newColor", "newColor");
        targetNode->newLabel(targetLabel);
        targetNode->nextColor();
        targetNode->nextLabel();
        codeHighlight->next_currentLines(codeLines);
    }
    if (this->startStep[curChildIndex]) startStep[curChildIndex] = 0;

    float scale = 1.f / this->animationTime * this->dt;
    targetNode->shape.setScale(sf::Vector2f(scale, scale));
    targetNode->text.setScale(sf::Vector2f(scale, scale));
    targetNode->labelText.setScale(sf::Vector2f(scale, scale));

    targetNode->shape.setPosition(targetNode->x_center - targetNode->radius * scale,
                                  targetNode->y_center - targetNode->radius * scale);
    targetNode->updateText();
}

void AnimationCLL::hideNode(CLLNode *targetNode, int &numberNode, std::vector<int> codeLines)
{   
    /* hide Node => hide Arrow also */
    /* For deleted Node only */

    if (this->reverse && this->doneStep) {
        ++numberNode;
        if (targetNode->next != nullptr) targetNode->showArrow = 1;
        codeHighlight->prev_currentLines();
    }
    if (!this->reverse && this->startStep[curChildIndex]) {
        --numberNode;
        targetNode->showArrow = 0;
        codeHighlight->next_currentLines(codeLines);
    }
    if (this->startStep[curChildIndex]) startStep[curChildIndex] = 0;
    targetNode->showNode = 1;

    float scale = 1.f - 1.f / this->animationTime * this->dt;
    targetNode->shape.setScale(sf::Vector2f(scale, scale));
    targetNode->text.setScale(sf::Vector2f(scale, scale));
    targetNode->labelText.setScale(sf::Vector2f(scale, scale));

    targetNode->shape.setPosition(targetNode->x_center - targetNode->radius * scale,
                                  targetNode->y_center - targetNode->radius * scale);
    targetNode->updateText();
}

void AnimationCLL::changeNodeLabel(CLLNode *targetNode, std::string targetLabel, std::vector<int> codeLines)
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

void AnimationCLL::changeNodeValue(CLLNode *targetNode, int newValue, std::vector<int> codeLines)
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

void AnimationCLL::connectNodes(CLLNode *targetNode, CLLNode *nextNode, std::vector<int> codeLines)
{   
    if (this->reverse && this->doneStep)
    {   
        targetNode->prevNext();
        codeHighlight->prev_currentLines();

        if (targetNode->next != nullptr) {
            targetNode->showArrow = 1;
            this->makeArrow(&targetNode->shape, &targetNode->next->shape, &targetNode->arrow);
        }
        else {
            targetNode->showArrow = 0;
        }

        targetNode->arrow.setScale(sf::Vector2f(1, 1));
        return;
    }

    if (!this->reverse && this->startStep[curChildIndex]) {
        targetNode->newNext(nextNode);
        targetNode->nextNext();
        codeHighlight->next_currentLines(codeLines);
    }
    if (this->startStep[curChildIndex]) this->startStep[curChildIndex] = 0;

    if (targetNode->next != nullptr) {
        targetNode->showArrow = 1;
        this->makeArrow(&targetNode->shape, &targetNode->next->shape, &targetNode->arrow);
    }
    else {
        targetNode->showArrow = 0;
    }

    float scale = 1.f / this->animationTime * this->dt;
    targetNode->arrow.setScale(sf::Vector2f(scale, 1));
}

void AnimationCLL::highlightCurrentNode(CLLNode *targetNode, std::string targetLabel, std::string fillColor, std::string outlineColor, std::vector<int> codeLines)
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

void AnimationCLL::highlightPassedNode(CLLNode *targetNode, std::string passedLabel, std::string currentLabel, std::string passedColor, std::string currentColor, std::vector<int> codeLines)
{      
    if (!this->startStep[curChildIndex]) return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse) {
        targetNode->prevColor();
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();

        if (targetNode->next == nullptr) return;

        targetNode->next->prevColor();
        targetNode->next->prevLabel();
        return;
    }
    codeHighlight->next_currentLines(codeLines);

    targetNode->newColor("normalFillColor", passedColor);
    targetNode->newLabel(passedLabel);

    targetNode->nextColor();
    targetNode->nextLabel();

    if (targetNode->next == nullptr) return;;

    targetNode->next->newColor(currentColor, currentColor);
    targetNode->next->newLabel(currentLabel);

    targetNode->next->nextColor();
    targetNode->next->nextLabel();
}


void AnimationCLL::moveNode(CLLNode *targetNode, float x1, float y1)
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

void AnimationCLL::makeArrow(sf::CircleShape *node1, sf::CircleShape *node2, sf::RectangleShape *targetArrow)
{   
    if (node1 == node2) {
        targetArrow->setSize(sf::Vector2f(0, targetArrow->getLocalBounds().height));
        return;
    }
    /* Replace arrow such that it point from node1 -> node 2 */
    float pi = atan(1) * 4;
    float r1, r2, x1, y1, x2, y2, alpha, xM, yM, distance;
    auto initVariables = [&]()
    {   
        r1 = node1->getRadius();
        r2 = node2->getRadius();

        x1 = node1->getPosition().x;
        y1 = node1->getPosition().y;

        x2 = node2->getPosition().x;
        y2 = node2->getPosition().y;

        if (x1 == x2)
            alpha = pi / 2;
        else
        {
            alpha = atan(fabs(y2 - y1) / fabs(x2 - x1));
        }

        xM = x1 + r1 + r1 * cos(alpha);
        yM = y1 + r1 + r1 * sin(alpha);

        float s1 = (x2 - x1);
        float s2 = (y2 - y1);

        distance = sqrt(s1 * s1 + s2 * s2);
    };

    initVariables();

    if (x1 > x2)
    {
        if (y1 > y2)
        {
            alpha = pi + alpha;
            xM = 2 * (x1 + r1) - xM;
            yM = 2 * (y1 + r1) - yM;
        }
        else
        {
            xM = xM - 2 * r1 * cos(alpha);
            alpha = pi - alpha;
        }
    }
    else if (y1 > y2)
    {
        yM = yM - 2 * r1 * sin(alpha);
        alpha = 2 * pi - alpha;
    }

    targetArrow->setSize(sf::Vector2f(distance - r1 - r2, targetArrow->getLocalBounds().height));
    targetArrow->setPosition(xM, yM);
    targetArrow->setRotation(alpha / pi * 180);
}

void AnimationCLL::Relayout(bool emptyList, CLLNode *head, int numberNode, float start_x, float start_y, float distance_x, float distance_y, std::vector<int> codeLines)
{       
    if (emptyList) {
        if (!this->startStep[curChildIndex]) return;
        this->startStep[curChildIndex] = 0;

        if (this->reverse)
            codeHighlight->prev_currentLines();
        else
            codeHighlight->next_currentLines(codeLines);
        return;
    }


    if (this->startStep[curChildIndex]) {
        this->startStep[curChildIndex] = 0;
        for (CLLNode *cur = head; ; cur = cur->next) {
            if (cur == head) cur->newLabel("head");
            else cur->newLabel("");

            if (this->reverse) {
                cur->prevLabel();
                codeHighlight->prev_currentLines();
            }
            else {
                cur->nextLabel();
                codeHighlight->next_currentLines(codeLines);
            }
            if (cur->next == head) break;
        }
    }

    head->index = 0;

    for (CLLNode *cur = head; ; cur = cur->next)
    {   
        float x1 = start_x + cur->index * distance_x;
        int add_y = cur->index;
        if (2 * add_y > numberNode - 1) add_y = numberNode - 1 - add_y;
        float y1 = start_y + add_y * distance_y;

        this->moveNode(cur, x1, y1);

        if (cur->next == head) break;
        cur->next->index = cur->index + 1;
    }

    /* Show Arrow */
    for (CLLNode *cur = head; ; cur = cur->next)
    {
        this->makeArrow(&cur->shape, &cur->next->shape, &cur->arrow);
        if (cur->next == head) break;
    }
}

//Reset
void AnimationCLL::updateNodePosition(CLLNode *head, int numberNode, float start_x, float start_y, float distance_x, float distance_y)
{   
    if (head == nullptr)
        return;

    head->index = 0;
    

    for (CLLNode *cur = head; ; cur = cur->next)
    {
        cur->labelString = "";
        cur->isNewNode = 0;
        cur->showNode = 1;
        
        cur->x = start_x + cur->index * distance_x;
        int add_y = cur->index;
        if (2 * add_y > numberNode - 1) add_y = numberNode - 1 - add_y;
        cur->y = start_y + add_y * distance_y;

        cur->shape.setPosition(cur->x, cur->y);

        if (cur->next == head) break;
        
        cur->next->index = cur->index + 1;
    }

    head->labelString = "head";

    /* Show Arrow */
    for (CLLNode *cur = head; ; cur = cur->next)
    {   
        if (numberNode > 1) cur->showArrow = 1;
        this->makeArrow(&cur->shape, &cur->next->shape, &cur->arrow);
        if (cur->next == head) break;
    }

    for (CLLNode *cur = head; ; cur = cur->next)
    {
        cur->reset();
        if (cur->next == head) break;
    }
}

void AnimationCLL::newInstruction(CLLNode *head, int numberNode, float start_x, float start_y, float distance_x, float distance_y)
{   
    /* Already completed all animations */
    this->updateNodePosition(head, numberNode, start_x, start_y, distance_x, distance_y);

    this->resetStartStep();
    this->curIndex = 0;

    for (auto &ins : instructions) ins.clear();
    this->instructions.clear();

    this->animationClock.restart();
    this->codeHighlight->reset();
}
