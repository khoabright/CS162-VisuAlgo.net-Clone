#include "SLL.h"

SLL::SLL(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states)
    : DataStructure(window, supportedKeys, states)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->exist.resize(105);

    auto SLLScale = [&]()
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
    SLLScale();

    this->initHighlightCode();
    this->initTriggerFunction();
    this->initChildButtons();
    this->initInputFields();
    this->initAnimation();
}

SLL::~SLL()
{
    // std::cout << "Ending SLL\n";
    this->DeleteNodePointers();
    delete (this->animationSLL);
}

void SLL::DeleteNodePointers()
{
    for (auto &v : this->Nodes)
    {
        delete (v);
    }
    this->Nodes.clear();
}

/* Init */
void SLL::initTriggerFunction()
{
    auto InstructionButtons = [&]()
    {
        this->triggerFunction["Theme"] = std::bind(&SLL::button_theme, this);
        this->triggerFunction["Initialize"] = std::bind(&SLL::button_initialize, this);
        this->triggerFunction["Add"] = std::bind(&SLL::button_add, this);
        this->triggerFunction["Delete"] = std::bind(&SLL::button_delete, this);
        this->triggerFunction["Update"] = std::bind(&SLL::button_update, this);
        this->triggerFunction["Search"] = std::bind(&SLL::button_search, this);
        this->triggerFunction["Quit"] = std::bind(&SLL::button_quit, this);
        this->triggerFunction["SpeedMinus"] = std::bind(&SLL::button_SpeedMinus, this);
        this->triggerFunction["SpeedPlus"] = std::bind(&SLL::button_SpeedPlus, this);
        this->triggerFunction["CodeSwitch"] = std::bind(&SLL::button_CodeSwitch, this);
    };

    auto RunButtons = [&]()
    {
        this->triggerFunction["First"] = std::bind(&SLL::button_first, this);
        this->triggerFunction["Prev"] = std::bind(&SLL::button_prev, this);
        this->triggerFunction["Pause"] = std::bind(&SLL::button_pause, this);
        this->triggerFunction["Play"] = std::bind(&SLL::button_play, this);
        this->triggerFunction["Replay"] = std::bind(&SLL::button_replay, this);
        this->triggerFunction["Next"] = std::bind(&SLL::button_next, this);
        this->triggerFunction["Last"] = std::bind(&SLL::button_last, this);
    };

    InstructionButtons();
    RunButtons();
}

void SLL::initChildButtons()
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

    auto init_add = [&]()
    {
        ++idy;
        this->childButtons["Add"].insert({"1Head", nullptr});
        this->childButtons["Add"].insert({"2Tail", nullptr});
        this->childButtons["Add"].insert({"3Middle", nullptr});

        int idx = 0;
        for (auto &it : this->childButtons["Add"])
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
    };

    auto init_delete = [&]()
    {
        ++idy;
        this->childButtons["Delete"].insert({"1Head", nullptr});
        this->childButtons["Delete"].insert({"2Tail", nullptr});
        this->childButtons["Delete"].insert({"3Middle", nullptr});

        int idx = 0;
        for (auto &it : this->childButtons["Delete"])
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
        this->childButtons["Delete"]["1Head"]->instantTrigger = true;
        this->childButtons["Delete"]["2Tail"]->instantTrigger = true;
    };

    auto init_update = [&]() {
    };

    auto init_search = [&]() {
    };

    init_theme();
    init_initialize();
    init_add();
    init_delete();
    init_update();
    init_search();
}

