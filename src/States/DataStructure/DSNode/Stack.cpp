#include "Stack.h"

Stack::Stack(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states)
    : DataStructure(window, supportedKeys, states)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->exist.resize(105);

    auto StackScale = [&]()
    {
        nodeDistanceX *= scale_x;
        nodeDistanceY *= scale_y;
    };

    auto childButtonScale = [&]()
    {
        childButtonOriginX *= scale_x;
        childButtonOriginY *= scale_y;
        childButtonWidth *= scale_x;
        childButtonHeight *= scale_y;
        childButtonCharacterSize *= scale_y;
        childButtonDistanceX *= scale_x;
    };

    auto inputFieldScale = [&]()
    {
        inputFieldOriginX *= scale_x;
        inputFieldOriginY *= scale_y;
        inputFieldWidth *= scale_x;
        inputFieldHeight *= scale_y;
        inputFieldCharacterSize *= scale_y;
        inputFieldDistanceX *= scale_x;
    };

    childButtonScale();
    inputFieldScale();
    StackScale();

    this->ReInitButtons();
    this->initHighlightCode();
    this->initTriggerFunction();
    this->initChildButtons();
    this->initInputFields();
    this->initAnimation();
}

Stack::~Stack()
{
    this->DeleteNodePointers();
    delete (this->animationStack);
}

void Stack::DeleteNodePointers()
{
    for (auto &v : this->Nodes)
    {
        delete (v);
    }
    this->Nodes.clear();
}

/* Init */
void Stack::initTriggerFunction()
{
    auto InstructionButtons = [&]()
    {
        this->triggerFunction["Theme"] = std::bind(&Stack::button_theme, this);
        this->triggerFunction["Initialize"] = std::bind(&Stack::button_initialize, this);
        this->triggerFunction["Add"] = std::bind(&Stack::button_add, this);
        this->triggerFunction["Delete"] = std::bind(&Stack::button_delete, this);
        this->triggerFunction["Clear"] = std::bind(&Stack::button_clear, this);
        this->triggerFunction["Quit"] = std::bind(&Stack::button_quit, this);
        this->triggerFunction["SpeedMinus"] = std::bind(&Stack::button_SpeedMinus, this);
        this->triggerFunction["SpeedPlus"] = std::bind(&Stack::button_SpeedPlus, this);
        this->triggerFunction["CodeSwitch"] = std::bind(&Stack::button_CodeSwitch, this);
    };

    auto RunButtons = [&]()
    {
        this->triggerFunction["First"] = std::bind(&Stack::button_first, this);
        this->triggerFunction["Prev"] = std::bind(&Stack::button_prev, this);
        this->triggerFunction["Pause"] = std::bind(&Stack::button_pause, this);
        this->triggerFunction["Play"] = std::bind(&Stack::button_play, this);
        this->triggerFunction["Replay"] = std::bind(&Stack::button_replay, this);
        this->triggerFunction["Next"] = std::bind(&Stack::button_next, this);
        this->triggerFunction["Last"] = std::bind(&Stack::button_last, this);
    };

    InstructionButtons();
    RunButtons();
}

void Stack::initChildButtons()
{
    int idy = -1;

    auto init_theme = [&]()
    {
        ++idy;
        this->childButtons["Theme"].insert({"1Standard", nullptr});
        this->childButtons["Theme"].insert({"2Sky", nullptr});
        this->childButtons["Theme"].insert({"3Desert", nullptr});

        int idx = 0;
        for (auto &it : this->childButtons["Theme"])
        {
            it.second = new Button(
                this->childButtonOriginX + (idx++) * (this->childButtonDistanceX + this->childButtonWidth),
                this->childButtonOriginY + idy * (this->childButtonDistanceY + this->childButtonHeight),
                this->scale_x, this->scale_y,
                &this->font, it.first.substr(1), (int)(scale_y * 16.f),
                sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
                sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
                1, this->childButtonWidth, this->childButtonHeight);
        }
        this->childButtons["Theme"]["1Standard"]->instantTrigger = true;
        this->childButtons["Theme"]["2Sky"]->instantTrigger = true;
        this->childButtons["Theme"]["3Desert"]->instantTrigger = true;
    };

    auto init_initialize = [&]()
    {
        ++idy;
        this->childButtons["Initialize"].insert({"1Empty", nullptr});
        this->childButtons["Initialize"].insert({"2Random", nullptr});
        this->childButtons["Initialize"].insert({"3Load File", nullptr});

        int idx = 0;
        for (auto &it : this->childButtons["Initialize"])
        {
            it.second = new Button(
                this->childButtonOriginX + (idx++) * (this->childButtonDistanceX + this->childButtonWidth),
                this->childButtonOriginY + idy * (this->childButtonDistanceY + this->childButtonHeight),
                this->scale_x, this->scale_y,
                &this->font, it.first.substr(1), (int)(scale_y * 16.f),
                sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
                sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
                1, this->childButtonWidth, this->childButtonHeight);
        }
        this->childButtons["Initialize"]["1Empty"]->instantTrigger = true;
        this->childButtons["Initialize"]["3Load File"]->instantTrigger = true;
    };

    init_theme();
    init_initialize();
}

