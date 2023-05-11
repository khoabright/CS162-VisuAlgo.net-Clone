#include "DLL.h"

DLL::DLL(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states)
    : DataStructure(window, supportedKeys, states)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->exist.resize(105);

    auto DLLScale = [&]()
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
    DLLScale();

    this->initHighlightCode();
    this->initTriggerFunction();
    this->initChildButtons();
    this->initInputFields();
    this->initAnimation();
}

DLL::~DLL()
{
    // std::cout << "Ending DLL\n";
    this->DeleteNodePointers();
    delete (this->animationDLL);
}

void DLL::DeleteNodePointers()
{
    for (auto &v : this->Nodes)
    {
        delete (v);
    }
    this->Nodes.clear();
}

/* Init */
void DLL::initTriggerFunction()
{
    auto InstructionButtons = [&]()
    {
        this->triggerFunction["Theme"] = std::bind(&DLL::button_theme, this);
        this->triggerFunction["Initialize"] = std::bind(&DLL::button_initialize, this);
        this->triggerFunction["Add"] = std::bind(&DLL::button_add, this);
        this->triggerFunction["Delete"] = std::bind(&DLL::button_delete, this);
        this->triggerFunction["Update"] = std::bind(&DLL::button_update, this);
        this->triggerFunction["Search"] = std::bind(&DLL::button_search, this);
        this->triggerFunction["Quit"] = std::bind(&DLL::button_quit, this);
        this->triggerFunction["SpeedMinus"] = std::bind(&DLL::button_SpeedMinus, this);
        this->triggerFunction["SpeedPlus"] = std::bind(&DLL::button_SpeedPlus, this);
        this->triggerFunction["CodeSwitch"] = std::bind(&DLL::button_CodeSwitch, this);
    };

    auto RunButtons = [&]()
    {
        this->triggerFunction["First"] = std::bind(&DLL::button_first, this);
        this->triggerFunction["Prev"] = std::bind(&DLL::button_prev, this);
        this->triggerFunction["Pause"] = std::bind(&DLL::button_pause, this);
        this->triggerFunction["Play"] = std::bind(&DLL::button_play, this);
        this->triggerFunction["Replay"] = std::bind(&DLL::button_replay, this);
        this->triggerFunction["Next"] = std::bind(&DLL::button_next, this);
        this->triggerFunction["Last"] = std::bind(&DLL::button_last, this);
    };

    InstructionButtons();
    RunButtons();
}

void DLL::initChildButtons()
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

void DLL::initInputFields()
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