void SLL::initInputFields()
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
        ++idy;
        this->inputFields["Add"].insert({"1Head", nullptr});
        this->inputFields["Add"].insert({"2Tail", nullptr});
        this->inputFields["Add"].insert({"3Middle", nullptr});

        int idx = 0;
        for (auto &it : this->inputFields["Add"])
        {
            it.second = new InputField(this->inputFieldOriginX + (idx++) * (this->inputFieldWidth + this->inputFieldDistanceX),
                                       this->inputFieldOriginY + idy * (this->inputFieldHeight + this->inputFieldDistanceY),
                                       this->inputFieldWidth, this->inputFieldHeight, this->scale_x, this->scale_y,
                                       &this->font, this->inputFieldCharacterSize,
                                       sf::Color::White, sf::Color::White, sf::Color::Black);
        }
    };

    auto init_delete = [&]()
    {
        ++idy;
        this->inputFields["Delete"].insert({"1Head", nullptr});
        this->inputFields["Delete"].insert({"2Tail", nullptr});
        this->inputFields["Delete"].insert({"3Middle", nullptr});

        int idx = 0;
        for (auto &it : this->inputFields["Delete"])
        {
            it.second = new InputField(this->inputFieldOriginX + (idx++) * (this->inputFieldWidth + this->inputFieldDistanceX),
                                       this->inputFieldOriginY + idy * (this->inputFieldHeight + this->inputFieldDistanceY),
                                       this->inputFieldWidth, this->inputFieldHeight, this->scale_x, this->scale_y,
                                       &this->font, this->inputFieldCharacterSize,
                                       sf::Color::White, sf::Color::White, sf::Color::Black);
        }
    };

    auto init_update = [&]()
    {
        this->inputFields["Update"]["1Update"] = new InputField(this->inputFieldOriginX, 663.f * this->scale_y,
                                                                this->inputFieldWidth, this->inputFieldHeight, this->scale_x, this->scale_y,
                                                                &this->font, this->inputFieldCharacterSize,
                                                                sf::Color::White, sf::Color::White, sf::Color::Black);
    };

    auto init_search = [&]()
    {
        this->inputFields["Search"]["1Search"] = new InputField(this->inputFieldOriginX, 693.f * this->scale_y,
                                                                this->inputFieldWidth, this->inputFieldHeight, this->scale_x, this->scale_y,
                                                                &this->font, this->inputFieldCharacterSize,
                                                                sf::Color::White, sf::Color::White, sf::Color::Black);
    };

    init_initialize();
    init_add();
    init_delete();
    init_update();
    init_search();
}

