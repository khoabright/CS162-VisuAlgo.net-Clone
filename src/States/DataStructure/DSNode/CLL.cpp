#include "CLL.h"

CLL::CLL(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states)
    : DataStructure(window, supportedKeys, states)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->exist.resize(105);

    auto CLLScale = [&]()
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
    CLLScale();

    this->initHighlightCode();
    this->initTriggerFunction();
    this->initChildButtons();
    this->initInputFields();
    this->initAnimation();
}

CLL::~CLL()
{
    this->DeleteNodePointers();
    delete (this->animationCLL);
}

void CLL::DeleteNodePointers()
{
    for (auto &v : this->Nodes)
    {
        delete (v);
    }
    this->Nodes.clear();
}

/* Init */
void CLL::initTriggerFunction()
{
    auto InstructionButtons = [&]()
    {
        this->triggerFunction["Theme"] = std::bind(&CLL::button_theme, this);
        this->triggerFunction["Initialize"] = std::bind(&CLL::button_initialize, this);
        this->triggerFunction["Add"] = std::bind(&CLL::button_add, this);
        this->triggerFunction["Delete"] = std::bind(&CLL::button_delete, this);
        this->triggerFunction["Update"] = std::bind(&CLL::button_update, this);
        this->triggerFunction["Search"] = std::bind(&CLL::button_search, this);
        this->triggerFunction["Quit"] = std::bind(&CLL::button_quit, this);
        this->triggerFunction["SpeedMinus"] = std::bind(&CLL::button_SpeedMinus, this);
        this->triggerFunction["SpeedPlus"] = std::bind(&CLL::button_SpeedPlus, this);
        this->triggerFunction["CodeSwitch"] = std::bind(&CLL::button_CodeSwitch, this);
    };

    auto RunButtons = [&]()
    {
        this->triggerFunction["First"] = std::bind(&CLL::button_first, this);
        this->triggerFunction["Prev"] = std::bind(&CLL::button_prev, this);
        this->triggerFunction["Pause"] = std::bind(&CLL::button_pause, this);
        this->triggerFunction["Play"] = std::bind(&CLL::button_play, this);
        this->triggerFunction["Replay"] = std::bind(&CLL::button_replay, this);
        this->triggerFunction["Next"] = std::bind(&CLL::button_next, this);
        this->triggerFunction["Last"] = std::bind(&CLL::button_last, this);
    };

    InstructionButtons();
    RunButtons();
}

void CLL::initChildButtons()
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

void CLL::initInputFields()
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

void CLL::initAnimation()
{
    this->animation = new AnimationCLL(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationCLL = dynamic_cast<AnimationCLL*>(this->animation);
    assert (animationCLL != nullptr);
    this->animationCLL->animationTime = speedGap * (float)maxSpeed;

}

void CLL::initHighlightCode()
{
    codeHighlight = new CodeHighlight(scale_x, scale_y, &Colors);
}

void CLL::button_theme()
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
void CLL::button_initialize()
{
    auto createNewNode = [&](int newValue)
    {
        if (this->head == nullptr)
        {
            this->head = new CLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, newValue, &this->font, &this->Colors);
            this->head->next = head;
            this->Nodes.push_back(this->head);
            ++this->numberNode;
            this->tail = head;
            return;
        }
        /* Add to tail */
        CLLNode *newNode = new CLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, newValue, &this->font, &this->Colors);
        this->Nodes.push_back(newNode);
        ++this->numberNode;

        this->tail->next = newNode;
        this->tail = newNode;
        this->tail->next = head;
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
        this->animationCLL->updateNodePosition(this->head, this->numberNode, this->start_x, this->start_y, this->nodeDistanceX, this->nodeDistanceY);
        return;
    }

    if (this->choosingChildButton == "3Load File")
    {
        if (!this->newStepTriggered)
            return;
        this->newStepTriggered = 0;

        this->reset();
        std::ifstream inp;
        inp.open("InputFiles/inputCLL.txt");
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

        this->animationCLL->updateNodePosition(this->head, this->numberNode, this->start_x, this->start_y, this->nodeDistanceX, this->nodeDistanceY);
        return;
    }
}

