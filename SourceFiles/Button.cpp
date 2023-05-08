#include "Button.h"

Button::Button(float x, float y, float scale_x, float scale_y,
               sf::Font *font, std::string text, int textSize,
               sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
               sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
               bool visible_background, float width, float height)
{
    this->buttonState = BTN_IDLE;

    this->font = font;
    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setFillColor(textIdleColor);
    this->text.setCharacterSize(textSize);
    this->text.setPosition(x, y);

    // this->shape.setPosition(sf::Vector2f(x, y));


    if (!visible_background)
    {
        this->shape.setFillColor(idleColor);
        this->shape.setSize(sf::Vector2f(this->text.getGlobalBounds().width, this->text.getGlobalBounds().height));
        this->shape.setPosition(x, y + 10 * scale_y);
    }

    else {
        // set background wider a bit
        this->shape.setFillColor(idleColor);
        this->shape.setSize(sf::Vector2f(width, height));
        this->shape.setPosition(x, y);
        this->text.setPosition(x + width / 2.f - this->text.getGlobalBounds().width / 2.f, 
                               y + height / 2.f - this->text.getGlobalBounds().height / 2.f);
    }

    this->textIdleColor = textIdleColor;
    this->textHoverColor = textHoverColor;
    this->textActiveColor = textActiveColor;

    this->idleColor = idleColor;
    this->hoverColor = hoverColor;
    this->activeColor = activeColor;

    this->shape.setFillColor(this->idleColor);
}

Button::~Button()
{
}

// Accessors
bool Button::isPressed() 
{
    if (this->buttonState == BTN_ACTIVE)
    {   
        // Avoid multiple clicks
        this->buttonState = BTN_HOVER;

        return true;
    }

    return false;
}

// Functions

void Button::updateText()
{
    // this->text.setPosition(x + width / 2.f - this->text.getGlobalBounds().width / 2.f, 
    //                            y + height / 2.f - this->text.getGlobalBounds().height / 2.f);
}

void Button::updateSFMLEvents(const float &dt, sf::Event &event, const sf::Vector2f mousePos)
{   
    if (this->disabled) return;
    /* Update the booleans for hover and pressed */

    // IDLE
    this->buttonState = BTN_IDLE;

    // Hover
    if (this->shape.getGlobalBounds().contains(mousePos))
    {
        this->buttonState = BTN_HOVER;

        // Pressed
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
                this->buttonState = BTN_ACTIVE;
        }
    }
}

void Button::update(bool &isHandCursor)
{   
    if (this->disabled) return;
    switch (this->buttonState)
    {
    case BTN_IDLE:
        this->shape.setFillColor(this->idleColor);
        this->text.setFillColor(this->textIdleColor);
        break;

    case BTN_HOVER:
        this->shape.setFillColor(this->hoverColor);
        this->text.setFillColor(this->textHoverColor);
        break;

    case BTN_ACTIVE:
        this->shape.setFillColor(this->activeColor);
        this->text.setFillColor(this->textActiveColor);
        break;
    default:
        this->shape.setFillColor(sf::Color::Red);
        this->text.setFillColor(sf::Color::Blue);
        break;
    }

    /* Update mouse cursor */
    if (buttonState != BTN_IDLE)
    {
        isHandCursor = true;
    }
}

void Button::render(sf::RenderTarget *target)
{   
    if (this->disabled) return;
    target->draw(this->shape);
    target->draw(this->text);
}