void SLL::initAnimation()
{
    this->animation = new AnimationSLL(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationSLL = dynamic_cast<AnimationSLL*>(this->animation);
    assert (animationSLL != nullptr);
    this->animationSLL->animationTime = speedGap * (float)maxSpeed;

}

void SLL::initHighlightCode()
{
    codeHighlight = new CodeHighlight(scale_x, scale_y, &Colors);
}

void SLL::button_theme()
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
void SLL::button_initialize()
{
    auto createNewNode = [&](int newValue)
    {
        if (this->head == nullptr)
        {
            this->head = new SLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, 12, &this->font, &this->Colors);
            this->Nodes.push_back(this->head);
            ++this->numberNode;

            this->head->val = newValue;
            this->tail = head;
            return;
        }
        /* Add to tail */
        SLLNode *newNode = new SLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, 12, &this->font, &this->Colors);
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
        this->animationSLL->updateNodePosition(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }

    if (this->choosingChildButton == "3Load File")
    {
        if (!this->newStepTriggered)
            return;
        this->newStepTriggered = 0;

        this->reset();
        std::ifstream inp;
        inp.open("InputFiles/inputSLL.txt");
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

        this->animationSLL->updateNodePosition(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }
}

void SLL::button_add()
{
    if (this->choosingChildButton == "1Head")
    {
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

    if (this->choosingChildButton == "2Tail")
    {
        if (this->newStepTriggered)
        {
            this->newStepTriggered = 0;
            this->inputGuide.setString("");
            this->addTail(this->valueFirst);
            return;
        }
        else
        {
            this->inputGuide.setString("Input a value");
        }
    }

    if (this->choosingChildButton == "3Middle")
    {
        if (this->newStepTriggered)
        {
            this->newStepTriggered = 0;
            this->inputGuide.setString("");
            this->addMiddle(this->valueFirst, this->valueSecond);
            return;
        }
        else
        {
            this->inputGuide.setString("Input format: [index],[value]");
        }
    }
}

void SLL::button_delete()
{

    if (this->choosingChildButton == "1Head")
    {
        if (this->newStepTriggered)
        {
            this->newStepTriggered = 0;
            this->inputGuide.setString("");
            this->deleteHead();
            return;
        }
        else
        {
            this->inputGuide.setString("");
        }
    }

    if (this->choosingChildButton == "2Tail")
    {
        if (this->newStepTriggered)
        {
            this->newStepTriggered = 0;
            this->inputGuide.setString("");
            this->deleteTail();
            return;
        }
        else
        {
            this->inputGuide.setString("");
        }
    }

    if (this->choosingChildButton == "3Middle")
    {
        if (this->newStepTriggered)
        {
            this->newStepTriggered = 0;
            this->inputGuide.setString("");
            this->deleteMiddle(this->valueFirst);
            return;
        }
        else
        {
            this->inputGuide.setString("Input an index");
        }
    }
}

void SLL::button_update()
{
    this->choosingChildButton = "1Update";

    if (this->newStepTriggered)
    {
        this->newStepTriggered = 0;
        this->inputGuide.setString("");
        this->updateNode(this->valueFirst, this->valueSecond);
        return;
    }
    else
    {
        this->inputGuide.setString("Input form: [index],[value]");
    }
}

void SLL::button_search()
{
    this->choosingChildButton = "1Search";

    if (this->newStepTriggered)
    {
        this->newStepTriggered = 0;
        this->inputGuide.setString("");
        this->searchNode(this->valueFirst);
        return;
    }
    else
    {
        this->inputGuide.setString("Input a value to search");
    }
}

void SLL::button_quit()
{
    this->endState();
}

/* SLL operations */
// Add
void SLL::addHead(int nodeValue)
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
        this->head = new SLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
        this->head->isNewNode = 1;
        this->Nodes.push_back(this->head);

        this->tail = this->head;

        this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->showNode(this->head, "newNode", this->numberNode, {0}); }});
        this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->changeNodeLabel(this->head, "head", {1}); }});
        this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->changeNodeLabel(this->head, "head/tail", {2}); }});

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
    SLLNode *newNode = new SLLNode(xHead, yHead + this->nodeDistanceY,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;

    this->Nodes.push_back(newNode);

    this->animationSLL->instructions.push_back({[this, newNode]()
                                             { this->animationSLL->showNode(newNode, "newNode", this->numberNode, {0}); }});

    /* Connect Node to Head*/
    this->animationSLL->instructions.push_back({[this, newNode]()
                                             { this->animationSLL->connectNodes(newNode, this->head, {1}); }});

    /* Change head to newNode */
    SLLNode *old_head = this->head;
    this->animationSLL->instructions.push_back({[this, newNode, old_head]()
                                             { this->animationSLL->changeHead(this->head, old_head, newNode, {2}); }});

    /* Relayout */
    this->animationSLL->instructions.push_back({[this]()
                                             { this->animationSLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {3}); }});
}

void SLL::addTail(int nodeValue)
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
        this->head = new SLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
        this->head->isNewNode = 1;
        this->Nodes.push_back(this->head);

        this->tail = head;

        this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->showNode(this->head, "newNode", this->numberNode, {0}); }});
        this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->changeNodeLabel(this->head, "head", {1}); }});
        this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->changeNodeLabel(this->head, "head/tail", {2}); }});

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
    SLLNode *newNode = new SLLNode(xTail + this->nodeDistanceX, yTail,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;
    this->Nodes.push_back(newNode);

    this->animationSLL->instructions.push_back({[this, newNode]()
                                             { this->animationSLL->showNode(newNode, "newNode", this->numberNode, {0}); }});

    /* connect Tail to newNode*/
    this->animationSLL->instructions.push_back({[this, newNode]()
                                             { this->animationSLL->connectNodes(this->tail, newNode, {1}); }});

    /* change Tail to newNode */
    SLLNode *old_tail = this->tail;
    this->animationSLL->instructions.push_back({[this, newNode, old_tail]()
                                             { this->animationSLL->changeTail(this->tail, old_tail, newNode, {2}); }});

    /* Relayout */
    this->animationSLL->instructions.push_back({[this]()
                                             { this->animationSLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {3}); }});
}