void CLL::button_add()
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
            this->addMiddle(this->valueFirst, this->valueSecond, 0, 0);
            return;
        }
        else
        {
            this->inputGuide.setString("Input format: [index],[value]");
        }
    }
}

void CLL::button_delete()
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
            this->deleteMiddle(this->valueFirst, 0, 0);
            return;
        }
        else
        {
            this->inputGuide.setString("Input an index");
        }
    }
}

void CLL::button_update()
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

void CLL::button_search()
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

void CLL::button_quit()
{
    this->endState();
}

/* CLL operations */
// Add
void CLL::addHead(int nodeValue)
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
            codeHighlight->codeStrings.push_back("Node newNode = new Node(v)"); // 0
            codeHighlight->codeStrings.push_back("newNode.next = newNode"); // 1
            codeHighlight->codeStrings.push_back("head = newNode"); // 2
            this->codeHighlight->updateTexts();
        };
        addHighlightCodes();

        /* Create and Show Node */
        this->head = new CLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
        this->head->isNewNode = 1;
        this->head->next = this->head;
        this->Nodes.push_back(this->head);

        this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->showNode(this->head, "newNode", this->numberNode, {0}); }});
        this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->addCodeLines({1}); }});
        this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->changeNodeLabel(this->head, "head", {2}); }});

        return;
    }
    
    /* Not nullptr, call addMiddle */
    addMiddle(0, nodeValue, 1, 0);
}

void CLL::addTail(int nodeValue)
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
            codeHighlight->codeStrings.push_back("Node newNode = new Node(v)"); // 0
            codeHighlight->codeStrings.push_back("newNode.next = newNode"); // 1
            codeHighlight->codeStrings.push_back("head = newNode"); // 2
            this->codeHighlight->updateTexts();
        };
        addHighlightCodes();

        /* Create and Show Node */
        this->head = new CLLNode(this->start_x, this->start_y, this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
        this->head->isNewNode = 1;
        this->head->next = this->head;
        this->Nodes.push_back(this->head);

        this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->showNode(this->head, "newNode", this->numberNode, {0}); }});
        this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->addCodeLines({1}); }});
        this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->changeNodeLabel(this->head, "head", {2}); }});

        return;
    }

    /* Not nullptr, call addMiddle */
    addMiddle(0, nodeValue, 0, 1);
}