void DLL::initAnimation()
{
    this->animation = new AnimationDLL(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationDLL = dynamic_cast<AnimationDLL*>(this->animation);
    this->animationDLL->animationTime = speedGap * (float)maxSpeed;
}

void DLL::initHighlightCode()
{
    codeHighlight = new CodeHighlight(scale_x, scale_y, &Colors);
}

void DLL::button_theme()
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
void DLL::button_initialize()
{
    auto createNewNode = [&](int newValue)
    {
        if (this->head == nullptr)
        {
            this->head = new DLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, 12, &this->font, &this->Colors);
            this->Nodes.push_back(this->head);
            ++this->numberNode;

            this->head->val = newValue;
            this->tail = head;
            return;
        }
        /* Add to tail */
        DLLNode *newNode = new DLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, 12, &this->font, &this->Colors);
        this->Nodes.push_back(newNode);
        ++this->numberNode;

        newNode->val = newValue;
        this->tail->next = newNode;
        newNode->prev = tail;
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
        this->animationDLL->updateNodePosition(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }

    if (this->choosingChildButton == "3Load File")
    {
        if (!this->newStepTriggered)
            return;
        this->newStepTriggered = 0;

        this->reset();
        std::ifstream inp;
        inp.open("InputFiles/inputDLL.txt");
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

        this->animationDLL->updateNodePosition(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }
}

void DLL::button_add()
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

void DLL::button_delete()
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

void DLL::button_update()
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

void DLL::button_search()
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

void DLL::button_quit()
{
    this->endState();
}

/* DLL operations */
// Add
void DLL::addHead(int nodeValue)
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
        this->head = new DLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
        this->head->isNewNode = 1;
        this->Nodes.push_back(this->head);

        this->tail = this->head;

        this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->showNode(this->head, "newNode", this->numberNode, {0}); }});
        this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->changeNodeLabel(this->head, "head", {1}); }});
        this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->changeNodeLabel(this->head, "head/tail", {2}); }});

        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to head");
        codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");           // 0
        codeHighlight->codeStrings.push_back("newNode.next = head");                  // 1
        codeHighlight->codeStrings.push_back("head.prev = newNode");                  // 2
        codeHighlight->codeStrings.push_back("head = newNode");                       // 3
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 4
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    float xHead = this->head->x;
    float yHead = this->head->y;

    /* Create and Show Node */
    DLLNode *newNode = new DLLNode(xHead, yHead + this->nodeDistanceY,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;

    this->Nodes.push_back(newNode);

    this->animationDLL->instructions.push_back({[this, newNode]()
                                             { this->animationDLL->showNode(newNode, "newNode", this->numberNode, {0}); }});

    /* ConnectNext Node to Head*/
    this->animationDLL->instructions.push_back({[this, newNode]()
                                             { this->animationDLL->connectNext(newNode, this->head, {1}); }});

    /* ConnectPrev Head to Node*/
    this->animationDLL->instructions.push_back({[this, newNode]()
                                             { this->animationDLL->connectPrev(this->head, newNode, {2}); }});

    /* Change head to newNode */
    DLLNode *old_head = this->head;
    this->animationDLL->instructions.push_back({[this, newNode, old_head]()
                                             { this->animationDLL->changeHead(this->head, old_head, newNode, {3}); }});

    /* Relayout */
    this->animationDLL->instructions.push_back({[this]()
                                             { this->animationDLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {4}); }});
}

void DLL::addTail(int nodeValue)
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
        this->head = new DLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
        this->head->isNewNode = 1;
        this->Nodes.push_back(this->head);

        this->tail = head;

        this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->showNode(this->head, "newNode", this->numberNode, {0}); }});
        this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->changeNodeLabel(this->head, "head", {1}); }});
        this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->changeNodeLabel(this->head, "head/tail", {2}); }});

        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to tail");
        codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");           // 0
        codeHighlight->codeStrings.push_back("tail.next = newNode");                  // 1
        codeHighlight->codeStrings.push_back("newNode.prev = tail");                  // 2
        codeHighlight->codeStrings.push_back("tail = newNode");                       // 3
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 4
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    float xTail = this->tail->x;
    float yTail = this->tail->y;

    /* Create and Show Node */
    DLLNode *newNode = new DLLNode(xTail + this->nodeDistanceX, yTail,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;
    this->Nodes.push_back(newNode);

    this->animationDLL->instructions.push_back({[this, newNode]()
                                             { this->animationDLL->showNode(newNode, "newNode", this->numberNode, {0}); }});

    /* connectNext Tail to newNode*/
    this->animationDLL->instructions.push_back({[this, newNode]()
                                             { this->animationDLL->connectNext(this->tail, newNode, {1}); }});

    /* connectPrev newNode to Tail*/
    this->animationDLL->instructions.push_back({[this, newNode]()
                                             { this->animationDLL->connectPrev(newNode, this->tail, {2}); }});

    /* change Tail to newNode */
    DLLNode *old_tail = this->tail;
    this->animationDLL->instructions.push_back({[this, newNode, old_tail]()
                                             { this->animationDLL->changeTail(this->tail, old_tail, newNode, {3}); }});

    /* Relayout */
    this->animationDLL->instructions.push_back({[this]()
                                             { this->animationDLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {4}); }});
}

