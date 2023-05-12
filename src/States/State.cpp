#include "State.h"

State::State(sf::RenderWindow *window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
{   
    handCursor.loadFromSystem(sf::Cursor::Hand);
    arrowCursor.loadFromSystem(sf::Cursor::Arrow);

    this->window = window;
    float width = this->window->getSize().x;
    float height = this->window->getSize().y;
    // this->scale_x = width / 800.f;
    // this->scale_y = height / 600.f;

    this->scale_x = width / 1280.f;
    this->scale_y = height / 800.f;

    this->scale_xy = (width * height) / (1280.f * 800.f);

    randomize.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    this->supportedKeys = supportedKeys;
    this->states = states;
    this->quit = false;
}

State::~State()
{
}

const bool &State::getQuit() const
{
    return this->quit;
}

void State::endState()
{
    this->quit = true;
}

void State::updateMousePositions()
{
    this->mousePosScreen = sf::Mouse::getPosition();
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}
