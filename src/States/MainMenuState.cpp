#include "MainMenuState.h"

// Initializer functions
void MainMenuState::initVariables()
{
    /* Scale sizes */
    start_x *= scale_x;
    start_y *= scale_y;
    distance_x *= scale_x;
    distance_y *= scale_x;
    characterSize *= scale_x * scale_y;
}

void MainMenuState::initBackground()
{
    this->background.setSize(sf::Vector2f(this->window->getSize().x, this->window->getSize().y));

    if (!this->backgroundTexture.loadFromFile("Resources/Images/menu_background.png"))
    {
        throw "ERROR::MAIN MENU STATE::FAILED TO LOAD BACKGROUND TEXTURE";
    }
    this->background.setTexture(&this->backgroundTexture);
}

void MainMenuState::initFonts()
{
    if (!this->font.loadFromFile("Resources/Fonts/Dosis-Light.ttf"))
    {
        throw("ERROR::MAIN MENU STATE::COULD NOT LOAD FONT");
    }
}

void MainMenuState::initKeybinds()
{
    std::ifstream ifs("Config/mainMenuState_keybinds.ini");

    if (ifs.is_open())
    {
        std::string key = "";
        std::string key2 = "";

        while (ifs >> key >> key2)
        {
            this->keybinds[key] = this->supportedKeys->at(key2);
        }
    }

    ifs.close();
}

void MainMenuState::initButtons()
{

    this->buttons["Static Array"] = new Button(
        start_x + 0 * distance_x, start_y + 0 * distance_y, scale_x, scale_y,
        &this->font, "Static Array", characterSize,
        sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
        0
    ); 

    this->buttons["Dynamic Array"] = new Button(
        start_x + 0 * distance_x, start_y + 1 * distance_y, scale_x, scale_y,
        &this->font, "Dynamic Array", characterSize,
        sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
        0
    );

    this->buttons["Stack"] = new Button(
        start_x + 0 * distance_x, start_y + 2 * distance_y, scale_x, scale_y,
        &this->font, "Stack", characterSize,
        sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
        0
    );

    this->buttons["Queue"] = new Button(
        start_x + 0 * distance_x, start_y + 3 * distance_y, scale_x, scale_y,
        &this->font, "Queue", characterSize,
        sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
        0
    );

    this->buttons["Singly Linked List"] = new Button(
        start_x + 1 * distance_x, start_y + 0 * distance_y, scale_x, scale_y,
        &this->font, "Singly Linked List", characterSize,
        sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
        0
    );

    this->buttons["Doubly Linked List"] = new Button(
        start_x + 1 * distance_x, start_y + 1 * distance_y, scale_x, scale_y,
        &this->font, "Doubly Linked List", characterSize,
        sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0), 
        0
    );

    this->buttons["Circular Linked List"] = new Button(
        start_x + 1 * distance_x, start_y + 2 * distance_y, scale_x, scale_y,
        &this->font, "Circular Linked List", characterSize,
        sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
        0
    );

    this->buttons["Quit"] = new Button(
        start_x + 1 * distance_x, start_y + 3 * distance_y, scale_x, scale_y,
        &this->font, "Quit", characterSize,
        sf::Color(255, 255, 255, 255), sf::Color(240, 180, 10, 255), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
        0
    );
}

MainMenuState::MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states)
    : State(window, supportedKeys, states)
{
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initKeybinds();
    this->initButtons();
}

MainMenuState::~MainMenuState()
{
    auto it = this->buttons.begin();
    for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
    {
        delete it->second;
    }
}

void MainMenuState::updateInput(const float &dt)
{
}


void MainMenuState::updateCursor()
{   
    /* Update all the buttons */
    isHandCursor = false;
    for (auto &it : this->buttons)
    {
        it.second->update(isHandCursor);
    }

    /*  Show Mouse Cursor */
    if (isHandCursor)
    {
        this->window->setMouseCursor(handCursor);
    }
    else
    {
        this->window->setMouseCursor(arrowCursor);
    }
}

void MainMenuState::updateButtons()
{
    // SLL
    if (this->buttons["Singly Linked List"]->isPressed())
    {
        this->states->push(new SLL(this->window, this->supportedKeys, this->states));
    }

    //DLL
    if (this->buttons["Doubly Linked List"]->isPressed())
    {
        this->states->push(new DLL(this->window, this->supportedKeys, this->states));
    }

    //CLL
    if (this->buttons["Circular Linked List"]->isPressed())
    {
        this->states->push(new CLL(this->window, this->supportedKeys, this->states));
    }

    //Static Array
    if (this->buttons["Static Array"]->isPressed())
    {
        this->states->push(new SA(this->window, this->supportedKeys, this->states));
    }

    //Dynamic Array
    if (this->buttons["Dynamic Array"]->isPressed())
    {
        this->states->push(new DA(this->window, this->supportedKeys, this->states));
    }

    //Stack
    if (this->buttons["Stack"]->isPressed())
    {
        this->states->push(new Stack(this->window, this->supportedKeys, this->states));
    }

    if (this->buttons["Queue"]->isPressed())
    {
        this->states->push(new Queue(this->window, this->supportedKeys, this->states));
    }

    // Quit the game
    if (this->buttons["Quit"]->isPressed())
    {
        this->endState();
    }
}

void MainMenuState::updateSFMLEvents(const float &dt, sf::Event& event)
{
    for (auto &it : this->buttons)
    {
        it.second->updateSFMLEvents(dt, event, this->mousePosView);
    }
}

void MainMenuState::update(const float &dt)
{
    this->window->setTitle("Data Structure Visualization");
    this->updateMousePositions();
    this->updateInput(dt);
    this->updateCursor();
    this->updateButtons();
}

void MainMenuState::renderButtons(sf::RenderTarget *target)
{
    /* Render all buttons */
    for (auto &it : this->buttons)
    {
        it.second->render(target);
    }
}

void MainMenuState::render(sf::RenderTarget *target)
{
    if (!target)
    {
        target = this->window;
    }
    target->draw(this->background);
    this->renderButtons(target);
}
