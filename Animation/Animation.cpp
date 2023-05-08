#include "Animation.h"

Animation::Animation(std::map<std::string, sf::Color> *Colors, float animationTime, CodeHighlight *codeHighlight)
{
    this->startStep.resize(5, 1);
    this->Colors = Colors;
    this->animationTime = animationTime;
    this->codeHighlight = codeHighlight;
}

Animation::~Animation()
{
    // delete Colors;
}

// Running

void Animation::pause(bool &doneAnimation, sf::Text *step)
{
    doneAnimation = 1;
}

void Animation::first(bool &doneAnimation, sf::Text *step)
{
    /* Already finished undone step in first trigger */
    step->setString("Step: 0 / " + std::to_string((int)instructions.size()));
    this->reverse = 1;
    this->dt = 0;
    while (curIndex > 0) {
        --curIndex;
        this->resetStartStep();
        this->doneStep = 1;
        this->triggerOneIns(curIndex);
    }
    assert(curIndex == 0);

    doneAnimation = 1;
    return;
}

void Animation::prev(bool &doneAnimation, sf::Text *step)
{
    this->reverse = 1;
    if (this->doneStep && this->curIndex == 0)
    {
        doneAnimation = 1;
        return;
    }

    if (this->doneStep) {
        this->resetStartStep();
        --this->curIndex;
        step->setString("Step: " + std::to_string(curIndex) + " / " + std::to_string((int)instructions.size()));
    }

    this->activateStep();

    if (this->doneStep)
    {
        doneAnimation = 1;
    }

}

void Animation::next(bool &doneAnimation, sf::Text *step)
{   
    this->reverse = 0;
    if (this->curIndex == (int)this->instructions.size())
    {
        doneAnimation = 1;
        return;
    }

    if (this->doneStep) {
        this->resetStartStep();
        step->setString("Step: " + std::to_string(curIndex + 1) + " / " + std::to_string((int)instructions.size()));
    }
    this->activateStep();

    if (this->doneStep)
    {
        ++this->curIndex;
        doneAnimation = 1;
    }
    
}

void Animation::last(bool &doneAnimation, sf::Text *step)
{
    step->setString("Step: " + std::to_string((int)instructions.size()) + " / " + std::to_string((int)instructions.size()));
    this->reverse = 0;
    this->dt = this->animationTime;
    while (curIndex < (int)instructions.size()) {
        this->resetStartStep();
        this->doneStep = 1;
        this->triggerOneIns(curIndex);
        ++curIndex;
    }
    doneAnimation = 1;
    return;
}

void Animation::play(bool &doneAnimation, sf::Text *step)
{
    this->reverse = 0;
    if (this->curIndex == (int)this->instructions.size())
    {
        doneAnimation = 1;
        return;
    }
    step->setString("Step: " + std::to_string(curIndex + 1) + " / " + std::to_string((int)instructions.size()));

    this->activateStep();
    if (this->doneStep) {
        ++this->curIndex;
    }

    return;
}

void Animation::replay(bool &doneAnimation, sf::Text *step)
{
}

void Animation::resetStartStep()
{
    startStep.assign(5, 1);
}

void Animation::triggerOneIns(int index)
{   
    curChildIndex = 0;
    for (auto &ins : instructions[index]) {
        ++curChildIndex;
        ins();
    }
}

void Animation::finishStep()
{
    if (this->doneStep) {
        this->resetStartStep();
        this->animationClock.restart();
        return;
    }
    this->doneStep = 1;
    this->dt = this->animationTime;
    if (this->reverse) dt = animationTime - dt;
    
    if (this->curIndex < (int)this->instructions.size()) {
        this->triggerOneIns(curIndex);
    }
    if (!this->reverse && this->curIndex < (int)this->instructions.size()) {
        ++this->curIndex;
    }
    this->animationClock.restart();
    this->resetStartStep();
}

void Animation::activateStep()
{
    this->dt = this->animationClock.getElapsedTime().asSeconds();
    this->dt = std::min(this->dt, this->animationTime);

    if (this->dt < this->animationTime)
    {
        if (this->reverse)
            this->dt = this->animationTime - this->dt;
        this->doneStep = 0;
        this->triggerOneIns(curIndex);
        return;
    }
    if (this->reverse)
        this->dt = this->animationTime - this->dt;
    this->doneStep = 1;
    this->triggerOneIns(curIndex);
    this->animationClock.restart();
    this->resetStartStep();
}