void CLL::addMiddle(int index, int nodeValue, bool addHead, bool addTail)
{   
    /* Complete all animations */
    this->prepareNewInstruction();
    this->button_play();

    if (!addHead && !addTail) /* Conditions are not be checked */
    {

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
        if (index < 0 || index > this->numberNode - 1)
        {
            this->inputWarning.setString("Index should be in range [0.." + std::to_string(this->numberNode - 1) + "]");
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
    }

    if (index == 0) index = numberNode; /* Because circular */
    /* Start here*/

    auto addHighlightCodes = [&]
    {
        std::string idxString = "index - 1";
        if (addHead) {
            idxString = "sizeList - 1";
            codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to head"); 
        }
        if (addTail) {
            idxString = "sizeList - 1";
            codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to tail"); 
        }
        if (!addHead && !addTail)
            codeHighlight->introText.setString("Add " + std::to_string(nodeValue) + " to index " + std::to_string(index));

        codeHighlight->codeStrings.push_back("Node cur = head");                      // 0


        codeHighlight->codeStrings.push_back("for (i = 0; i < " + idxString + "; ++i)");      // 1
        codeHighlight->codeStrings.push_back("   cur = cur.next");                    // 2
        codeHighlight->codeStrings.push_back("Node nxt = cur.next");                  // 3
        codeHighlight->codeStrings.push_back("Node newNode = new Node(v)");           // 4
        codeHighlight->codeStrings.push_back("newNode.next = nxt");                   // 5
        codeHighlight->codeStrings.push_back("cur.next = newNode");                   // 6
        if (addHead) 
            codeHighlight->codeStrings.push_back("head = newNode");                   // 7

        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 7 or 8
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    /* Highlight first node  */
    CLLNode *cur = this->head;

    this->animationCLL->instructions.push_back(
        {[this]()
         {
             this->animationCLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", "currentColor", {0});
         }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationCLL->instructions.push_back({[this, cur, passedLabel, currentLabel]()
                                                    {
                                                        this->animationCLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2});
                                                    }});
        cur = cur->next;
    }

    std::string passedLabel = "nxt/" + std::to_string(index);
    if (cur == head) passedLabel = "h/cur/nxt";

    this->animationCLL->instructions.push_back(
        {[this, cur, index, passedLabel]()
         {
             this->animationCLL->highlightPassedNode(cur, cur->labelString, passedLabel, "passedColor", "reachColor", {3});
         }});

    /* Create new Node */
    CLLNode *newNode = new CLLNode(cur->next->x, cur->next->y + 3 * this->nodeDistanceY,
                                   this->scale_x, this->scale_y, nodeValue, &this->font, &this->Colors);
    newNode->isNewNode = 1;
    this->Nodes.push_back(newNode);
    this->animationCLL->instructions.push_back({[this, newNode]()
                                             { this->animationCLL->showNode(newNode, "newNode", this->numberNode, {4}); }});

    /* Connect newNode to cur->next */
    this->animationCLL->instructions.push_back({[this, newNode, cur]()
                                             { this->animationCLL->connectNodes(newNode, cur->next, {5}); }});

    // Connect cur to new Node
    this->animationCLL->instructions.push_back({[this, cur, newNode]()
                                             { this->animationCLL->connectNodes(cur, newNode, {6}); }});

    /* Change head to newNode */
    if (addHead) {
        CLLNode *old_head = this->head;
        this->animationCLL->instructions.push_back({[this, newNode, old_head]()
                                                { this->animationCLL->changeHead(this->head, old_head, newNode, {7}); }});
    }

    // Relayout
    this->animationCLL->instructions.push_back({[this, addHead, addTail]()
                                             { this->animationCLL->Relayout((numberNode == 0), this->head, numberNode, this->start_x, this->start_y, nodeDistanceX, nodeDistanceY, {7 + (addHead > 0)}); }});
}

// Delete
void CLL::deleteHead()
{
    this->prepareNewInstruction();
    this->button_play();

    this->deleteMiddle(0, 1, 0);
    return;
}

void CLL::deleteTail()
{      
    this->prepareNewInstruction();
    this->button_play();

    this->deleteMiddle(numberNode - 1, 0, 1);
    return;
}

void CLL::deleteMiddle(int index, bool delHead, bool delTail)
{   
    this->prepareNewInstruction();
    this->button_play();

    if (!delHead && !delTail)
    { /* Conditions are not be checked */

        if (index < 0 || index > this->numberNode - 1)
        {
            this->inputWarning.setString("Index should be in range [0.." + std::to_string(this->numberNode - 1) + "]");
            return;
        }
        if (index == 0) {
            deleteHead();
            return;
        }
    }

    if (index == 0) index = numberNode;

    //Start here
    auto addHighlightCodes = [&]
    {
        std::string idxString = "index - 1";
        if (delHead)
        {
            idxString = "sizeList - 1";
            codeHighlight->introText.setString("Delete head"); 
        }
        if (delTail)
        {
            idxString = "sizeList - 1";
            codeHighlight->introText.setString("Delete tail"); 
        }

        if (!delHead && !delTail)
            codeHighlight->introText.setString("Delete index " + std::to_string(index));
        codeHighlight->codeStrings.push_back("if empty, do nothing");                        // 0
        codeHighlight->codeStrings.push_back("if (sizeList == 1) delete head, return");      // 1
        codeHighlight->codeStrings.push_back("Node cur = head");                             // 2
        codeHighlight->codeStrings.push_back("for (i = 0; i < " + idxString + "; ++i)");     // 3
        codeHighlight->codeStrings.push_back("   cur = cur.next");                           // 4
        codeHighlight->codeStrings.push_back("Node target = cur.next");                      // 5
        codeHighlight->codeStrings.push_back("Node nxt = target.next");                      // 6
        codeHighlight->codeStrings.push_back("cur.next = nxt");                              // 7

        if (delHead)
            codeHighlight->codeStrings.push_back("head = nxt");                              // 8
        

        codeHighlight->codeStrings.push_back("delete target");                        // 8 or 9
        codeHighlight->codeStrings.push_back("// Relayout, not in actual operation"); // 9 or 10
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    if (this->head == nullptr) {
        this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->addCodeLines({0}); }});

        return;
    }

    if (numberNode == 1)
    {
        CLLNode* targetNode = head;
        this->head = nullptr;

        /* Delete head */
        this->exist[targetNode->val] = 0;
        this->animationCLL->instructions.push_back({[this, targetNode]()
                                                    {
                                                        this->animationCLL->hideNode(targetNode, this->numberNode, {1});
                                                    }});
        return;
    }

    /* Highlight first node  */
    CLLNode *cur = this->head;

    this->animationCLL->instructions.push_back({[this]()
                                             {
                                                 this->animationCLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", "currentColor", {2});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationCLL->instructions.push_back(
            {[this, cur, passedLabel, currentLabel]()
             {
                 this->animationCLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {3, 4});
             }});
        cur = cur->next;
    }

    /* Highlight target Node */
    CLLNode *targetNode = cur->next;
    this->animationCLL->instructions.push_back(
        {[this, cur, index]()
         {
             this->animationCLL->highlightPassedNode(cur, cur->labelString, "target/" + std::to_string(index), "passedColor", "deleteColor", {5});
         }});


    /* Highlight target->next */
    std::string passedLabel = "nxt/" + std::to_string(index);
    if (cur == targetNode->next) passedLabel = "cur/nxt";

    CLLNode *nxt = targetNode->next;
    this->animationCLL->instructions.push_back({[this, nxt, index, passedLabel]()
                                             {
                                                 this->animationCLL->highlightCurrentNode(nxt, passedLabel, "reachColor", "reachColor", {6});
                                             }});


    /* Connect cur-----cur->next->next ; simultaneously Move targetNode to another position */
    this->animationCLL->instructions.push_back({[this, targetNode]()
                                             {
                                                 this->animationCLL->moveNode(targetNode, targetNode->x, targetNode->y + 3 * nodeDistanceY);
                                             },
                                             [this, targetNode]()
                                             {
                                                 this->animationCLL->makeArrow(&targetNode->shape, &targetNode->next->shape, &targetNode->arrow);
                                             },
                                             [this, cur]()
                                             {
                                                 this->animationCLL->connectNodes(cur, cur->next->next, {7});
                                             }});

    /* Change head to nxt */
    if (delHead) {
        CLLNode *old_head = this->head;
        this->animationCLL->instructions.push_back({[this, nxt, old_head]()
                                                { this->animationCLL->changeHead(this->head, old_head, nxt, {8}); }});
    }

    /* Delete targetNode */
    this->exist[targetNode->val] = 0;
    this->animationCLL->instructions.push_back({[this, targetNode, delHead]()
                                             {
                                                 this->animationCLL->hideNode(targetNode, this->numberNode, {8 + delHead});
                                             }});

    /* Relayout */
    this->animationCLL->instructions.push_back({[this, delHead]()
                                             { this->animationCLL->Relayout((numberNode == 0), this->head, numberNode, this->start_x, this->start_y, nodeDistanceX, nodeDistanceY, {9 + delHead}); }});

    return;
}

