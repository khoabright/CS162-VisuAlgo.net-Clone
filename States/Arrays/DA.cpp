#include "DA.h"

DA::DA(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states)
    : DataStructure(window, supportedKeys, states)
{
    auto DAScale = [&]()
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
    DAScale();

    this->ReInitButtons();
    this->initHighlightCode();
    this->initTriggerFunction();
    this->initChildButtons();
    this->initInputFields();
    this->initAnimation();

    // Text
    arr_name.setString("arr");
    arr_name.setCharacterSize(1.5 * characterSize);
    arr_name.setFont(font);
    arr_name.setPosition(start_x - arr_name.getGlobalBounds().width - 20.f * scale_x, start_y + 12.5 * scale_y - arr_name.getGlobalBounds().height / 2);

    new_arr_name.setString("arr2");
    new_arr_name.setCharacterSize(1.5 * characterSize);
    new_arr_name.setFont(font);
    new_arr_name.setPosition(start_x - new_arr_name.getGlobalBounds().width - 20.f * scale_x, start_y + 12.5 * scale_y - arr_name.getGlobalBounds().height / 2 + 2 * nodeDistanceY - 30.f * scale_y);
}

DA::~DA()
{
    // std::cout << "Ending DA\n";
    this->DeleteArrayPointers(arr);
    this->DeleteArrayPointers(new_arr);
    delete (this->animationDA);
}

void DA::DeleteArrayPointers(std::vector<DANode *> &vec)
{
    for (auto &v : vec)
    {
        delete (v);
    }
    vec.clear();
}

/* Init */
void DA::initTriggerFunction()
{
    auto InstructionButtons = [&]()
    {
        this->triggerFunction["Theme"] = std::bind(&DA::button_theme, this);
        this->triggerFunction["Initialize"] = std::bind(&DA::button_initialize, this);
        this->triggerFunction["Add"] = std::bind(&DA::button_add, this);
        this->triggerFunction["Delete"] = std::bind(&DA::button_delete, this);
        this->triggerFunction["Update"] = std::bind(&DA::button_update, this);
        this->triggerFunction["Search"] = std::bind(&DA::button_search, this);
        this->triggerFunction["Access"] = std::bind(&DA::button_access, this);
        this->triggerFunction["Quit"] = std::bind(&DA::button_quit, this);
        this->triggerFunction["SpeedMinus"] = std::bind(&DA::button_SpeedMinus, this);
        this->triggerFunction["SpeedPlus"] = std::bind(&DA::button_SpeedPlus, this);
        this->triggerFunction["CodeSwitch"] = std::bind(&DA::button_CodeSwitch, this);
    };

    auto RunButtons = [&]()
    {
        this->triggerFunction["First"] = std::bind(&DA::button_first, this);
        this->triggerFunction["Prev"] = std::bind(&DA::button_prev, this);
        this->triggerFunction["Pause"] = std::bind(&DA::button_pause, this);
        this->triggerFunction["Play"] = std::bind(&DA::button_play, this);
        this->triggerFunction["Replay"] = std::bind(&DA::button_replay, this);
        this->triggerFunction["Next"] = std::bind(&DA::button_next, this);
        this->triggerFunction["Last"] = std::bind(&DA::button_last, this);
    };

    InstructionButtons();
    RunButtons();
}

void DA::initChildButtons()
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

    auto init_access = [&]() {
    };

    init_theme();
    init_initialize();
    init_add();
    init_delete();
    init_update();
    init_search();
    init_access();
}

void DA::initInputFields()
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

    auto init_access = [&]()
    {
        this->inputFields["Access"]["1Access"] = new InputField(this->inputFieldOriginX, 723.f * this->scale_y,
                                                                this->inputFieldWidth, this->inputFieldHeight, this->scale_x, this->scale_y,
                                                                &this->font, this->inputFieldCharacterSize,
                                                                sf::Color::White, sf::Color::White, sf::Color::Black);
    };

    init_initialize();
    init_add();
    init_delete();
    init_update();
    init_search();
    init_access();
}

