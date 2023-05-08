#include "AnimationDLL.h"

AnimationDLL::AnimationDLL(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight)
    : Animation(Colors, animationTime, codeHighlight)
{
    this->startStep.resize(5, 1);
    this->Colors = Colors;
    this->animationTime = animationTime;
    this->codeHighlight = codeHighlight;
}

AnimationDLL::~AnimationDLL()
{
}

// Initialize

// AnimationDLL

void AnimationDLL::addCodeLines(std::vector<int> codeLines)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;
    if (this->reverse)
        codeHighlight->prev_currentLines();
    else
        codeHighlight->next_currentLines(codeLines);
}

void AnimationDLL::changeHead(DLLNode *&head, DLLNode *old_head, DLLNode *targetNode, std::vector<int> codeLines)
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

void AnimationDLL::changeTail(DLLNode *&tail, DLLNode *old_tail, DLLNode *targetNode, std::vector<int> codeLines)
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

void AnimationDLL::showNode(DLLNode *targetNode, std::string targetLabel, int &numberNode, std::vector<int> codeLines)
{
    /* For newNode only */

    targetNode->showNode = 1;
    assert(targetNode->isNewNode);

    if (this->reverse && this->doneStep)
    {
        --numberNode;
        targetNode->prevColor();
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();
    }
    if (!this->reverse && this->startStep[curChildIndex])
    {
        ++numberNode;
        targetNode->newColor("newColor", "newColor");
        targetNode->newLabel(targetLabel);
        targetNode->nextColor();
        targetNode->nextLabel();
        codeHighlight->next_currentLines(codeLines);
    }
    if (this->startStep[curChildIndex])
        startStep[curChildIndex] = 0;

    float scale = 1.f / this->animationTime * this->dt;
    targetNode->shape.setScale(sf::Vector2f(scale, scale));
    targetNode->text.setScale(sf::Vector2f(scale, scale));
    targetNode->labelText.setScale(sf::Vector2f(scale, scale));

    targetNode->shape.setPosition(targetNode->x_center - targetNode->radius * scale,
                                  targetNode->y_center - targetNode->radius * scale);
    targetNode->updateText();
}

void AnimationDLL::hideNode(DLLNode *targetNode, int &numberNode, std::vector<int> codeLines)
{
    /* hide Node => hide Arrow also */
    /* For deleted Node only */

    if (this->reverse && this->doneStep)
    {
        codeHighlight->prev_currentLines();

        ++numberNode;
        if (targetNode->next != nullptr)
            targetNode->showArrowNext = 1;

        if (targetNode->prev != nullptr)
            targetNode->showArrowPrev = 1;

        if (targetNode->next != nullptr && targetNode->next->prev == targetNode)
            targetNode->next->showArrowPrev = 1;

        if (targetNode->prev != nullptr && targetNode->prev->next == targetNode)
            targetNode->prev->showArrowNext = 1;

    }
    if (!this->reverse && this->startStep[curChildIndex])
    {
        codeHighlight->next_currentLines(codeLines);

        --numberNode;

        targetNode->showArrowNext = 0;
        targetNode->showArrowPrev = 0;

        if (targetNode->next != nullptr && targetNode->next->prev == targetNode) {
            targetNode->next->showArrowPrev = 0;
        }

        if (targetNode->prev != nullptr && targetNode->prev->next == targetNode) {
            targetNode->prev->showArrowNext = 0;
        }
    }
    if (this->startStep[curChildIndex])
        startStep[curChildIndex] = 0;
    targetNode->showNode = 1;

    float scale = 1.f - 1.f / this->animationTime * this->dt;
    targetNode->shape.setScale(sf::Vector2f(scale, scale));
    targetNode->text.setScale(sf::Vector2f(scale, scale));
    targetNode->labelText.setScale(sf::Vector2f(scale, scale));

    targetNode->shape.setPosition(targetNode->x_center - targetNode->radius * scale,
                                  targetNode->y_center - targetNode->radius * scale);
    targetNode->updateText();
}

void AnimationDLL::changeNodeLabel(DLLNode *targetNode, std::string targetLabel, std::vector<int> codeLines)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse)
    {
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();
        return;
    }
    targetNode->newLabel(targetLabel);
    targetNode->nextLabel();
    codeHighlight->next_currentLines(codeLines);
}

void AnimationDLL::changeNodeValue(DLLNode *targetNode, int newValue, std::vector<int> codeLines)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse)
    {
        targetNode->prevVal();
        codeHighlight->prev_currentLines();
        return;
    }
    targetNode->newVal(newValue);
    targetNode->nextVal();
    codeHighlight->next_currentLines(codeLines);
}

