#include "SANode.h"

SANode::SANode()
{
}

SANode::SANode(float x, float y, float scale_x, float scale_y, int _val, std::string newLabelString, sf::Font *_font, std::map<std::string, sf::Color> *Colors)
{
    this->Colors = Colors;
    /* parameters are already scaled */

    /* just scale predeclared sizes */
    auto scaleSize = [&]()
    {
        this->side *= scale_x;
        this->standard_width *= scale_x;
        this->standard_height *= scale_y;
    };

    scaleSize();

    //Variables
    newVal(_val);
    nextVal();

    this->font = _font;
    this->scale_x = scale_x;
    this->scale_y = scale_y;
    this->x = x;
    this->y = y;

    //Shape
    shape.setSize(sf::Vector2f(side, side));
    this->width = shape.getGlobalBounds().width;
    this->height = shape.getGlobalBounds().height;

    shape.setPosition(x, y);
    shape.setOutlineThickness(4);

    //Color
    newColor("normalFillColor", "emptyColor");
    nextColor();

    //Text
    text.setFont(*font);
    text.setCharacterSize(30 * scale_x * scale_y);
    text.setFillColor(Colors->at("characterColor"));
    
    //Label
    labelColor = Colors->at("labelColor");
    labelText.setFont(*font);
    labelText.setCharacterSize(20 * scale_x * scale_y);
    labelText.setFillColor(this->labelColor);
    labelText.setStyle(sf::Text::Bold);
    newLabel(newLabelString);
    nextLabel();

    this->updateText();
}

void SANode::reset()
{   
    std::string curFillColor = array_fillColor[idx_fillColor];
    std::string curOutlineColor = array_outlineColor[idx_outlineColor];

    array_fillColor.clear();
    array_outlineColor.clear();
    array_label.clear();
    array_val.clear();

    idx_fillColor = -1;
    idx_outlineColor = -1;
    idx_label = -1;
    idx_val = -1;

    newColor(curFillColor, curOutlineColor);
    nextColor();

    newLabel(this->labelString);
    nextLabel();

    newVal(this->val);
    nextVal();
}

/* Update along time */


//Val
void SANode::updateVal()
{
    this->val = array_val[idx_val];
}

void SANode::newVal(int new_val)
{
    if (this->idx_val == (int)this->array_val.size() - 1) {
        this->array_val.push_back(new_val);
    }
}

void SANode::prevVal()
{   
    this->idx_val--;
    this->updateVal();
}

void SANode::nextVal()
{
    this->idx_val++;
    this->updateVal();
}


//Color
void SANode::setNormalColor()
{
    this->shape.setOutlineColor(this->Colors->at("emptyColor"));
    this->shape.setFillColor(this->Colors->at("normalFillColor"));
}

void SANode::newColor(std::string fillColor, std::string outlineColor)
{
    assert(this->idx_fillColor == this->idx_outlineColor);
    if (this->idx_fillColor == (int)this->array_fillColor.size() - 1) {
        this->array_fillColor.push_back(fillColor);
        this->array_outlineColor.push_back(outlineColor);
    }
}

void SANode::updateColor()
{
    this->shape.setFillColor(Colors->at(array_fillColor[idx_fillColor]));
    this->shape.setOutlineColor(Colors->at(array_outlineColor[idx_outlineColor]));
    this->labelColor = Colors->at("labelColor");
    this->text.setFillColor(Colors->at("characterColor"));
    this->labelText.setFillColor(labelColor);
}

void SANode::prevColor()
{
    this->idx_fillColor--;
    this->idx_outlineColor--;
    this->updateColor();
}

void SANode::nextColor()
{
    this->idx_fillColor++;
    this->idx_outlineColor++;
    this->updateColor();
}

//Text & Label
void SANode::updateText()
{
    if (val > 0) this->text.setString(std::to_string(val));
    else this->text.setString("");

    float cur_x = shape.getPosition().x;
    float cur_y = shape.getPosition().y;
    float cur_w = shape.getGlobalBounds().width;
    float cur_h = shape.getGlobalBounds().height;

    this->text.setPosition(cur_x + cur_w / 2 - this->text.getGlobalBounds().width / 2 - 2.f * cur_w / this->standard_width,
                           cur_y + cur_h / 2 - this->text.getGlobalBounds().height / 2 - 8.f * cur_h / this->standard_height);
}

void SANode::newLabel(std::string new_label)
{
    if (this->idx_label == (int)this->array_label.size() - 1) {
        this->array_label.push_back(new_label);
    }
}

void SANode::updateLabel()
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

void SANode::prevLabel()
{
    this->idx_label--;
    this->updateLabel();
}

void SANode::nextLabel()
{
    this->idx_label++;
    this->updateLabel();
}

void SANode::render(sf::RenderTarget *target)
{
    this->updateText();
    this->updateLabel();

    if (this->showNode) {
        target->draw(this->shape);
        target->draw(this->text);
    }

    if (this->showLabel) 
        target->draw(this->labelText);
}