void Stack::initInputFields()
{
    int idy = -1;

    auto init_initialize = [&]()
    {
        ++idy;
        this->inputFields["Initialize"].insert({"1Empty", nullptr});
        this->inputFields["Initialize"].insert({"2Random", nullptr});
        this->inputFields["Initialize"].insert({"3Load File", nullptr});

        int idx = 0;
        for (auto &it : this->inputFields["Initialize"])
        {
            it.second = new InputField(this->inputFieldOriginX + (idx++) * (this->inputFieldWidth + this->inputFieldDistanceX),
                                       this->inputFieldOriginY + idy * (this->inputFieldHeight + this->inputFieldDistanceY),
                                       this->inputFieldWidth, this->inputFieldHeight, this->scale_x, this->scale_y,
                                       &this->font, this->inputFieldCharacterSize,
                                       sf::Color::White, sf::Color::White, sf::Color::Black);
        }

        this->inputFields["Initialize"]["1Empty"]->disabled = true;
        this->inputFields["Initialize"]["3Load File"]->disabled = true;
    };

    auto init_add = [&]()
    {
        this->inputFields["Add"]["1Add"] = new InputField(this->inputFieldOriginX, 603.f * this->scale_y,
                                                                this->inputFieldWidth, this->inputFieldHeight, this->scale_x, this->scale_y,
                                                                &this->font, this->inputFieldCharacterSize,
                                                                sf::Color::White, sf::Color::White, sf::Color::Black);
    };

    init_initialize();
    init_add();
}