void SLL::addMiddle(int index, int nodeValue)
{   
    /* Complete all animations */
    this->prepareNewInstruction();
    this->button_play();

    if (index == -1 || nodeValue == -1)
    {
        this->inputWarning.setString("Wrong input format");
        return;
    }
    if (nodeValue < 1 || nodeValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }
    if (index < 1 || index > this->numberNode - 2)
    {
        this->inputWarning.setString("Index should be in range [1.." + std::to_string(this->numberNode - 2) + "]");
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


    if (index > this->numberNode)
    {
        return;
    }

    if (index == 0)
    {
        this->addHead(nodeValue);
        return;
    }
    if (index == this->numberNode)
    {
        this->addTail(nodeValue);
        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to index " + std::to_string(index));
        codeHighlight->codeStrings.push_back("Node cur = head");                      // 0
        codeHighlight->codeStrings.push_back("for (i = 0; i < index - 1; ++i)");      // 1
        codeHighlight->codeStrings.push_back("   cur = cur.next");                    // 2
        codeHighlight->codeStrings.push_back("Node nxt = cur.next");                  // 3
        codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");              // 4
        codeHighlight->codeStrings.push_back("newNode.next = nxt");                   // 5
        codeHighlight->codeStrings.push_back("cur.next = newNode");                   // 6
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 7
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    /* Highlight first node  */
    SLLNode *cur = this->head;

    this->animationSLL->instructions.push_back(
        {[this]()
         {
             this->animationSLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {0});
         }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationSLL->instructions.push_back(
            {[this, cur, passedLabel, currentLabel]()
                                                 {
                                                     this->animationSLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2});
                                                 }});
        cur = cur->next;
    }

    this->animationSLL->instructions.push_back(
        {[this, cur, index]()
         {
             this->animationSLL->highlightPassedNode(cur, cur->labelString, "nxt/" + std::to_string(index), "passedColor", "reachColor", {3});
         }});

    /* Create new Node */
    SLLNode *newNode = new SLLNode(cur->next->x, cur->next->y + this->nodeDistanceY,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;
    this->Nodes.push_back(newNode);
    this->animationSLL->instructions.push_back({[this, newNode]()
                                             { this->animationSLL->showNode(newNode, "newNode", this->numberNode, {4}); }});

    /* Connect newNode to cur->next */
    this->animationSLL->instructions.push_back({[this, newNode, cur]()
                                             { this->animationSLL->connectNodes(newNode, cur->next, {5}); }});

    // Connect cur to new Node
    this->animationSLL->instructions.push_back({[this, cur, newNode]()
                                             { this->animationSLL->connectNodes(cur, newNode, {6}); }});

    // Relayout
    this->animationSLL->instructions.push_back({[this]()
                                             { this->animationSLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {7}); }});
}

// Delete
void SLL::deleteHead()
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
        this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->addCodeLines({0}); }});

        return;
    }

    /* Highlight current Node */
    SLLNode *old_head = this->head;
    this->animationSLL->instructions.push_back({[this, old_head]()
                                             { this->animationSLL->highlightCurrentNode(old_head, "target", "deleteColor", {1}); }});

    if (this->numberNode > 1)
    {
        /* Change head to head->next */
        SLLNode *new_head = this->head->next;
        this->animationSLL->instructions.push_back({[this, old_head, new_head]()
                                                 { this->animationSLL->changeHead(this->head, old_head, new_head, {2}); }});
    }
    else
    {
        this->head = nullptr;
        this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->addCodeLines({2}); }});
    }

    /* Delete head */
    this->exist[old_head->val] = 0;
    this->animationSLL->instructions.push_back({[this, old_head]()
                                             { this->animationSLL->hideNode(old_head, this->numberNode, {3}); }});
    /* Relayout */
    this->animationSLL->instructions.push_back({[this]()
                                             { this->animationSLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {4}); }});

    return;
}

