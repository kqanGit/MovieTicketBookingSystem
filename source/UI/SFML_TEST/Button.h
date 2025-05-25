#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include "RoundedRectangleShape.h"

class Button {
private:
    RoundedRectangleShape shape;
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;

public:
    Button(const std::string& label, const sf::Font& font,
           float x, float y, float width, float height);

    void draw(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void setFillColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);
    void setTextStyle(sf::Uint32 style);

    void setCharacterSize(unsigned int size);
    void setSize(float width, float height);

    bool isClicked(const sf::RenderWindow& window) const;

};

#endif // BUTTON_H
