#include "Button.h"

Button::Button(const std::string& label, const sf::Font& font,
               float x, float y, float width, float height)
{
    // shape.setPosition(x, y);
    // shape.setSize(sf::Vector2f(width, height));
    // shape.setFillColor(sf::Color(100, 149, 237)); // cornflower blue
    // shape.setOutlineColor(sf::Color::White);
    // shape.setOutlineThickness(1);

    shape.setPosition(x, y);
    shape.setSize({width, height});
    shape.setCornersRadius(25.f); // Bo góc bán kính 15 px
    shape.setFillColor(sf::Color(100, 149, 237));
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1);


    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);

    // Canh giữa chữ
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    text.setPosition(x + width / 2, y + height / 2);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (shape.getGlobalBounds().contains(mousePos))
            return true;
    }
    return false;
}

void Button::setFillColor(const sf::Color& color) {
    shape.setFillColor(color);
}

void Button::setTextColor(const sf::Color& color) {
    text.setFillColor(color);
}

void Button::setTextStyle(sf::Uint32 style) {
    text.setStyle(style);
}

void Button::setCharacterSize(unsigned int size) {
    text.setCharacterSize(size);

    // Cập nhật lại vị trí text để vẫn canh giữa
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f sizeShape = shape.getSize();
    text.setPosition(pos.x + sizeShape.x / 2, pos.y + sizeShape.y / 2);
}

void Button::setSize(float width, float height) {
    shape.setSize({width, height});
    // Cập nhật lại vị trí chữ cho canh giữa
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2,
                   textBounds.top + textBounds.height / 2);
    sf::Vector2f pos = shape.getPosition();
    text.setPosition(pos.x + width / 2, pos.y + height / 2);
}

bool Button::isClicked(const sf::RenderWindow& window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    return shape.getGlobalBounds().contains(mousePosF);
}