void SLL::deleteTail()
{      
    this->prepareNewInstruction();
    this->button_play();

    if (numberNode < 2) {deleteHead(); return;}

    auto addHighlightCodes = [&] {
        codeHighlight->introText.setString("Delete tail");
        codeHighlight->codeStrings.push_back("if empty, do nothing"); // 0
        codeHighlight->codeStrings.push_back("Node cur = head"); // 1
        codeHighlight->codeStrings.push_back("while (cur.next != tail)"); // 2
        codeHighlight->codeStrings.push_back("   cur = cur.next"); // 3
        codeHighlight->codeStrings.push_back("Node target = cur.next"); // 4
        codeHighlight->codeStrings.push_back("tail = cur"); // 5
        codeHighlight->codeStrings.push_back("delete target"); // 6
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 7
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    assert(this->head != nullptr);

    /* Highlight first node  */
    SLLNode *cur = this->head;

    this->animationSLL->instructions.push_back(
        {[this]()
         {
             this->animationSLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {1});
         }});

    int index = this->numberNode - 1;
    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationSLL->instructions.push_back(
            {[this, cur, passedLabel, currentLabel]()
             {
                 this->animationSLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {2, 3});
             }});
        cur = cur->next;
    }

    /* Highlight target Node */
    this->animationSLL->instructions.push_back(
        {[this, cur, index]()
         {
             this->animationSLL->highlightPassedNode(cur, cur->labelString, "target/" + std::to_string(index), "passedColor", "deleteColor", {4});
         }});

    SLLNode *targetNode = cur->next;

    /* Change tail to cur */
    this->animationSLL->instructions.push_back({[this, targetNode, cur]()
                                             {
                                                 this->animationSLL->changeTail(this->tail, targetNode, cur, {5});
                                             }});

    /* Delete targetNode */
    this->exist[targetNode->val] = 0;
    this->animationSLL->instructions.push_back({[this, targetNode]()
                                             {
                                                 this->animationSLL->hideNode(targetNode, this->numberNode, {6});
                                             },
                                             [this, cur]()
                                             {
                                                 /* Disconnect cur from targetNode */
                                                 this->animationSLL->connectNodes(cur, nullptr, {});
                                             }});

    /* Relayout */
    this->animationSLL->instructions.push_back({[this]()
                                             { this->animationSLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {7}); }});

    return;
}

void SLL::deleteMiddle(int index)
{   
    this->prepareNewInstruction();
    this->button_play();

    if (index < 1 || index > this->numberNode - 2)
    {
        this->inputWarning.setString("Index should be in range [1.." + std::to_string(this->numberNode - 2) + "]");
        return;
    }


    if (index == 0)
    {
        this->deleteHead();
        return;
    }
    if (index == this->numberNode)
    {
        this->deleteTail();
        return;
    }

    auto addHighlightCodes = [&] {
        codeHighlight->introText.setString("Delete index" + std::to_string(index));
        codeHighlight->codeStrings.push_back("Node cur = head"); // 0
        codeHighlight->codeStrings.push_back("for (i = 0; i < index - 1; ++i)"); // 1
        codeHighlight->codeStrings.push_back("   cur = cur.next"); // 2
        codeHighlight->codeStrings.push_back("Node target = cur.next"); // 3
        codeHighlight->codeStrings.push_back("Node nxt = target.next"); // 4
        codeHighlight->codeStrings.push_back("cur.next = nxt"); // 5
        codeHighlight->codeStrings.push_back("delete target"); // 6
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 7
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    /* Highlight first node  */
    SLLNode *cur = this->head;

    this->animationSLL->instructions.push_back({[this]()
                                             {
                                                 this->animationSLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {0});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationSLL->instructions.push_back(
            {[this, cur, passedLabel, currentLabel]()
             {
                 this->animationSLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2});
             }});
        cur = cur->next;
    }

    /* Highlight target Node */
    this->animationSLL->instructions.push_back(
        {[this, cur, index]()
         {
             this->animationSLL->highlightPassedNode(cur, cur->labelString, "target/" + std::to_string(index), "passedColor", "deleteColor", {3});
         }});

    /* Highlight target->next */
    SLLNode *nxt = cur->next->next;
    this->animationSLL->instructions.push_back({[this, nxt, index]()
                                             {
                                                 this->animationSLL->highlightCurrentNode(nxt, "nxt/" + std::to_string(index + 1), "reachColor", {4});
                                             }});

    SLLNode *targetNode = cur->next;

    /* Connect cur-----cur->next->next ; simultaneously Move targetNode to another position */
    this->animationSLL->instructions.push_back({[this, targetNode]()
                                             {
                                                 this->animationSLL->moveNode(targetNode, targetNode->x, targetNode->y + this->nodeDistanceY);
                                             },
                                             [this, targetNode]()
                                             {
                                                 this->animationSLL->makeArrow(&targetNode->shape, &targetNode->next->shape, &targetNode->arrow);
                                             },
                                             [this, cur]()
                                             {
                                                 this->animationSLL->connectNodes(cur, cur->next->next, {5});
                                             }});

    /* Delete targetNode */
    this->exist[targetNode->val] = 0;
    this->animationSLL->instructions.push_back({[this, targetNode]()
                                             {
                                                 this->animationSLL->hideNode(targetNode, this->numberNode, {6});
                                             }});

    /* Relayout */
    this->animationSLL->instructions.push_back({[this]()
                                             { this->animationSLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {7}); }});

    return;
}