void CLL::updateNode(int index, int newValue)
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
        CLLNode *targetNode = head;

        this->animationCLL->instructions.push_back({[this, targetNode]()
                                                 {
                                                     this->animationCLL->highlightCurrentNode(targetNode, "target/0", "reachColor", "reachColor", {});
                                                 }});

        /* Change targetNode to newValue */
        this->animationCLL->instructions.push_back({[this, targetNode, newValue]()
                                                 {
                                                     this->animationCLL->changeNodeValue(targetNode, newValue, {});
                                                 }});

        this->exist[targetNode->val] = 0;
        this->exist[newValue] = 1;

        return;
    }

    /* Highlight first node  */
    CLLNode *cur = this->head;

    this->animationCLL->instructions.push_back({[this]()
                                             {
                                                 this->animationCLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", "currentColor", {0});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < index - 1; ++i)
    {
        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        this->animationCLL->instructions.push_back({[this, cur, passedLabel, currentLabel]()
                                                 {
                                                     this->animationCLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2});
                                                 }});
        cur = cur->next;
    }

    /* Highlight target Node */
    this->animationCLL->instructions.push_back({[this, cur, index]()
                                             {
                                                 this->animationCLL->highlightPassedNode(cur, cur->labelString, "target/" + std::to_string(index), "passedColor", "reachColor", {3});
                                             }});

    CLLNode *targetNode = cur->next;
    /* Change targetNode to newValue */
    this->animationCLL->instructions.push_back({[this, targetNode, newValue]()
                                             {
                                                 this->animationCLL->changeNodeValue(targetNode, newValue, {4});
                                             }});

    this->exist[targetNode->val] = 0;
    this->exist[newValue] = 1;
}