void AnimationDLL::connectNext(DLLNode *targetNode, DLLNode *nextNode, std::vector<int> codeLines) 
{
    if (this->reverse && this->doneStep)
    {
        targetNode->prevNext();

        codeHighlight->prev_currentLines();

        if (targetNode->next != nullptr)
        {
            targetNode->showArrowNext = 1;
            this->makeArrow(&targetNode->shape, &targetNode->next->shape, &targetNode->arrowNext);
        }
        else
        {
            targetNode->showArrowNext = 0;
        }

        targetNode->arrowNext.setScale(sf::Vector2f(1, 1));
        return;
    }

    if (!this->reverse && this->startStep[curChildIndex])
    {
        targetNode->newNext(nextNode);
        targetNode->nextNext();
        codeHighlight->next_currentLines(codeLines);
    }
    if (this->startStep[curChildIndex])
        this->startStep[curChildIndex] = 0;

    if (targetNode->next != nullptr)
    {
        targetNode->showArrowNext = 1;
        this->makeArrow(&targetNode->shape, &targetNode->next->shape, &targetNode->arrowNext);
    }
    else
    {
        targetNode->showArrowNext = 0;
    }

    float scale = 1.f / this->animationTime * this->dt;
    targetNode->arrowNext.setScale(sf::Vector2f(scale, 1));
}

void AnimationDLL::connectPrev(DLLNode *targetNode, DLLNode *prevNode, std::vector<int> codeLines) 
{
    if (this->reverse && this->doneStep)
    {
        targetNode->prevPrev();

        codeHighlight->prev_currentLines();

        if (targetNode->prev != nullptr)
        {
            targetNode->showArrowPrev = 1;
            this->makeArrow(&targetNode->shape, &targetNode->prev->shape, &targetNode->arrowPrev);
        }
        else
        {
            targetNode->showArrowPrev = 0;
        }

        targetNode->arrowPrev.setScale(sf::Vector2f(1, 1));
        return;
    }

    if (!this->reverse && this->startStep[curChildIndex])
    {
        targetNode->newPrev(prevNode);
        targetNode->nextPrev();
        codeHighlight->next_currentLines(codeLines);
    }
    if (this->startStep[curChildIndex])
        this->startStep[curChildIndex] = 0;

    if (targetNode->prev != nullptr)
    {
        targetNode->showArrowPrev = 1;
        this->makeArrow(&targetNode->shape, &targetNode->prev->shape, &targetNode->arrowPrev);
    }
    else
    {
        targetNode->showArrowPrev = 0;
    }

    float scale = 1.f / this->animationTime * this->dt;
    targetNode->arrowPrev.setScale(sf::Vector2f(scale, 1));
}

void AnimationDLL::highlightCurrentNode(DLLNode *targetNode, std::string targetLabel, std::string targetColor, std::vector<int> codeLines)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse)
    {
        targetNode->prevColor();
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();
        return;
    }
    codeHighlight->next_currentLines(codeLines);

    targetNode->newColor(targetColor, targetColor);
    targetNode->newLabel(targetLabel);

    targetNode->nextColor();
    targetNode->nextLabel();
}

void AnimationDLL::highlightPassedNode(DLLNode *targetNode, std::string passedLabel, std::string currentLabel, std::string passedColor, std::string currentColor, std::vector<int> codeLines)
{
    if (!this->startStep[curChildIndex])
        return;
    this->startStep[curChildIndex] = 0;

    if (this->reverse)
    {
        targetNode->prevColor();
        targetNode->prevLabel();
        codeHighlight->prev_currentLines();

        if (targetNode->next == nullptr)
            return;

        targetNode->next->prevColor();
        targetNode->next->prevLabel();
        return;
    }
    codeHighlight->next_currentLines(codeLines);

    targetNode->newColor("normalFillColor", passedColor);
    targetNode->newLabel(passedLabel);

    targetNode->nextColor();
    targetNode->nextLabel();

    if (targetNode->next == nullptr)
        return;
    ;

    targetNode->next->newColor(currentColor, currentColor);
    targetNode->next->newLabel(currentLabel);

    targetNode->next->nextColor();
    targetNode->next->nextLabel();
}