void SLL::updateNode(int index, int newValue)
{   
    this->prepareNewInstruction();
    this->button_play();

    if (index == -1 || newValue == -1)
    {
        this->inputWarning.setString("Wrong input format");
        return;
    }
    if (numberNode == 0)
    {
        this->inputWarning.setString("Cannot update empty list");
        return;
    }
    if (newValue < 1 || newValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }

    if (index < 0 || index >= this->numberNode)
    {
        this->inputWarning.setString("Index should be in range [0.." + std::to_string(this->numberNode - 1) + "]");
        return;
    }
    if (this->exist[newValue])
    {
        this->inputWarning.setString(std::to_string(newValue) + " is already existed");
        return;
    }


    auto addHighlightCodes = [&] {
        codeHighlight->introText.setString("Update node at index " + std::to_string(index) + " to " + std::to_string(newValue));
        codeHighlight->codeStrings.push_back("Node cur = head"); // 0
        codeHighlight->codeStrings.push_back("for (i = 0; i < index - 1; ++i)"); // 1
        codeHighlight->codeStrings.push_back("   cur = cur.next"); // 2
        codeHighlight->codeStrings.push_back("Node target = cur.next"); // 3
        codeHighlight->codeStrings.push_back("target.val = newVal"); // 4
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    if (index == 0)
    {
        SLLNode *targetNode = head;

        this->animationSLL->instructions.push_back({[this, targetNode]()
                                                 {
                                                     this->animationSLL->highlightCurrentNode(targetNode, "target/0", "reachColor", {});
                                                 }});

        /* Change targetNode to newValue */
        this->animationSLL->instructions.push_back({[this, targetNode, newValue]()
                                                 {
                                                     this->animationSLL->changeNodeValue(targetNode, newValue, {});
                                                 }});

        this->exist[targetNode->val] = 0;
        this->exist[newValue] = 1;

        return;
    }

    /* Highlight first node  */
    SLLNode *cur = this->head;

    this->animationSLL->instructions.push_back({[this]()
                                             {
                                                 this->animationSLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {0});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationSLL->instructions.push_back({[this, cur, passedLabel, currentLabel]()
                                                 {
                                                     this->animationSLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2});
                                                 }});
        cur = cur->next;
    }

    /* Highlight target Node */
    this->animationSLL->instructions.push_back({[this, cur, index]()
                                             {
                                                 this->animationSLL->highlightPassedNode(cur, cur->labelString, "target/" + std::to_string(index), "passedColor", "reachColor", {3});
                                             }});

    SLLNode *targetNode = cur->next;
    /* Change targetNode to newValue */
    this->animationSLL->instructions.push_back({[this, targetNode, newValue]()
                                             {
                                                 this->animationSLL->changeNodeValue(targetNode, newValue, {4});
                                             }});

    this->exist[targetNode->val] = 0;
    this->exist[newValue] = 1;
}