void CLL::searchNode(int searchValue)
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
    CLLNode *cur = this->head;

    this->animationCLL->instructions.push_back({[this]()
                                             {
                                                 this->animationCLL->highlightCurrentNode(this->head, "h/cur/0", "currentColor", "currentColor", {0});
                                             }});

    /* Highlight path to index */
    for (int i = 0; i < this->numberNode; ++i)
    {
        if (cur->val == searchValue)
        {
            this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->addCodeLines({5}); }});
            return;
        }

        std::string passedLabel = cur->labelString;
        if (i == 0)
            passedLabel = "head/0";
        std::string currentLabel = "cur/" + std::to_string(i + 1);

        if (cur->next == head) {
            this->animationCLL->instructions.push_back({[this, cur]()
                                                        {
                                                            this->animationCLL->highlightCurrentNode(cur, "", "normalFillColor", "passedColor", {4});
                                                        }});
            return;
        }
        this->animationCLL->instructions.push_back({[this, cur, passedLabel, currentLabel]()
                                                 {
                                                     this->animationCLL->highlightPassedNode(cur, passedLabel, currentLabel, "passedColor", "currentColor", {1, 2, 3});
                                                 }});

        cur = cur->next;
    }

    assert(0); /* It should end previously, not until here */
    //Not Found
    this->animationCLL->instructions.push_back({[this]()
                                                 { this->animationCLL->addCodeLines({4}); }});
}

void CLL::prepareNewInstruction()
{
    this->animation->finishStep();
    bool trash = 0;
    this->animationCLL->last(trash, &this->stepText);
    this->animationCLL->newInstruction(this->head, this->numberNode, this->start_x, this->start_y, this->nodeDistanceX, this->nodeDistanceY);
    this->codeHighlight->reset();
}

/* Update and Render */
void CLL::updateButtons()
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

void CLL::updateChildButtons()
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

void CLL::updateInputFields()
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

void CLL::updateSFMLEvents(const float &dt, sf::Event &event)
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
                //     this->inputWarning.setString("Sorry, the animationCLL is running...");
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

void CLL::update(const float &dt)
{   
    this->window->setTitle("Circular Linked List");
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

void CLL::renderButtons(sf::RenderTarget *target)
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

void CLL::renderNode(sf::RenderTarget *target)
{
    for (CLLNode *node : this->Nodes)
    {
        node->render(target);
    }
}

void CLL::renderAnimation()
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
        this->animationCLL->play(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "pause")
    {
        this->animationCLL->pause(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "prev")
    {
        this->animationCLL->prev(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "next")
    {
        this->animationCLL->next(this->doneAnimation, &this->stepText);
        if (this->doneAnimation && this->animationCLL->curIndex == (int)this->animationCLL->instructions.size())
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "first")
    {
        this->animationCLL->first(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "last")
    {
        this->animationCLL->last(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }
}

void CLL::renderHighlightCode(sf::RenderTarget *target)
{
    codeHighlight->render(target);
}

void CLL::render(sf::RenderTarget *target)
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

void CLL::reset()
{
    exist.assign((int)exist.size(), 0);
    this->DeleteNodePointers();
    this->head = nullptr;
    this->tail = nullptr;
    this->numberNode = 0;

    delete (this->animation);
    
    this->codeHighlight->reset();
    this->animation = new AnimationCLL(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationCLL = dynamic_cast<AnimationCLL*>(this->animation);
    assert (animationCLL != nullptr);

    this->newStepTriggered = 0;
    this->doneAnimation = 1;
    this->choosingButton = "";
    this->choosingChildButton = "";
    inputWarning.setString("");
    inputGuide.setString("");
    exist.assign((int)exist.size(), 0);
    runState = "";
}