void Stack::initAnimation()
{
    this->animation = new AnimationStack(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationStack = dynamic_cast<AnimationStack*>(this->animation);
    assert (animationStack != nullptr);
    this->animationStack->animationTime = speedGap * (float)maxSpeed;

}

void Stack::initHighlightCode()
{
    codeHighlight = new CodeHighlight(scale_x, scale_y, &Colors);
}

void Stack::button_theme()
{

    if (this->choosingChildButton == "1Standard")
    {
        this->newStepTriggered = 0;
        this->DataStructureThemeStandard();
        for (auto &it : Nodes)
            it->updateColor();
        this->choosingButton = "";
        return;
    }
    if (this->choosingChildButton == "2Sky")
    {
        this->newStepTriggered = 0;
        this->DataStructureThemeSky();
        for (auto &it : Nodes)
            it->updateColor();
        this->choosingButton = "";
        return;
    }
    if (this->choosingChildButton == "3Desert")
    {
        this->newStepTriggered = 0;
        this->DataStructureThemeDesert();
        for (auto &it : Nodes)
            it->updateColor();
        this->choosingButton = "";
        return;
    }

    return;
}

/* Instruction Buttons */
void Stack::button_initialize()
{
    auto createNewNode = [&](int newValue)
    {
        if (this->head == nullptr)
        {
            this->head = new StackNode(this->start_x, this->start_y, this->scale_x, this->scale_y, 12, &this->font, &this->Colors);
            this->Nodes.push_back(this->head);
            ++this->numberNode;

            this->head->val = newValue;
            this->tail = head;
            return;
        }
        /* Add to tail */
        StackNode *newNode = new StackNode(this->start_x, this->start_y, this->scale_x, this->scale_y, 12, &this->font, &this->Colors);
        this->Nodes.push_back(newNode);
        ++this->numberNode;

        newNode->val = newValue;
        this->tail->next = newNode;
        this->tail = newNode;
    };

    if (this->choosingChildButton == "1Empty")
    {
        if (!this->newStepTriggered)
            return;
        this->newStepTriggered = 0;

        this->reset();
        return;
    }

    if (this->choosingChildButton == "2Random")
    {
        if (this->newStepTriggered)
        {
            this->newStepTriggered = 0;
            this->inputGuide.setString("");
            if (this->valueFirst > maxNode)
            {
                this->inputWarning.setString("Sorry, the maximum size is " + std::to_string(maxNode));
                return;
            }
        }
        else
        {
            this->inputGuide.setString("Input a size");
            return;
        }

        this->reset();
        std::vector<int> values(99);
        for (int i = 0; i < 99; ++i)
            values[i] = i + 1;
        std::shuffle(values.begin(), values.end(), this->randomize);
        for (int i = 0; i < this->valueFirst; ++i)
        {
            int newValue = values[i];
            assert(this->exist[newValue] == 0);
            this->exist[newValue] = 1;
            createNewNode(newValue);
        }
        this->animationStack->updateNodePosition(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }

    if (this->choosingChildButton == "3Load File")
    {
        if (!this->newStepTriggered)
            return;
        this->newStepTriggered = 0;

        this->reset();
        std::ifstream inp;
        inp.open("InputFiles/inputStack.txt");
        int newValue = 0;
        while (inp >> newValue)
        {
            if (this->exist[newValue])
            {
                this->inputWarning.setString("No duplicate vertex allowed!");
                return;
            }
            this->exist[newValue] = 1;
            createNewNode(newValue);
        }
        inp.close();

        this->animationStack->updateNodePosition(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }
}

void Stack::button_add()
{
    this->choosingChildButton = "1Add";

    if (this->newStepTriggered)
    {
        this->newStepTriggered = 0;
        this->inputGuide.setString("");
        this->addHead(this->valueFirst);
        return;
    }
    else
    {
        this->inputGuide.setString("Input a value");
    }
}

void Stack::button_delete()
{
    this->deleteHead();
    return;
}

void Stack::button_clear()
{
    this->reset();
    return;
}


void Stack::button_quit()
{
    this->endState();
}

/* Stack operations */
// Add
void Stack::addHead(int nodeValue)
{   
    /* Complete all animations */
    this->prepareNewInstruction();
    this->button_play();
    
    if (nodeValue < 1 || nodeValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }

    if (numberNode + 1 > maxNode)
    {
        this->inputWarning.setString("Sorry, the maximum size is " + std::to_string(maxNode));
        return;
    }

    if (this->exist[nodeValue])
    {
        this->inputWarning.setString("No duplicate vertex allowed!");
        return;
    }
    this->exist[nodeValue] = 1;

    if (this->head == nullptr)
    {
        auto addHighlightCodes = [&]
        {
            codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to empty list");
            codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");
            codeHighlight->codeStrings.push_back("head = newNode");
            codeHighlight->codeStrings.push_back("tail = newNode");
            this->codeHighlight->updateTexts();
        };
        addHighlightCodes();

        /* Create and Show Node */
        this->head = new StackNode(this->start_x, this->start_y, this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
        this->head->isNewNode = 1;
        this->Nodes.push_back(this->head);

        this->tail = this->head;

        this->animationStack->instructions.push_back({[this]()
                                                 { this->animationStack->showNode(this->head, "newNode", this->numberNode, {0}); }});
        this->animationStack->instructions.push_back({[this]()
                                                 { this->animationStack->changeNodeLabel(this->head, "head", {1}); }});
        this->animationStack->instructions.push_back({[this]()
                                                 { this->animationStack->changeNodeLabel(this->head, "head/tail", {2}); }});

        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to head");
        codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");
        codeHighlight->codeStrings.push_back("newNode.next = head");
        codeHighlight->codeStrings.push_back("head = newNode");
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation");
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    float xHead = this->head->x;
    float yHead = this->head->y;

    /* Create and Show Node */
    StackNode *newNode = new StackNode(xHead, yHead + this->nodeDistanceY,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;

    this->Nodes.push_back(newNode);

    this->animationStack->instructions.push_back({[this, newNode]()
                                             { this->animationStack->showNode(newNode, "newNode", this->numberNode, {0}); }});

    /* Connect Node to Head*/
    this->animationStack->instructions.push_back({[this, newNode]()
                                             { this->animationStack->connectNodes(newNode, this->head, {1}); }});

    /* Change head to newNode */
    StackNode *old_head = this->head;
    this->animationStack->instructions.push_back({[this, newNode, old_head]()
                                             { this->animationStack->changeHead(this->head, old_head, newNode, {2}); }});

    /* Relayout */
    this->animationStack->instructions.push_back({[this]()
                                             { this->animationStack->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {3}); }});

}

void Stack::addTail(int nodeValue)
{   
    /* Complete all animations */
    this->prepareNewInstruction();
    this->button_play();

    if (nodeValue < 1 || nodeValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }

    if (numberNode + 1 > maxNode)
    {
        this->inputWarning.setString("Sorry, the maximum size is " + std::to_string(maxNode));
        return;
    }

    if (this->exist[nodeValue])
    {
        this->inputWarning.setString("No duplicate vertex allowed!");
        return;
    }
    this->exist[nodeValue] = 1;

    if (this->head == nullptr)
    {
        auto addHighlightCodes = [&]
        {
            codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to empty list");
            codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");
            codeHighlight->codeStrings.push_back("head = newNode");
            codeHighlight->codeStrings.push_back("tail = newNode");
            this->codeHighlight->updateTexts();
        };
        addHighlightCodes();

        /* Create and Show Node */
        this->head = new StackNode(this->start_x, this->start_y, this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
        this->head->isNewNode = 1;
        this->Nodes.push_back(this->head);

        this->tail = head;

        this->animationStack->instructions.push_back({[this]()
                                                 { this->animationStack->showNode(this->head, "newNode", this->numberNode, {0}); }});
        this->animationStack->instructions.push_back({[this]()
                                                 { this->animationStack->changeNodeLabel(this->head, "head", {1}); }});
        this->animationStack->instructions.push_back({[this]()
                                                 { this->animationStack->changeNodeLabel(this->head, "head/tail", {2}); }});

        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to tail");
        codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");
        codeHighlight->codeStrings.push_back("tail.next = newNode");
        codeHighlight->codeStrings.push_back("tail = newNode");
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation");
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    float xTail = this->tail->x;
    float yTail = this->tail->y;

    /* Create and Show Node */
    StackNode *newNode = new StackNode(xTail + this->nodeDistanceX, yTail,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;
    this->Nodes.push_back(newNode);

    this->animationStack->instructions.push_back({[this, newNode]()
                                             { this->animationStack->showNode(newNode, "newNode", this->numberNode, {0}); }});

    /* connect Tail to newNode*/
    this->animationStack->instructions.push_back({[this, newNode]()
                                             { this->animationStack->connectNodes(this->tail, newNode, {1}); }});

    /* change Tail to newNode */
    StackNode *old_tail = this->tail;
    this->animationStack->instructions.push_back({[this, newNode, old_tail]()
                                             { this->animationStack->changeTail(this->tail, old_tail, newNode, {2}); }});

    /* Relayout */
    this->animationStack->instructions.push_back({[this]()
                                             { this->animationStack->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {3}); }});
}

// Delete
void Stack::deleteHead()
{
    this->prepareNewInstruction();
    this->button_play();

    auto addHighlightCodes = [&] {
        codeHighlight->introText.setString("Delete head");
        codeHighlight->codeStrings.push_back("if empty, do nothing"); // 0
        codeHighlight->codeStrings.push_back("Node target = head"); // 1
        codeHighlight->codeStrings.push_back("Node head = head.next"); // 2
        codeHighlight->codeStrings.push_back("delete target"); // 3
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 4
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    if (this->head == nullptr) {
        this->animationStack->instructions.push_back({[this]()
                                                 { this->animationStack->addCodeLines({0}); }});

        return;
    }

    /* Highlight current Node */
    StackNode *old_head = this->head;
    this->animationStack->instructions.push_back({[this, old_head]()
                                             { this->animationStack->highlightCurrentNode(old_head, "target", "deleteColor", {1}); }});

    if (this->numberNode > 1)
    {
        /* Change head to head->next */
        StackNode *new_head = this->head->next;
        this->animationStack->instructions.push_back({[this, old_head, new_head]()
                                                 { this->animationStack->changeHead(this->head, old_head, new_head, {2}); }});
    }
    else
    {
        this->head = nullptr;
        this->animationStack->instructions.push_back({[this]()
                                                 { this->animationStack->addCodeLines({2}); }});
    }

    /* Delete head */
    this->exist[old_head->val] = 0;
    this->animationStack->instructions.push_back({[this, old_head]()
                                             { this->animationStack->hideNode(old_head, this->numberNode, {3}); }});
    /* Relayout */
    this->animationStack->instructions.push_back({[this]()
                                             { this->animationStack->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {4}); }});

    return;
}

void Stack::prepareNewInstruction()
{
    this->animation->finishStep();
    bool trash = 0;
    this->animationStack->last(trash, &this->stepText);
    this->animationStack->newInstruction(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
    this->codeHighlight->reset();
    this->doneAnimation = 1;
}

/* Update and Render */
void Stack::updateButtons()
{
    /* Update all the buttons */
    for (auto it : this->buttons)
    {
        if (it.second->isPressed())
        {   
            if (this->choosingButton != it.first)
            {
                this->choosingChildButton = "";
            }
            this->choosingButton = it.first;

            if (it.second->instantTrigger)
            {
                this->newStepTriggered = true;
            }
        }
    }
}

void Stack::updateChildButtons()
{
    if (this->choosingButton != "")
    {
        for (auto it : this->childButtons[this->choosingButton])
        {
            if (it.second->isPressed())
            {
                if (this->choosingChildButton != it.first)
                {
                }
                this->choosingChildButton = it.first;

                if (it.second->instantTrigger)
                {
                    this->newStepTriggered = true;
                }
                // this->triggerChildFunction[it.first];
            }
        }
    }
}

void Stack::updateInputFields()
{
    if (this->choosingButton != "" && this->choosingChildButton != "")
    {
        if (inputFields.find(choosingButton) != inputFields.end() &&
            inputFields[choosingButton].find(choosingChildButton) != inputFields[choosingButton].end())
        {
            this->inputFields[this->choosingButton][this->choosingChildButton]->update(mousePosView);
        }
    }
}

void Stack::updateSFMLEvents(const float &dt, sf::Event &event)
{
    // for Buttons
    for (auto &it : this->buttons)
    {
        it.second->updateSFMLEvents(dt, event, this->mousePosView);
    }

    // for Child Buttons
    if (this->choosingButton != "")
    {
        for (auto &it : this->childButtons[this->choosingButton])
        {
            it.second->updateSFMLEvents(dt, event, this->mousePosView);
        }
    }

    // for InputField
    if (this->choosingButton != "" && this->choosingChildButton != "")
    {
        if (inputFields.find(choosingButton) != inputFields.end() &&
            inputFields[choosingButton].find(choosingChildButton) != inputFields[choosingButton].end())
        {
            auto readingInput = [&]()
            {
                bool hitEnter = 0;
                std::string str;
                this->inputFields[this->choosingButton][this->choosingChildButton]->updateSFMLEvents(dt, event, str, hitEnter);

                std::function<std::pair<int, int>()> checkString = [&]()
                {
                    int x = -1, y = -1;
                    if (str.size() == 0)
                        return std::make_pair(x, y);

                    int numComma = 0, indexComma = -1;
                    for (int i = 0; i < (int)str.size(); ++i)
                    {
                        if (str[i] == ',')
                        {
                            indexComma = i;
                            ++numComma;
                        }
                    }
                    if (numComma > 1 || indexComma == 0 || indexComma == (int)str.size() - 1)
                        return std::make_pair(x, y);

                    if (numComma != 0)
                    {
                        std::string sx = str.substr(0, indexComma);
                        std::string sy = str.substr(indexComma + 1);
                        std::stringstream ss;
                        ss << sx;
                        ss >> x;
                        ss.clear();

                        ss << sy;
                        ss >> y;
                        ss.clear();
                    }
                    else
                    {
                        std::stringstream ss;
                        ss << str;
                        ss >> x;
                        ss.clear();
                    }
                    return std::make_pair(x, y);
                };

                if (!hitEnter)
                    return;
                hitEnter = 0;
                valueFirst = valueSecond = -1;

                std::pair<int, int> p = checkString();
                int x = p.first, y = p.second;
                if (x == -1)
                {
                    this->inputWarning.setString("Wrong input format");
                    hitEnter = 0;
                    return;
                }

                this->inputWarning.setString("");
                this->newStepTriggered = 1;
                this->valueFirst = x;
                this->valueSecond = y;
            };
            readingInput();
        }
    }
}

void Stack::update(const float &dt)
{   
    this->window->setTitle("Stack");
    this->updateMousePositions();
    this->updateInput(dt);
    this->updateTexts();

    this->updateCursor();
    this->updateButtons();
    this->updateChildButtons();
    this->updateInputFields();

    if (this->choosingButton != "")
        this->triggerFunction[this->choosingButton]();
}

void Stack::renderButtons(sf::RenderTarget *target)
{
    /* Render parent buttons */
    for (auto &it : this->buttons)
    {
        it.second->render(target);
    }

    /* Render child buttons */

    if (this->choosingButton != "")
    {
        for (auto &it : this->childButtons[this->choosingButton])
        {
            it.second->render(target);
        }
    }

    /* Render Input Fields */
    if (this->choosingButton != "" && this->choosingChildButton != "")
    {
        if (inputFields.find(choosingButton) != inputFields.end() &&
            inputFields[choosingButton].find(choosingChildButton) != inputFields[choosingButton].end())
        {
            this->inputFields[this->choosingButton][this->choosingChildButton]->render(target);
        }
    }
}

void Stack::renderNode(sf::RenderTarget *target)
{
    for (StackNode *node : this->Nodes)
    {
        node->render(target);
    }
}

void Stack::renderAnimation()
{
    if (this->doneAnimation)
    {
        this->runState = "";
        return;
    }

    auto enable_replayButton = [&]()
    {
        /* Show Replay button */
        this->buttons["Play"]->disabled = 1;
        this->buttons["Pause"]->disabled = 1;
        this->buttons["Replay"]->disabled = 0;
    };

    if (this->runState == "play")
    {
        this->animationStack->play(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "pause")
    {
        this->animationStack->pause(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "prev")
    {
        this->animationStack->prev(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "next")
    {
        this->animationStack->next(this->doneAnimation, &this->stepText);
        if (this->doneAnimation && this->animationStack->curIndex == (int)this->animationStack->instructions.size())
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "first")
    {
        this->animationStack->first(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "last")
    {
        this->animationStack->last(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }
}

void Stack::renderHighlightCode(sf::RenderTarget *target)
{
    codeHighlight->render(target);
}

void Stack::render(sf::RenderTarget *target)
{
    if (!target)
    {
        target = this->window;
    }
    target->draw(this->background);

    this->renderAnimation();
    this->renderHighlightCode(target);

    this->renderButtons(target);
    this->renderTexts(target);
    this->renderNode(target);
}

void Stack::reset()
{
    exist.assign((int)exist.size(), 0);
    this->DeleteNodePointers();
    this->head = nullptr;
    this->tail = nullptr;
    this->numberNode = 0;

    delete (this->animation);
    
    this->codeHighlight->reset();
    this->animation = new AnimationStack(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationStack = dynamic_cast<AnimationStack*>(this->animation);
    assert (animationStack != nullptr);

    this->newStepTriggered = 0;
    this->doneAnimation = 1;
    this->choosingButton = "";
    this->choosingChildButton = "";
    inputWarning.setString("");
    inputGuide.setString("");
    exist.assign((int)exist.size(), 0);
    runState = "";
}

void Stack::ReInitButtons()
{
    delete(this->buttons["Update"]); 
    delete(this->buttons["Search"]);
    delete(this->buttons["Delete"]);
    delete(this->buttons["Quit"]);

    this->buttons.erase(buttons.find("Update"));
    this->buttons.erase(buttons.find("Search"));


    this->buttons["Add"] = new Button(
            insButtonX, insButtonY + 2 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Push", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);


    this->buttons["Delete"] = new Button(
            insButtonX, insButtonY + 3 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Pop", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);
    this->buttons["Delete"]->instantTrigger = true;


    this->buttons["Clear"] = new Button(
            insButtonX, insButtonY + 4 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Clear", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);
    this->buttons["Clear"]->instantTrigger = true;


    this->buttons["Quit"] = new Button(
            insButtonX, insButtonY + 5 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Quit", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);
}