void DLL::addMiddle(int index, int nodeValue)
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
        codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");           // 4
        codeHighlight->codeStrings.push_back("newNode.next = nxt");                   // 5
        codeHighlight->codeStrings.push_back("nxt.prev = newNode");                   // 6
        codeHighlight->codeStrings.push_back("cur.next = newNode");                   // 7
        codeHighlight->codeStrings.push_back("newNode.prev = cur");                   // 8
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 9
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    /* Highlight first node  */
    DLLNode *cur = this->head;

    this->animationDLL->instructions.push_back(
        {[this]()
         {
             this->animationDLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {0});
         }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationDLL->instructions.push_back(
            {[this, cur, passedLabel, currentLabel]()
                                                 {
                                                     this->animationDLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2});
                                                 }});
        cur = cur->next;
    }

    this->animationDLL->instructions.push_back(
        {[this, cur, index]()
         {
             this->animationDLL->highlightPassedNode(cur, cur->labelString, "nxt/" + std::to_string(index), "passedColor", "reachColor", {3});
         }});

    /* Create new Node */
    DLLNode *newNode = new DLLNode(cur->next->x, cur->next->y + this->nodeDistanceY,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;
    this->Nodes.push_back(newNode);
    this->animationDLL->instructions.push_back({[this, newNode]()
                                             { this->animationDLL->showNode(newNode, "newNode", this->numberNode, {4}); }});

    /* ConnectNext newNode to cur->next */
    this->animationDLL->instructions.push_back({[this, newNode, cur]()
                                             { this->animationDLL->connectNext(newNode, cur->next, {5}); }});

    /* ConnectPrev cur->next to newNode */
    this->animationDLL->instructions.push_back({[this, newNode, cur]()
                                             { this->animationDLL->connectPrev(cur->next, newNode, {6}); }});

    // ConnectNext cur to newNode
    this->animationDLL->instructions.push_back({[this, cur, newNode]()
                                             { this->animationDLL->connectNext(cur, newNode, {7}); }});

    // ConnectPrev newNode to cur
    this->animationDLL->instructions.push_back({[this, cur, newNode]()
                                             { this->animationDLL->connectPrev(newNode, cur, {8}); }});

    // Relayout
    this->animationDLL->instructions.push_back({[this]()
                                             { this->animationDLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {9}); }});
}

// Delete
void DLL::deleteHead()
{
    this->prepareNewInstruction();
    this->button_play();

    auto addHighlightCodes = [&] {
        codeHighlight->introText.setString("Delete head");
        codeHighlight->codeStrings.push_back("if empty, do nothing");                 // 0
        codeHighlight->codeStrings.push_back("Node target = head");                   // 1
        codeHighlight->codeStrings.push_back("Node head = head.next");                // 2
        codeHighlight->codeStrings.push_back("delete target");                        // 3
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 4
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    if (this->head == nullptr) {
        this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->addCodeLines({0}); }});

        return;
    }

    /* Highlight current Node */
    DLLNode *old_head = this->head;
    this->animationDLL->instructions.push_back({[this, old_head]()
                                             { this->animationDLL->highlightCurrentNode(old_head, "target", "deleteColor", {1}); }});

    if (this->numberNode > 1)
    {
        /* Change head to head->next */
        DLLNode *new_head = this->head->next;
        this->animationDLL->instructions.push_back({[this, old_head, new_head]()
                                                 { this->animationDLL->changeHead(this->head, old_head, new_head, {2}); }});
    }
    else
    {
        this->head = nullptr;
        this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->addCodeLines({2}); }});
    }

    /* Delete head */
    this->exist[old_head->val] = 0;
    this->animationDLL->instructions.push_back({[this, old_head]()
                                             { this->animationDLL->hideNode(old_head, this->numberNode, {3}); }});
    /* Relayout */
    this->animationDLL->instructions.push_back({[this]()
                                             { this->animationDLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {4}); }});

    return;
}