void SLL::searchNode(int searchValue)
{
    this->prepareNewInstruction();
    this->button_play();

    if (searchValue < 1 || searchValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }

    if (this->head == nullptr)
    {
        this->inputWarning.setString("Empty list");
        return;
    }

    auto addHighlightCodes = [&] {
        codeHighlight->introText.setString("Search node with value " + std::to_string(searchValue));
        codeHighlight->codeStrings.push_back("Node cur = head, index = 0"); // 0
        codeHighlight->codeStrings.push_back("while (cur != nullptr && cur.val != v)"); // 1
        codeHighlight->codeStrings.push_back("   ++index, cur = cur.next"); // 2
        codeHighlight->codeStrings.push_back("   if (cur == nullptr)"); // 3
        codeHighlight->codeStrings.push_back("      return NOT_FOUND"); // 4
        codeHighlight->codeStrings.push_back("return index"); // 5
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    /* Highlight first node  */
    SLLNode *cur = this->head;

    this->animationSLL->instructions.push_back({[this]()
                                             {
                                                 this->animationSLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {0});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < this->numberNode; ++i)
    {
        if (cur->val == searchValue)
        {
            this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->addCodeLines({5}); }});
            return;
        }

        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationSLL->instructions.push_back({[this, cur, passedLabel, currentLabel]()
                                                 {
                                                     this->animationSLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2, 3});
                                                 }});

        cur = cur->next;
    }

    //Not Found
    this->animationSLL->instructions.push_back({[this]()
                                                 { this->animationSLL->addCodeLines({4}); }});
}

void SLL::prepareNewInstruction()
{   
    this->animation->finishStep();
    bool trash = 0;
    this->animationSLL->last(trash, &this->stepText);
    this->animationSLL->newInstruction(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
    this->codeHighlight->reset();
}

/* Update and Render */
void SLL::updateButtons()
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

void SLL::updateChildButtons()
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

void SLL::updateInputFields()
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

void SLL::updateSFMLEvents(const float &dt, sf::Event &event)
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

                // if (!this->doneAnimation)
                // {
                //     this->inputWarning.setString("Sorry, the animationSLL is running...");
                //     return;
                // }
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

void SLL::update(const float &dt)
{   
    this->window->setTitle("Singly Linked List");
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

void SLL::renderButtons(sf::RenderTarget *target)
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

void SLL::renderNode(sf::RenderTarget *target)
{
    for (SLLNode *node : this->Nodes)
    {
        node->render(target);
    }
}

void SLL::renderAnimation()
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
        this->animationSLL->play(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "pause")
    {
        this->animationSLL->pause(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "prev")
    {
        this->animationSLL->prev(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "next")
    {
        this->animationSLL->next(this->doneAnimation, &this->stepText);
        if (this->doneAnimation && this->animationSLL->curIndex == (int)this->animationSLL->instructions.size())
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "first")
    {
        this->animationSLL->first(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "last")
    {
        this->animationSLL->last(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }
}

void SLL::renderHighlightCode(sf::RenderTarget *target)
{
    codeHighlight->render(target);
}

void SLL::render(sf::RenderTarget *target)
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

    // REMOVE LATER!!!
    // sf::Text mouseText;
    // mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
    // mouseText.setFont(this->font);
    // mouseText.setCharacterSize(20);
    // std::stringstream ss;
    // ss << this->mousePosView.x << " " << this->mousePosView.y;
    // mouseText.setString(ss.str());

    // target->draw(mouseText);
}

void SLL::reset()
{
    exist.assign((int)exist.size(), 0);
    this->DeleteNodePointers();
    this->head = nullptr;
    this->tail = nullptr;
    this->numberNode = 0;

    delete (this->animation);
    
    this->codeHighlight->reset();
    this->animation = new AnimationSLL(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationSLL = dynamic_cast<AnimationSLL*>(this->animation);
    assert (animationSLL != nullptr);

    this->newStepTriggered = 0;
    this->doneAnimation = 1;
    this->choosingButton = "";
    this->choosingChildButton = "";
    inputWarning.setString("");
    inputGuide.setString("");
    exist.assign((int)exist.size(), 0);
    runState = "";
}
