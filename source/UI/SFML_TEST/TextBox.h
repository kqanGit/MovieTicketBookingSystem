#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class TextBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Font font;
    bool isSelected;
    bool isPassword;
    std::string inputString;

public:
    TextBox(float x = 0, float y = 0, float w = 0, float h = 0, bool isPassword = false);

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    void setSelected(bool selected);
    void setPosition(float x, float y);
    std::string getText() const;
    void clear();

    void setFont(const sf::Font& font);
    void setSize(float w, float h);
};

#endif // TEXTBOX_H