void DLL::deleteTail()
{      
    this->prepareNewInstruction();
    this->button_play();

    if (numberNode < 2) {deleteHead(); return;}

    auto addHighlightCodes = [&] {
        codeHighlight->introText.setString("Delete tail");
        codeHighlight->codeStrings.push_back("if empty, do nothing");                 // 0
        codeHighlight->codeStrings.push_back("Node target = tail");                   // 1
        codeHighlight->codeStrings.push_back("tail = tail.prev");                     // 2
        codeHighlight->codeStrings.push_back("delete target");                        // 3
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 4
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    assert(this->head != nullptr);

    /* Highlight current Node */
    DLLNode *old_tail = this->tail;
    this->animationDLL->instructions.push_back({[this, old_tail]()
                                             { this->animationDLL->highlightCurrentNode(old_tail, "target", "deleteColor", {1}); }});

    /* Change tail to tail->prev */
        DLLNode *new_tail = this->tail->prev;
        this->animationDLL->instructions.push_back({[this, old_tail, new_tail]()
                                                 { this->animationDLL->changeTail(this->tail, old_tail, new_tail, {2}); }});

    /* Delete tail */
    this->exist[old_tail->val] = 0;
    this->animationDLL->instructions.push_back({[this, old_tail]()
                                             { this->animationDLL->hideNode(old_tail, this->numberNode, {3}); }});
    /* Relayout */
    this->animationDLL->instructions.push_back({[this]()
                                             { this->animationDLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {4}); }});

    return;
}

void DLL::deleteMiddle(int index)
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
        codeHighlight->codeStrings.push_back("cur.next = nxt, nxt.prev = cur"); // 5
        codeHighlight->codeStrings.push_back("delete target"); // 6
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 7
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    /* Highlight first node  */
    DLLNode *cur = this->head;

    this->animationDLL->instructions.push_back({[this]()
                                             {
                                                 this->animationDLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {0});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationDLL->instructions.push_back(
            {[this, cur, passedLabel, currentLabel]()
             {
                 this->animationDLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2});
             }});
        cur = cur->next;
    }

    /* Highlight target Node */
    this->animationDLL->instructions.push_back(
        {[this, cur, index]()
         {
             this->animationDLL->highlightPassedNode(cur, cur->labelString, "target/" + std::to_string(index), "passedColor", "deleteColor", {3});
         }});

    /* Highlight target->next */
    DLLNode *nxt = cur->next->next;
    this->animationDLL->instructions.push_back({[this, nxt, index]()
                                             {
                                                 this->animationDLL->highlightCurrentNode(nxt, "nxt/" + std::to_string(index + 1), "reachColor", {4});
                                             }});

    DLLNode *targetNode = cur->next;

    /* ConnectNext cur-----cur->next->next ;
    ConnectPrev cur->next->next---cur,
    simultaneously Move targetNode to another position */
    this->animationDLL->instructions.push_back({[this, targetNode]()
                                             {
                                                 this->animationDLL->moveNode(targetNode, targetNode->x, targetNode->y + this->nodeDistanceY);
                                             },
                                             [this, targetNode]()
                                             {
                                                 this->animationDLL->makeArrow(&targetNode->shape, &targetNode->next->shape, &targetNode->arrowNext);
                                             },
                                             [this, targetNode]()
                                             {
                                                 this->animationDLL->makeArrow(&targetNode->shape, &targetNode->prev->shape, &targetNode->arrowPrev);
                                             },
                                             [this, nxt, cur]()
                                             {
                                                 this->animationDLL->connectPrev(nxt, cur, {5});
                                             },
                                             [this, nxt, cur]()
                                             {
                                                 this->animationDLL->connectNext(cur, nxt, {5});
                                             }});


    /* Delete targetNode */
    this->exist[targetNode->val] = 0;
    this->animationDLL->instructions.push_back({[this, targetNode]()
                                             {
                                                 this->animationDLL->hideNode(targetNode, this->numberNode, {6});
                                             }});

    /* Relayout */
    this->animationDLL->instructions.push_back({[this]()
                                             { this->animationDLL->Relayout((numberNode == 0), this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX, {7}); }});

    return;
}

