#ifndef SA_H
#define SA_H

#pragma once

#include "DataStructure.h"
#include "SANode.h"

class SA : public DataStructure
{
private:
    // Variables
    float start_x = 90, start_y = 110;
    int maxCapacity = 20;
    int arr_size = 0;
    int capacity = 0;

    float nodeDistanceX = 58.f;
    float nodeDistanceY = 100.f;

    AnimationSA *animationSA;
    std::vector<SANode *> arr;
    sf::Text arr_name;
    
    // Child Buttons
    float childButtonOriginX = 125.f;
    float childButtonOriginY = 545.f; // last is 375 
    float childButtonWidth = 80.f;
    float childButtonHeight = 25.f;
    float childButtonCharacterSize = 16.f;
    float childButtonDistanceX = 5.f;
    float childButtonDistanceY = 5.f;

    // Input Fields
    float inputFieldOriginX = 125.f;
    float inputFieldOriginY = 603.f; // last is 433
    float inputFieldWidth = 80.f;
    float inputFieldHeight = 25.f;
    float inputFieldCharacterSize = 16.f;
    float inputFieldDistanceX = 5.f;
    float inputFieldDistanceY = 5.f;

    // Buttons
    void ReInitButtons();
    void initTriggerFunction();
    void initChildButtons();
    void initInputFields();
    void initAnimation();
    void initHighlightCode();

    // Instruction Buttons
    void button_theme();
    void button_initialize();
    void button_add();
    void button_delete();
    void button_update();
    void button_search();
    void button_access();
    void button_quit();

    // Add
    void addHead(int nodeValue);
    void addTail(int nodeValue);
    void addMiddle(int index, int nodeValue);

    // Delete
    void deleteHead();
    void deleteTail();
    void deleteMiddle(int index);

    //Update
    void updateNode(int index, int newValue);
    void searchNode(int searchValue);
    void accessNode(int index);

    void prepareNewInstruction();
    
    
public:
    SA(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states);
    ~SA();

    //Reset
    void DeleteArrayPointers();

    // Functions
    // Init

    // Update
    void updateButtons();
    void updateChildButtons();
    void updateInputFields();
    void updateSFMLEvents(const float &dt, sf::Event &event);
    void update(const float &dt);
    void renderButtons(sf::RenderTarget *target = NULL);
    void renderNode(sf::RenderTarget *target);
    void renderAnimation();
    void renderHighlightCode(sf::RenderTarget *target);
    void render(sf::RenderTarget *target = NULL);
    void reset();
};

#endif