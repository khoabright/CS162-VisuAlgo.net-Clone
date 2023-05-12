#include "InputField.h"

void InputField::handleTextEntered(sf::Uint32 unicode)
{
    if (unicode == '\b')
    { // If the backspace key is pressed
        if (!this->inputString.empty())
        {
            this->inputString.pop_back();
        }
    }
    else if ((unicode >= 48 && unicode <= 57) || (unicode == 44)) // numbers or comma only
    {
        this->inputString += static_cast<char>(unicode);
    }

    /* Check for full box */
    sf::Text tmp = this->inputText;
    tmp.setString(this->inputString);

    if (tmp.getGlobalBounds().left + tmp.getGlobalBounds().width < this->shape.getGlobalBounds().left + this->shape.getGlobalBounds().width)
        this->inputText.setString(this->inputString);
    else
    {
        if (!this->inputString.empty())
        {
            this->inputString.pop_back();
        }
    }
}

void InputField::updateSFMLEvents(const float &dt, sf::Event &event, std::string &return_value, bool& hitEnter)
{
    if (this->disabled) return;
    if (event.type == sf::Event::TextEntered)
    {
        handleTextEntered(event.text.unicode);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        hitEnter = 1;
        return_value = this->inputString;

        // Clear the input string and update the input text
        this->inputString.clear();
        this->inputText.setString(this->inputString);
    }
}

void InputField::update(const sf::Vector2f mousePos)
{   
    if (this->disabled) return;
    float x = this->shape.getPosition().x;
    float y = this->shape.getPosition().y;
    float width = this->shape.getGlobalBounds().width;
    float height = this->shape.getGlobalBounds().height;

    this->cursor.setPosition(x + this->inputText.getGlobalBounds().width + 2.f / standard_x * width * scale_x,
                             y + 5.f / standard_y * height * scale_y);

    if (blinkClock.getElapsedTime().asSeconds() >= 0.5)
    {
        showCursor = !showCursor;
        blinkClock.restart();
    }
}

void InputField::render(sf::RenderTarget *target)
{   
    if (this->disabled) return;
    target->draw(this->shape);
    target->draw(this->inputText);
    if (this->showCursor)
        target->draw(this->cursor);
}

InputField::InputField(float x, float y, float width, float height, float scale_x, float scale_y,
                       sf::Font *font, int textSize,
                       sf::Color textColor, sf::Color shapeOutlineColor, sf::Color shapeFillColor)
{
    // Every sizes is scaled

    this->window = window;
    this->scale_x = scale_x;
    this->scale_y = scale_y;

    this->font = font;

    textSize = standard_characterSize_ratio * height;
    this->inputText.setFont(*font);
    this->inputText.setCharacterSize(textSize);
    this->inputText.setFillColor(sf::Color::White);
    this->inputText.setPosition(sf::Vector2f(x + 2.f / standard_x * width * scale_x, y + 2.f / standard_y * height * scale_y));

    this->message.setPosition(sf::Vector2f(x, y));

    this->shape.setOutlineThickness(2);
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));
    this->shape.setOutlineColor(shapeOutlineColor);
    this->shape.setFillColor(shapeFillColor);

    this->cursor.setSize(sf::Vector2f(2.f, height - 10.f / standard_y * height * scale_y)); // standard width, height = (100, 50)
    cursor.setFillColor(sf::Color::White);
    cursor.setOrigin(0, 1);
}

InputField::~InputField()
{
}