void DLL::updateNode(int index, int newValue)
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
        DLLNode *targetNode = head;

        this->animationDLL->instructions.push_back({[this, targetNode]()
                                                 {
                                                     this->animationDLL->highlightCurrentNode(targetNode, "target/0", "reachColor", {});
                                                 }});

        /* Change targetNode to newValue */
        this->animationDLL->instructions.push_back({[this, targetNode, newValue]()
                                                 {
                                                     this->animationDLL->changeNodeValue(targetNode, newValue, {});
                                                 }});

        this->exist[targetNode->val] = 0;
        this->exist[newValue] = 1;

        return;
    }

    /* Highlight first node  */
    DLLNode *cur = this->head;

    this->animationDLL->instructions.push_back({[this]()
                                             {
                                                 this->animationDLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {0});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationDLL->instructions.push_back({[this, cur, passedLabel, currentLabel]()
                                                 {
                                                     this->animationDLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2});
                                                 }});
        cur = cur->next;
    }

    /* Highlight target Node */
    this->animationDLL->instructions.push_back({[this, cur, index]()
                                             {
                                                 this->animationDLL->highlightPassedNode(cur, cur->labelString, "target/" + std::to_string(index), "passedColor", "reachColor", {3});
                                             }});

    DLLNode *targetNode = cur->next;
    /* Change targetNode to newValue */
    this->animationDLL->instructions.push_back({[this, targetNode, newValue]()
                                             {
                                                 this->animationDLL->changeNodeValue(targetNode, newValue, {4});
                                             }});

    this->exist[targetNode->val] = 0;
    this->exist[newValue] = 1;
}

void DLL::searchNode(int searchValue)
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
    DLLNode *cur = this->head;

    this->animationDLL->instructions.push_back({[this]()
                                             {
                                                 this->animationDLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", {0});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < this->numberNode; ++i)
    {
        if (cur->val == searchValue)
        {
            this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->addCodeLines({5}); }});
            return;
        }

        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationDLL->instructions.push_back({[this, cur, passedLabel, currentLabel]()
                                                 {
                                                     this->animationDLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2, 3});
                                                 }});

        cur = cur->next;
    }

    //Not Found
    this->animationDLL->instructions.push_back({[this]()
                                                 { this->animationDLL->addCodeLines({4}); }});
}

void DLL::prepareNewInstruction()
{
    this->animation->finishStep();
    bool trash = 0;
    this->animationDLL->last(trash, &this->stepText);
    this->animationDLL->newInstruction(this->head, this->tail, this->start_x, this->start_y, this->nodeDistanceX);
    this->codeHighlight->reset();
}

/* Update and Render */
void DLL::updateButtons()
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

void DLL::updateChildButtons()
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

void DLL::updateInputFields()
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

void DLL::updateSFMLEvents(const float &dt, sf::Event &event)
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
                //     this->inputWarning.setString("Sorry, the animationDLL is running...");
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

void DLL::update(const float &dt)
{
    this->window->setTitle("Doubly Linked List");
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

void DLL::renderButtons(sf::RenderTarget *target)
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

void DLL::renderNode(sf::RenderTarget *target)
{
    for (DLLNode *node : this->Nodes)
    {
        node->render(target);
    }
}

void DLL::renderAnimation()
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
        this->animationDLL->play(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "pause")
    {
        this->animationDLL->pause(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "prev")
    {
        this->animationDLL->prev(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "next")
    {
        this->animationDLL->next(this->doneAnimation, &this->stepText);
        if (this->doneAnimation && this->animationDLL->curIndex == (int)this->animationDLL->instructions.size())
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "first")
    {
        this->animationDLL->first(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "last")
    {
        this->animationDLL->last(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }
}

void DLL::renderHighlightCode(sf::RenderTarget *target)
{
    codeHighlight->render(target);
}

void DLL::render(sf::RenderTarget *target)
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

void DLL::reset()
{
    exist.assign((int)exist.size(), 0);
    this->DeleteNodePointers();
    this->head = nullptr;
    this->tail = nullptr;
    this->numberNode = 0;

    delete (this->animation);
    
    this->codeHighlight->reset();
    this->animation = new AnimationDLL(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationDLL = dynamic_cast<AnimationDLL*>(this->animation);

    this->newStepTriggered = 0;
    this->doneAnimation = 1;
    this->choosingButton = "";
    this->choosingChildButton = "";
    inputWarning.setString("");
    inputGuide.setString("");
    exist.assign((int)exist.size(), 0);
    runState = "";
}
