#include "SLLNode.h"

SLLNode::SLLNode()
{
}

SLLNode::SLLNode(float x, float y, float scale_x, float scale_y, int _val, sf::Font *_font, std::map<std::string, sf::Color> *Colors)
{
    this->Colors = Colors;
    /* parameters are already scaled */

    /* just scale predeclared sizes */
    auto scaleSize = [&]()
    {
        this->radius *= scale_x;
        this->standard_width *= scale_x;
        this->standard_height *= scale_y;
    };

    scaleSize();

    //Variables
    newNext(nullptr);
    nextNext();

    newVal(_val);
    nextVal();

    this->font = _font;
    this->scale_x = scale_x;
    this->scale_y = scale_y;
    this->x = x;
    this->y = y;
    this->x_center = x + this->radius;
    this->y_center = y + this->radius;
    newPos(sf::Vector2f(x, y));
    nextPos();

    //Shape
    shape.setRadius(this->radius);
    this->width = shape.getGlobalBounds().width;
    this->height = shape.getGlobalBounds().height;

    shape.setPosition(x, y);
    shape.setOutlineThickness(3);


    //Color
    this->setNormalColor();
    newColor("normalFillColor", "normalOutlineColor");
    nextColor();

    //Arrow
    arrow_img.loadFromFile("Resources/Images/rightArrow.png");
    arrow.setTexture(&arrow_img);
    arrow.setSize(sf::Vector2f(100.f, 40.f));

    //Text
    text.setFont(*font);
    text.setCharacterSize(25 * scale_x * scale_y);
    text.setFillColor(Colors->at("characterColor"));
    
    //Label
    labelColor = Colors->at("labelColor");
    labelText.setFont(*font);
    labelText.setCharacterSize(20 * scale_x * scale_y);
    labelText.setFillColor(this->labelColor);
    labelText.setStyle(sf::Text::Bold);
    newLabel("");
    nextLabel();

    this->updateText();
}

void SLLNode::reset()
{   
    this->x_center = this->x + this->radius;
    this->y_center = this->y + this->radius;

    array_fillColor.clear();
    array_outlineColor.clear();
    array_label.clear();
    array_next.clear();
    array_val.clear();
    array_pos.clear();

    idx_fillColor = -1;
    idx_outlineColor = -1;
    idx_label = -1;
    idx_next = -1;
    idx_val = -1;
    idx_pos = -1;

    this->setNormalColor();

    newColor("normalFillColor", "normalOutlineColor");
    nextColor();

    newLabel(this->labelString);
    nextLabel();

    newNext(this->next);
    nextNext();

    newVal(this->val);
    nextVal();

    newPos(sf::Vector2f(this->x, this->y));
    nextPos();
}

/* Update along time */

//Next
void SLLNode::updateNext()
{
    this->next = array_next[idx_next];
}

void SLLNode::newNext(SLLNode *new_next)
{
    if (this->idx_next == (int)this->array_next.size() - 1) {
        this->array_next.push_back(new_next);
    }
}

void SLLNode::prevNext()
{
    this->idx_next--;
    this->updateNext();
}

void SLLNode::nextNext()
{
    this->idx_next++;
    this->updateNext();
}

//Val
void SLLNode::updateVal()
{
    this->val = array_val[idx_val];
}

void SLLNode::newVal(int new_val)
{
    if (this->idx_val == (int)this->array_val.size() - 1) {
        this->array_val.push_back(new_val);
    }
}

void SLLNode::prevVal()
{   
    this->idx_val--;
    this->updateVal();
}

void SLLNode::nextVal()
{
    this->idx_val++;
    this->updateVal();
}


//Color
void SLLNode::setNormalColor()
{
    this->shape.setOutlineColor(this->Colors->at("normalOutlineColor"));
    this->shape.setFillColor(this->Colors->at("normalFillColor"));
}

void SLLNode::newColor(std::string fillColor, std::string outlineColor)
{
    assert(this->idx_fillColor == this->idx_outlineColor);
    if (this->idx_fillColor == (int)this->array_fillColor.size() - 1) {
        this->array_fillColor.push_back(fillColor);
        this->array_outlineColor.push_back(outlineColor);
    }
}

void SLLNode::updateColor()
{
    this->shape.setFillColor(Colors->at(array_fillColor[idx_fillColor]));
    this->shape.setOutlineColor(Colors->at(array_outlineColor[idx_outlineColor]));
    this->labelColor = Colors->at("labelColor");
    this->text.setFillColor(Colors->at("characterColor"));
    this->labelText.setFillColor(labelColor);
}

void SLLNode::prevColor()
{
    this->idx_fillColor--;
    this->idx_outlineColor--;
    this->updateColor();
}

void SLLNode::nextColor()
{
    this->idx_fillColor++;
    this->idx_outlineColor++;
    this->updateColor();
}

//Text & Label
void SLLNode::updateText()
{
    this->text.setString(std::to_string(val));
    float cur_x = shape.getPosition().x;
    float cur_y = shape.getPosition().y;
    float cur_w = shape.getGlobalBounds().width;
    float cur_h = shape.getGlobalBounds().height;

    this->text.setPosition(cur_x + cur_w / 2 - this->text.getGlobalBounds().width / 2 - 3.f * cur_w / this->standard_width,
                           cur_y + cur_h / 2 - this->text.getGlobalBounds().height / 2 - 8.f * cur_h / this->standard_height);
}

void SLLNode::newLabel(std::string new_label)
{
    if (this->idx_label == (int)this->array_label.size() - 1) {
        this->array_label.push_back(new_label);
    }
}

void SLLNode::updateLabel()
{   
    float cur_x = shape.getPosition().x;
    float cur_y = shape.getPosition().y;
    float cur_w = shape.getGlobalBounds().width;
    float cur_h = shape.getGlobalBounds().height;

    this->labelString = array_label[idx_label];
    this->labelText.setString(this->labelString);

    this->labelText.setPosition(cur_x + cur_w / 2 - labelText.getGlobalBounds().width / 2,
                                cur_y + cur_h + 5.f * cur_h / this->standard_height);
}

void SLLNode::prevLabel()
{
    this->idx_label--;
    this->updateLabel();
}

void SLLNode::nextLabel()
{
    this->idx_label++;
    this->updateLabel();
}

// Pos
void SLLNode::updatePos()
{   
    this->x = array_pos[idx_pos].x;
    this->y = array_pos[idx_pos].y;
    this->x_center = this->x + this->radius;
    this->y_center = this->y + this->radius;

    this->shape.setPosition(this->x, this->y);
}

void SLLNode::newPos(sf::Vector2f new_pos)
{
    if (this->idx_pos == (int)this->array_pos.size() - 1) {
        this->array_pos.push_back(new_pos);
    }
}

void SLLNode::prevPos()
{
    this->idx_pos--;
    this->updatePos();
}

void SLLNode::nextPos()
{
    this->idx_pos++;
    this->updatePos();
}

//Render
void SLLNode::renderAnimation()
{
}

void SLLNode::render(sf::RenderTarget *target)
{
    this->updateText();
    this->updateLabel();

    if (this->showNode) {
        target->draw(this->shape);
        target->draw(this->text);
    }
    if (this->showArrow)
        target->draw(this->arrow);

    if (this->showLabel) 
        target->draw(this->labelText);
}