void DA::initAnimation()
{
    this->animation = new AnimationDA(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationDA = dynamic_cast<AnimationDA *>(this->animation);
    assert(animationDA != nullptr);
    this->animationDA->animationTime = speedGap * (float)maxSpeed;
}

void DA::initHighlightCode()
{
    codeHighlight = new CodeHighlight(scale_x, scale_y, &Colors);
}

void DA::button_theme()
{

    if (this->choosingChildButton == "1Standard")
    {
        this->newStepTriggered = 0;
        this->DataStructureThemeStandard();
        for (int i = 0; i < (int)arr.size(); ++i)
            arr[i]->updateColor();
        for (int i = 0; i < (int)new_arr.size(); ++i) 
            new_arr[i]->updateColor();
        
        this->choosingButton = "";
        return;
    }
    if (this->choosingChildButton == "2Sky")
    {
        this->newStepTriggered = 0;
        this->DataStructureThemeSky();
        for (int i = 0; i < (int)arr.size(); ++i)
            arr[i]->updateColor();
        for (int i = 0; i < (int)new_arr.size(); ++i) 
            new_arr[i]->updateColor();

        this->choosingButton = "";
        return;
    }
    if (this->choosingChildButton == "3Desert")
    {
        this->newStepTriggered = 0;
        this->DataStructureThemeDesert();
        for (int i = 0; i < (int)arr.size(); ++i)
            arr[i]->updateColor();
        for (int i = 0; i < (int)new_arr.size(); ++i) 
            new_arr[i]->updateColor();

        this->choosingButton = "";
        return;
    }

    return;
}

/* Instruction Buttons */
void DA::button_initialize()
{
    if (this->choosingChildButton == "1Empty")
    {
        if (this->newStepTriggered)
        {
            this->newStepTriggered = 0;
            this->choosingButton = "";
            this->inputGuide.setString("");

            if (valueFirst > maxCapacity)
            {
                this->inputWarning.setString("Sorry, maximum size is " + std::to_string(maxCapacity));
                return;
            }
            if (valueFirst < 1) {
                this->inputWarning.setString("Capacity must be positive");
                return;
            }
        }
        else
        {
            this->inputGuide.setString("Input a capacity");
            return;
        }

        this->reset();
        this->prepareNewInstruction();
        this->button_play();

        codeHighlight->introText.setString("New empty array with capacity " + std::to_string(valueFirst));
        codeHighlight->codeStrings.push_back("int *arr = new int[capacity]");    
        this->codeHighlight->updateTexts(); 
        codeHighlight->next_currentLines({0});  

        arr_size = 0;
        capacity = valueFirst;
        arr.resize(capacity);
        

        for (int i = 0; i < capacity; ++i)
        {
            DANode *newNode = new DANode(this->start_x, this->start_y, this->scale_x, this->scale_y, 0, std::to_string(i), &this->font, &this->Colors);
            arr[i] = newNode;
        }

        this->animationDA->updateNodePosition(arr, arr_size, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }

    if (this->choosingChildButton == "2Random")
    {
        if (this->newStepTriggered)
        {
            this->newStepTriggered = 0;
            this->choosingButton = "";
            this->inputGuide.setString("");

            if (valueSecond < valueFirst)
            {
                this->inputWarning.setString("Capacity cannot be smaller than size");
                return;
            }
            if (valueSecond > maxCapacity)
            {
                this->inputWarning.setString("Sorry, maximum size is " + std::to_string(maxCapacity));
                return;
            }
            if (valueSecond < 1) {
                this->inputWarning.setString("Capacity must be positive");
                return;
            }
        }
        else
        {
            this->inputGuide.setString("Input format: [size],[capacity]");
            return;
        }

        this->reset();
        this->prepareNewInstruction();
        this->button_play();

        arr_size = valueFirst;
        capacity = valueSecond;
        arr.resize(capacity);
        

        std::vector<int> values(99);
        for (int i = 0; i < 99; ++i)
            values[i] = i + 1;
        std::shuffle(values.begin(), values.end(), this->randomize);

        for (int i = 0; i < arr_size; ++i)
        {
            int newValue = values[i];
            DANode *newNode = new DANode(this->start_x, this->start_y, this->scale_x, this->scale_y, newValue, std::to_string(i), &this->font, &this->Colors);
            newNode->newColor("normalFillColor", "newColor");
            newNode->nextColor();
            arr[i] = newNode;
        }

        for (int i = arr_size; i < capacity; ++i)
        {
            DANode *newNode = new DANode(this->start_x, this->start_y, this->scale_x, this->scale_y, 0, std::to_string(i), &this->font, &this->Colors);
            arr[i] = newNode;
        }

        this->animationDA->updateNodePosition(arr, arr_size, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }

    if (this->choosingChildButton == "3Load File")
    {
        if (!this->newStepTriggered)
            return;

        this->newStepTriggered = 0;
        this->choosingButton = "";

        this->reset();
        this->prepareNewInstruction();
        this->button_play();

        std::ifstream inp;
        inp.open("InputFiles/inputDA.txt");
        inp >> capacity;
        arr.resize(capacity);
        
        inp >> arr_size;

        int newValue = 0;
        for (int i = 0; i < arr_size; ++i)
        {
            inp >> newValue;
            DANode *newNode = new DANode(this->start_x, this->start_y, this->scale_x, this->scale_y, newValue, std::to_string(i), &this->font, &this->Colors);
            newNode->newColor("normalFillColor", "newColor");
            newNode->nextColor();
            arr[i] = newNode;
        }
        for (int i = arr_size; i < capacity; ++i)
        {
            DANode *newNode = new DANode(this->start_x, this->start_y, this->scale_x, this->scale_y, 0, std::to_string(i), &this->font, &this->Colors);
            arr[i] = newNode;
        }
        inp.close();

        this->animationDA->updateNodePosition(this->arr, arr_size, this->start_x, this->start_y, this->nodeDistanceX);
        return;
    }
}

void DA::button_add()
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

void DA::button_delete()
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

void DA::button_update()
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

void DA::button_access()
{
    this->choosingChildButton = "1Access";

    if (this->newStepTriggered)
    {
        this->newStepTriggered = 0;
        this->inputGuide.setString("");
        this->accessNode(this->valueFirst);
        return;
    }
    else
    {
        this->inputGuide.setString("Input an index");
    }
}

void DA::button_search()
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

void DA::button_quit()
{
    this->endState();
}

/* DA operations */
// Add
void DA::addHead(int nodeValue)
{
    /* Complete all animations */
    this->prepareNewInstruction();
    this->button_play();

    if (nodeValue < 1 || nodeValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }

    if (arr_size + 1 > maxCapacity)
    {
        this->inputWarning.setString("Sorry, the maximum size is " + std::to_string(maxCapacity));
        return;
    }

    addMiddle(0, nodeValue);
}

void DA::addTail(int nodeValue)
{
    /* Complete all animations */
    this->prepareNewInstruction();
    this->button_play();

    if (nodeValue < 1 || nodeValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }

    if (arr_size + 1 > maxCapacity)
    {
        this->inputWarning.setString("Sorry, the maximum size is " + std::to_string(maxCapacity));
        return;
    }

    addMiddle(arr_size, nodeValue);
}

void DA::addMiddle(int index, int nodeValue)
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
    if (index < 0 || index > this->arr_size)
    {
        this->inputWarning.setString("Index should be in range [0.." + std::to_string(this->arr_size) + "]");
        return;
    }
    if (capacity == 0) {
        this->inputWarning.setString("Array does not exist");
        return;
    }
    if (arr_size + 1 > capacity && capacity * 2 > maxCapacity)
    {
        this->inputWarning.setString("Capacity is too large, cannot expand anymore");
    }

    /* Start here */
    
    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Insert " + std::to_string(nodeValue) + " to index " + std::to_string(index));
        codeHighlight->codeStrings.push_back("if (arrSize + 1 > capacity) {");          // 0
        codeHighlight->codeStrings.push_back("   int *arr2 = new int[capacity / 2]"); // 1
        codeHighlight->codeStrings.push_back("   for (i = 0; i < arrSize; ++i)");     // 2
        codeHighlight->codeStrings.push_back("      arr2[i] = arr[i]");               // 3
        codeHighlight->codeStrings.push_back("   delete[] arr, arr = arr2");          // 4
        codeHighlight->codeStrings.push_back("}");                                     // 5
        codeHighlight->codeStrings.push_back("for (i = arrSize; i > index; --i)");    // 6
        codeHighlight->codeStrings.push_back("   arr[i] = arr[i - 1]");               // 7
        codeHighlight->codeStrings.push_back("arr[index] = newVal");                  // 8
        codeHighlight->codeStrings.push_back("++arrSize");                            // 9
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    /* Reserve more spaces */
    if (arr_size + 1 > capacity) {
        resizeArray(2 * capacity, 1);
    }

    /* Start actual insert */
    std::vector<DANode*> tmp_arr = arr;
    int tmp_arr_size = arr_size;
    int tmp_capacity = capacity;
    if (resizing) {
        tmp_arr.clear();
        tmp_arr = new_arr;
        tmp_capacity = new_capacity;
    }

    for (int i = tmp_arr_size; i > index; --i)
    {
        std::vector<std::function<void()>> vec(
            {[this, i, tmp_arr]()
             { this->animationDA->highlightCurrentNode(tmp_arr[i], "i/" + std::to_string(i), "normalFillColor", "currentColor", {6}); }});

        /* Unhighlight last node */
        if (i < tmp_arr_size - 1)
        {
            vec.push_back([this, i, tmp_arr]()
                          { this->animationDA->highlightCurrentNode(tmp_arr[i + 1], std::to_string(i + 1), "normalFillColor", "newColor", {6}); });
        }
        if (i == tmp_arr_size - 1)
        {
            vec.push_back([this, i, tmp_arr]()
                          { this->animationDA->highlightCurrentNode(tmp_arr[i + 1], std::to_string(i + 1), "normalFillColor", "emptyColor", {6}); });
        }

        this->animationDA->instructions.push_back(vec);

        this->animationDA->instructions.push_back(
            {[this, i, tmp_arr]()
             { this->animationDA->changeNodeValue(tmp_arr[i], arr[i - 1]->val, {7}); }}); /* use arr because new_arr is not created yet */
    }

    /* arr[index] = newVal */
    std::vector<std::function<void()>> vec(
        {[this, index, nodeValue, tmp_arr]()
         { this->animationDA->changeNodeValue(tmp_arr[index], nodeValue, {8}); },

         [this, index, nodeValue, tmp_arr]()
         { this->animationDA->highlightCurrentNode(tmp_arr[index], "index/" + std::to_string(index), "normalFillColor", "currentColor", {8}); }

        });
    if (index < tmp_arr_size - 1)
    {   
        /* Unhighlight last node */
        vec.push_back(
        [this, index, tmp_arr]() 
            { this->animationDA->highlightCurrentNode(tmp_arr[index + 1], std::to_string(index + 1), "normalFillColor", "newColor", {8}); });
    }
    if (index == tmp_arr_size - 1)
    {   
        /* Unhighlight last node */
        vec.push_back(
        [this, index, tmp_arr]() 
            { this->animationDA->highlightCurrentNode(tmp_arr[index + 1], std::to_string(index + 1), "normalFillColor", "emptyColor", {8}); });
    }
    this->animationDA->instructions.push_back(vec);

    /* ++arr_size */
    this->animationDA->instructions.push_back({
        [this, index, tmp_arr, tmp_arr_size]() 
            { this->animationDA->highlightCurrentNode(tmp_arr[tmp_arr_size], std::to_string(tmp_arr_size), "normalFillColor", "newColor", {9}); }});

    ++arr_size;
}

// Delete
void DA::deleteHead()
{
    this->prepareNewInstruction();
    this->button_play();

    deleteMiddle(0);
}

void DA::deleteTail()
{
    this->prepareNewInstruction();
    this->button_play();

    deleteMiddle(arr_size - 1);
}

void DA::deleteMiddle(int index)
{
    this->prepareNewInstruction();
    this->button_play();

    if (arr_size == 0)
    {
        this->inputWarning.setString("Empty list");
        return;
    }

    if (index < 0 || index > this->arr_size - 1)
    {
        this->inputWarning.setString("Index should be in range [0.." + std::to_string(this->arr_size - 1) + "]");
        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Delete index " + std::to_string(index));
        codeHighlight->codeStrings.push_back("if (arrSize - 1 < capacity / 2");         // 0
        codeHighlight->codeStrings.push_back("    && capacity / 2 > threshold) {");        // 1
        codeHighlight->codeStrings.push_back("//threshold can be whatever, here is 3"); // 2
        codeHighlight->codeStrings.push_back("   int *arr2 = new int[capacity / 2]");             // 3
        codeHighlight->codeStrings.push_back("   for (i = 0; i < arrSize; ++i)");       // 4
        codeHighlight->codeStrings.push_back("      arr2[i] = arr[i]");                 // 5
        codeHighlight->codeStrings.push_back("   delete[] arr, arr = arr2");             // 6
        codeHighlight->codeStrings.push_back("}");                                       // 7
        codeHighlight->codeStrings.push_back("for (i = index; i < arr_size - 1; ++i)"); // 8
        codeHighlight->codeStrings.push_back("   a[i] = a[i + 1]");                     // 9
        codeHighlight->codeStrings.push_back("--arr_size");                             // 10
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    /* Delete some spaces */
    if (arr_size - 1 < capacity / 2 && capacity / 2 > threshold) {
        resizeArray(capacity / 2, 3);
    }

    /* Start actual insert */
    std::vector<DANode*> tmp_arr = arr;
    int tmp_arr_size = arr_size;
    int tmp_capacity = capacity;
    if (resizing) {
        tmp_arr.clear();
        tmp_arr = new_arr;
        tmp_capacity = new_capacity;
    }

    for (int i = index; i < tmp_arr_size - 1; ++i)
    {
        std::vector<std::function<void()>> vec(
            {[this, i, tmp_arr]()
             { this->animationDA->highlightCurrentNode(tmp_arr[i], "i/" + std::to_string(i), "normalFillColor", "currentColor", {8}); }});

        /* Unhighlight last node */
        if (i > index)
        {
            vec.push_back([this, i, tmp_arr]()
                          { this->animationDA->highlightCurrentNode(tmp_arr[i - 1], std::to_string(i - 1), "normalFillColor", "newColor", {9}); });
        }

        this->animationDA->instructions.push_back(vec);

        this->animationDA->instructions.push_back(
            {[this, i, tmp_arr]()
             { this->animationDA->changeNodeValue(tmp_arr[i], arr[i + 1]->val, {9}); }}); /* Use arr because new_arr is not created yet */
    }

    --arr_size;
    std::vector<std::function<void()>> vec({[this, index, tmp_arr]()
                                            { this->animationDA->highlightCurrentNode(tmp_arr[arr_size], std::to_string(arr_size), "normalFillColor", "emptyColor", {10}); }});

    if (arr_size > 0)
    {
        vec.push_back(
            [this, index, tmp_arr]() /* Unhighlight last node */
            { this->animationDA->highlightCurrentNode(tmp_arr[arr_size - 1], std::to_string(arr_size - 1), "normalFillColor", "newColor", {10}); });
    }
    this->animationDA->instructions.push_back(vec);
    return;
}

void DA::updateNode(int index, int newValue)
{
    this->prepareNewInstruction();
    this->button_play();

    if (index == -1 || newValue == -1)
    {
        this->inputWarning.setString("Wrong input format");
        return;
    }
    if (arr_size == 0)
    {
        this->inputWarning.setString("Cannot update empty list");
        return;
    }
    if (newValue < 1 || newValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }

    if (index < 0 || index > this->arr_size - 1)
    {
        this->inputWarning.setString("Index should be in range [0.." + std::to_string(this->arr_size - 1) + "]");
        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Update index " + std::to_string(index) + " to " + std::to_string(newValue));
        codeHighlight->codeStrings.push_back("a[index] = newVal"); // 0
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    this->animationDA->instructions.push_back(
        {[this, index]()
         { this->animationDA->highlightCurrentNode(arr[index], "index/" + std::to_string(index), "normalFillColor", "currentColor", {0}); },
         [this, index, newValue]()
         { this->animationDA->changeNodeValue(arr[index], newValue, {0}); }});

    // // reset label and highlight
    // this->animationDA->instructions.push_back(
    //     {[this, index]()
    //      { this->animationDA->highlightCurrentNode(arr[index], std::to_string(index), "normalFillColor", "newColor", {}); }});
    
    return;
}

void DA::searchNode(int searchValue)
{
    this->prepareNewInstruction();
    this->button_play();

    if (searchValue < 1 || searchValue > 99)
    {
        this->inputWarning.setString("Value should be in range [1..99]");
        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Search index with value " + std::to_string(searchValue));
        codeHighlight->codeStrings.push_back("for (int i = 0; i < arr_size; ++i)"); // 0
        codeHighlight->codeStrings.push_back("   if (a[i] == searchVal) return i"); // 1
        codeHighlight->codeStrings.push_back("return NOT_FOUND");                   // 2
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    for (int i = 0; i < arr_size; ++i)
    {
        std::vector<std::function<void()>> vec(
            {[this, i]()
             { this->animationDA->highlightCurrentNode(arr[i], "i/" + std::to_string(i), "normalFillColor", "currentColor", {0}); }});

        /* Unhighlight last node */
        if (i > 0)
        {
            vec.push_back([this, i]()
                          { this->animationDA->highlightCurrentNode(arr[i - 1], std::to_string(i - 1), "normalFillColor", "newColor", {0}); });
        }

        this->animationDA->instructions.push_back(vec);

        if (arr[i]->val == searchValue) {
            this->animationDA->instructions.push_back({[this, i]()
             { this->animationDA->highlightCurrentNode(arr[i], "return/" + std::to_string(i), "normalFillColor", "currentColor", {1}); }});
            
            return;
        }
    }

    this->animationDA->instructions.push_back({[this]()
             { this->animationDA->highlightCurrentNode(arr[arr_size - 1], std::to_string(arr_size - 1), "normalFillColor", "newColor", {2}); }});

    return;
}

void DA::accessNode(int index)
{
    this->prepareNewInstruction();
    this->button_play();

    if (index < 0 || index > this->arr_size - 1)
    {
        this->inputWarning.setString("Index should be in range [0.." + std::to_string(this->arr_size - 1) + "]");
        return;
    }

    auto addHighlightCodes = [&]
    {
        codeHighlight->introText.setString("Access index " + std::to_string(index));
        codeHighlight->codeStrings.push_back("return a[index]"); // 0
        this->codeHighlight->updateTexts();
    };
    addHighlightCodes();

    this->animationDA->instructions.push_back(
        {[this, index]()
         { this->animationDA->highlightCurrentNode(arr[index], "index/" + std::to_string(index), "normalFillColor", "currentColor", {0}); }});

    return;
}

void DA::prepareNewInstruction()
{
    this->animation->finishStep();
    bool trash = 0;
    this->animationDA->last(trash, &this->stepText);
    
    show_arr = 1;
    show_new_arr = 0;
    if (resizing) {
        resizing = 0;
        // DeleteArrayPointers(arr);
        arr = new_arr;
        new_arr.clear();
        capacity = new_capacity;
        show_arr = 1;
        show_new_arr = 0;
    }
    this->animationDA->newInstruction(arr, arr_size, this->start_x, this->start_y, this->nodeDistanceX);
    this->codeHighlight->reset();
}

void DA::createArray(std::vector<DANode *> &_arr, int _capacity, int _start_x, int _start_y)
{
    _arr.resize(_capacity);

    for (int i = 0; i < _capacity; ++i)
    {
        DANode *newNode = new DANode(_start_x, _start_y, scale_x, scale_y, 0, std::to_string(i), &font, &Colors);
        _arr[i] = newNode;
    }

    this->animationDA->updateNodePosition(_arr, 0, _start_x, _start_y, nodeDistanceX);
}

void DA::resizeArray(int newSize, int start_idx)
{
    /* Create new array */
    resizing = 1;
    new_arr_size = arr_size;
    new_capacity = newSize;
    createArray(new_arr, new_capacity, start_x, start_y + 2 * nodeDistanceY - 30.f * scale_y);

    this->animationDA->instructions.push_back(
        {[this, start_idx]()
            { this->animationDA->showThings(show_new_arr, show_new_label, {0 + start_idx}); }});

    /* Copy data */
    for (int i = 0; i < arr_size; ++i) {
        std::vector<std::function<void()>> vec(
            {[this, i, start_idx]()
                { this->animationDA->highlightCurrentNode(arr[i], "i/" + std::to_string(i), "normalFillColor", "currentColor", {1 + start_idx}); },

                [this, i, start_idx]()
                { this->animationDA->highlightCurrentNode(new_arr[i], "i/" + std::to_string(i), "normalFillColor", "currentColor", {1 + start_idx}); }

            });
        if (i > 0)
        {   
            /* Unhighlight last node */
            vec.push_back(
                [this, i, start_idx]() 
                { this->animationDA->highlightCurrentNode(arr[i - 1], std::to_string(i - 1), "normalFillColor", "newColor", {1 + start_idx}); });

            vec.push_back(
                [this, i, start_idx]() 
                { this->animationDA->highlightCurrentNode(new_arr[i - 1], std::to_string(i - 1), "normalFillColor", "newColor", {1 + start_idx}); });
        }
    
        this->animationDA->instructions.push_back(vec);

        /* new_arr[i] = arr[i] */
        this->animationDA->instructions.push_back({[this, i, start_idx]()
                                                    { this->animationDA->changeNodeValue(new_arr[i], arr[i]->val, {2 + start_idx}); }});
    }

    /* arr = arr2 */
    int tmp_sz = arr_size; /* Because arr_size will be modified later by 'this' */
    this->animationDA->instructions.push_back(
        {[this, start_idx]()
            { this->animationDA->moveArray(new_arr, new_arr_size, start_x, start_y, nodeDistanceX, {3 + start_idx}); },

            [this]()
            { this->animationDA->hideThings(show_arr, show_new_label); }});
}

/* Update and Render */
void DA::updateButtons()
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

void DA::updateChildButtons()
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

void DA::updateInputFields()
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

void DA::updateSFMLEvents(const float &dt, sf::Event &event)
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
                //     this->inputWarning.setString("Sorry, the animationDA is running...");
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

void DA::update(const float &dt)
{
    this->window->setTitle("Dynamic Array");
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

void DA::renderButtons(sf::RenderTarget *target)
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

void DA::renderNode(sf::RenderTarget *target)
{
    if (show_arr) {
        for (int i = 0; i < capacity; ++i)
        {
            arr[i]->render(target);
        }
    }

    if (show_new_arr) {
        for (int i = 0; i < new_capacity; ++i)
        {
            new_arr[i]->render(target);
        }
    }
}

void DA::renderAnimation()
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
        this->animationDA->play(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "pause")
    {
        this->animationDA->pause(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "prev")
    {
        this->animationDA->prev(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "next")
    {
        this->animationDA->next(this->doneAnimation, &this->stepText);
        if (this->doneAnimation && this->animationDA->curIndex == (int)this->animationDA->instructions.size())
        {
            enable_replayButton();
        }
        return;
    }

    if (this->runState == "first")
    {
        this->animationDA->first(this->doneAnimation, &this->stepText);
        return;
    }

    if (this->runState == "last")
    {
        this->animationDA->last(this->doneAnimation, &this->stepText);
        if (this->doneAnimation)
        {
            enable_replayButton();
        }
        return;
    }
}

void DA::renderHighlightCode(sf::RenderTarget *target)
{
    codeHighlight->render(target);
}

void DA::render(sf::RenderTarget *target)
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

    arr_name.setFillColor(Colors["labelColor"]);
    new_arr_name.setFillColor(Colors["labelColor"]);

    if (show_label > 0 && capacity > 0)
        target->draw(arr_name);
    if (show_new_label > 0 && new_capacity > 0)
        target->draw(new_arr_name);

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

void DA::reset()
{
    prepareNewInstruction();
    this->DeleteArrayPointers(this->arr);
    this->DeleteArrayPointers(this->new_arr);

    arr_size = 0;
    capacity = 0;
    show_label = 1;
    show_arr = 1;

    new_arr_size = 0;
    new_capacity = 0;
    show_new_label = 0;
    show_new_arr = 0;

    resizing = 0;

    delete (this->animation); /* Also deleted animationDA */

    this->codeHighlight->reset();
    this->animation = new AnimationDA(&this->Colors, (float)(maxSpeed * speedGap) / currentSpeed, codeHighlight);
    this->animationDA = dynamic_cast<AnimationDA *>(this->animation);
    assert(animationDA != nullptr);

    this->newStepTriggered = 0;
    this->doneAnimation = 1;
    this->choosingButton = "";
    this->choosingChildButton = "";
    inputWarning.setString("");
    inputGuide.setString("");
    exist.assign((int)exist.size(), 0);
    runState = "";
}

void DA::ReInitButtons()
{
    this->buttons["Access"] = new Button(
            insButtonX, insButtonY + 6 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Access", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);


    delete(this->buttons["Quit"]);
    this->buttons["Quit"] = new Button(
            insButtonX, insButtonY + 7 * insButtonDistanceY, scale_x, scale_y,
            &this->font, "Quit", characterSize,
            sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(240, 180, 10, 255),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(150, 150, 150, 255),
            1, insButtonSizeX, insButtonSizeY);
}


