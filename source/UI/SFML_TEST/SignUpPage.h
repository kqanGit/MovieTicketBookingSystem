#pragma once
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "Button.h"
#include "TextBox.h"

class SignUpPage {
private:
    sf::Font font;

    sf::Text emailLabel;
    sf::Text usernameLabel;
    sf::Text passwordLabel;
    sf::Text confirmPasswordLabel;

    TextBox emailBox;
    TextBox usernameBox;
    TextBox passwordBox;
    TextBox confirmPasswordBox;

    Button registerButton;
    Button backButton;

    sf::Text title;

public:
    SignUpPage(sf::RenderWindow& window);

    AppState handleEvent(sf::Event& event, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
};
