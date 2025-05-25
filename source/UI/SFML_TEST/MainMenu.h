#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "Button.h"
#include <vector>

class MainMenu {
public:
    MainMenu(sf::RenderWindow& window);
    AppState handleEvent(sf::Event& event);
    void draw();

private:
    sf::RenderWindow& window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Font font;
    sf::Text titleText;
    std::vector<Button> buttons;
};

#endif // MAINMENU_H