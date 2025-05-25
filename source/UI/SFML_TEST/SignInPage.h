// SignInPage.h
#ifndef SIGNUPPAGE_H
#define SIGNUPPAGE_H

#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "TextBox.h"
#include "Button.h"

class SignInPage {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text titleText;

    TextBox accountBox;
    TextBox passwordBox;
    Button signInButton;
    Button backButton;

public:
    SignInPage(sf::RenderWindow& window);

    AppState handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw();
};


#endif // SIGNUPPAGE_H