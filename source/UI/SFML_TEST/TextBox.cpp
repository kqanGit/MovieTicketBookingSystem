#include "TextBox.h"

TextBox::TextBox(float x, float y, float w, float h, bool isPassword)
    : isSelected(false), isPassword(isPassword)
{
    // font không load ở đây nữa
    box.setSize(sf::Vector2f(w, h));
    box.setFillColor(sf::Color(230, 230, 230));
    box.setOutlineThickness(2);
    box.setOutlineColor(sf::Color::Black);
    box.setPosition(x, y);

    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x + 10, y + 10);
}

void TextBox::setFont(const sf::Font& f) {
    font = f;  // copy font, hoặc tốt hơn nên lưu con trỏ
    text.setFont(font);
}

void TextBox::setSize(float w, float h) {
    box.setSize(sf::Vector2f(w, h));
}

void TextBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

void TextBox::handleEvent(const sf::Event& event) {
    if (!isSelected) return;

    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) {  // Backspace
            if (!inputString.empty()) inputString.pop_back();
        } else if (event.text.unicode >= 32 && event.text.unicode <= 126) {
            inputString += static_cast<char>(event.text.unicode);
        }

        if (isPassword) {
            text.setString(std::string(inputString.length(), '*'));
        } else {
            text.setString(inputString);
        }
    }
}

void TextBox::setSelected(bool selected) {
    isSelected = selected;
    box.setOutlineColor(selected ? sf::Color::Blue : sf::Color::Black);
}

void TextBox::setPosition(float x, float y) {
    box.setPosition(x, y);
    text.setPosition(x + 10, y + 10);
}

std::string TextBox::getText() const {
    return inputString;
}

void TextBox::clear() {
    inputString.clear();
    text.setString("");
}
