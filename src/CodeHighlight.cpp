#include "CodeHighlight.h"

CodeHighlight::CodeHighlight(float scale_x, float scale_y, std::map<std::string, sf::Color> *Colors)
{
    this->scale_x = scale_x;
    this->scale_y = scale_y;
    this->Colors = Colors;
    font.loadFromFile("Resources/Fonts/CONSOLA.TTF");

    auto scaleSize = [&]() {
        background_x *= scale_x;
        background_y *= scale_y;
        background_width *= scale_x;
        background_height *= scale_y;

        lineDistance *= scale_y;
        text_start_x *= scale_x;
        text_start_y *= scale_y;
        characterSize *= scale_x * scale_y;
    };

    scaleSize();

    introText.setCharacterSize((int)(characterSize + 3 * scale_y));
    introText.setFillColor(characterIntroColor);
    introText.setFont(font);
    introText.setPosition(text_start_x, background_y + 7 * scale_y);

    introBackground.setSize(sf::Vector2f(background_width, lineDistance + 10 * scale_y));
    introBackground.setPosition(background_x, background_y);
    introBackground.setFillColor(introBackgroundColor);

    background.setFillColor(backgroundColor);
    background.setSize(sf::Vector2f(background_width, background_height));
    background.setPosition(background_x, background_y);

    reset();
}

CodeHighlight::~CodeHighlight()
{
}

void CodeHighlight::reset()
{   
    idx_currentLines = -1;
    highlightBlock.clear();
    codeStrings.clear();
    codes.clear();
    currentLines.clear();
    array_currentLines.clear();
    introText.setString("");
    next_currentLines({});
}

void CodeHighlight::switchPanel()
{
    this->isHiding ^= 1;
}

void CodeHighlight::formatText(sf::Text *text, int idx)
{
    text->setCharacterSize(characterSize);
    text->setOutlineColor(characterColor);
    text->setFillColor(characterColor);
    text->setPosition(text_start_x, text_start_y + idx * lineDistance);
    text->setFont(font);
}

void CodeHighlight::prev_currentLines()
{
    this->idx_currentLines--;
    this->currentLines = array_currentLines[idx_currentLines];
    this->updateHighlight();
}

void CodeHighlight::next_currentLines(std::vector<int> new_currentLines)
{
    if (this->idx_currentLines == (int)this->array_currentLines.size() - 1) {
        this->array_currentLines.push_back(new_currentLines);
    }
    this->idx_currentLines++;
    this->currentLines = array_currentLines[idx_currentLines];
    this->updateHighlight();
}

void CodeHighlight::updateTexts()
{   
    int idx = -1;
    for (auto &str : this->codeStrings) {
        ++idx;
        sf::Text newText;
        this->formatText(&newText, idx);
        newText.setString(str);
        this->codes.push_back(newText);
    }

    highlightBlock.resize(idx + 1);
    for (int i = 0; i <= idx; ++i) {
        highlightBlock[i].setSize(sf::Vector2f(background_width, lineDistance));
        highlightBlock[i].setPosition(background_x, lineDistance + 10 * scale_y + background_y + i * lineDistance);
        highlightBlock[i].setFillColor(backgroundColor);
    }
}

void CodeHighlight::resetHighlight()
{   
    assert(codes.size() == highlightBlock.size());
    for (int i = 0; i < (int)codes.size(); ++i) {
        highlightBlock[i].setFillColor(backgroundColor);
    }
}

void CodeHighlight::updateHighlight()
{
    resetHighlight();
    for (int i : currentLines) {
        highlightBlock[i].setFillColor(highlightColor);
    }
}

void CodeHighlight::update()
{   
    updateTexts();
    updateHighlight();
}

void CodeHighlight::render(sf::RenderTarget *target)
{
    if (this->isHiding) return;
    target->draw(background);
    target->draw(introBackground);
    for (auto highlight : highlightBlock) target->draw(highlight);

    target->draw(introText);
    for (auto code : codes) target->draw(code);
}