void AnimationDLL::moveNode(DLLNode *targetNode, float x1, float y1)
{
    if (this->reverse && this->doneStep)
    {
        targetNode->prevPos();
        return;
    }

    if (!this->reverse && this->doneStep)
    {
        targetNode->newPos(sf::Vector2f(x1, y1));
        targetNode->nextPos();
        return;
    }

    if (this->startStep[curChildIndex])
        this->startStep[curChildIndex] = 0;

    float x0 = targetNode->x;
    float y0 = targetNode->y;
    // std::cout<<"node, re,x0,y0,x1,y0="<<targetNode->val<<' '<<reverse<<' '<<x0<<' '<<y0<<' '<<x1<<' '<<y1<<'\n';
    if (reverse)
    {
        x0 = targetNode->array_pos[targetNode->idx_pos - 1].x;
        y0 = targetNode->array_pos[targetNode->idx_pos - 1].y;
    }
    float sx = x1 - x0;
    float sy = y1 - y0;

    float x = x0 + sx / this->animationTime * this->dt;
    float y = y0 + sy / this->animationTime * this->dt;

    targetNode->shape.setPosition(x, y);
}

void AnimationDLL::makeArrow(sf::CircleShape *node1, sf::CircleShape *node2, sf::RectangleShape *targetArrow)
{
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

void AnimationDLL::Relayout(bool emptyList, DLLNode *head, DLLNode *tail, float start_x, float start_y, float distance, std::vector<int> codeLines)
{
    if (emptyList)
    {
        if (!this->startStep[curChildIndex])
            return;
        this->startStep[curChildIndex] = 0;

        if (this->reverse)
            codeHighlight->prev_currentLines();
        else
            codeHighlight->next_currentLines(codeLines);
        return;
    }

    // std::cout << "Replayout\n";

    if (this->startStep[curChildIndex])
    {
        this->startStep[curChildIndex] = 0;
        for (DLLNode *cur = head; cur != tail->next; cur = cur->next)
        {
            if (cur == head)
                cur->newLabel("head");
            else if (cur == tail)
                cur->newLabel("tail");
            else
                cur->newLabel("");

            if (this->reverse)
            {
                cur->prevLabel();
                codeHighlight->prev_currentLines();
            }
            else
            {
                cur->nextLabel();
                codeHighlight->next_currentLines(codeLines);
            }
        }
    }

    head->index = 0;

    for (DLLNode *cur = head; cur != tail->next; cur = cur->next)
    {
        // float x0 = cur->x;
        // float y0 = cur->y;
        float x1 = start_x + cur->index * distance;
        float y1 = start_y;

        this->moveNode(cur, x1, y1);
        if (cur != tail)
        {
            cur->next->index = cur->index + 1;
        }
    }

    /* Show Arrow */
    for (DLLNode *cur = head; cur != tail; cur = cur->next)
    {
        this->makeArrow(&cur->shape, &cur->next->shape, &cur->arrowNext);
        this->makeArrow(&cur->next->shape, &cur->shape, &cur->next->arrowPrev);
    }
}

// Reset
void AnimationDLL::updateNodePosition(DLLNode *head, DLLNode *tail, float start_x, float start_y, float distance)
{
    if (head == nullptr)
        return;

    head->index = 0;

    for (DLLNode *cur = head; cur != tail->next; cur = cur->next)
    {
        cur->labelString = "";
        cur->isNewNode = 0;
        cur->showNode = 1;

        cur->x = start_x + cur->index * distance;
        cur->y = start_y;
        cur->shape.setPosition(cur->x, cur->y);

        if (cur != tail)
            cur->next->index = cur->index + 1;
    }

    head->labelString = "head";
    tail->labelString = "tail";
    head->prev = nullptr;
    tail->next = nullptr;
    head->showArrowPrev = 0;
    tail->showArrowNext = 0;

    if (head == tail)
        head->labelString = "head/tail";

    /* Show Arrow */
    for (DLLNode *cur = head; cur != tail; cur = cur->next)
    {
        cur->showArrowNext = 1;
        cur->next->showArrowPrev = 1;
        this->makeArrow(&cur->shape, &cur->next->shape, &cur->arrowNext);
        this->makeArrow(&cur->next->shape, &cur->shape, &cur->next->arrowPrev);
    }

    for (DLLNode *cur = head; cur != tail->next; cur = cur->next)
    {
        cur->reset();
    }
}

void AnimationDLL::newInstruction(DLLNode *head, DLLNode *tail, float start_x, float start_y, float distance)
{
    /* Already completed all AnimationDLLs */
    this->updateNodePosition(head, tail, start_x, start_y, distance);

    this->resetStartStep();
    this->curIndex = 0;

    for (auto &ins : instructions)
        ins.clear();
    this->instructions.clear();

    this->animationClock.restart();
    this->codeHighlight->reset();
}
