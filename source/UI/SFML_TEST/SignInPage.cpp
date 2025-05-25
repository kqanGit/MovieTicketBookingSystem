#include "SignInPage.h"

SignInPage::SignInPage(sf::RenderWindow& window)
    : window(window),
      accountBox(300, 250, 300, 40),
      passwordBox(300, 320, 300, 40),
      signInButton("Sign In", font, 300, 400, 120, 40),
      backButton("Back", font, 440, 400, 120, 40)
{
    font.loadFromFile("./image/arial.ttf");

    titleText.setFont(font);
    titleText.setString("Sign In");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(350, 150);
}

AppState SignInPage::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    accountBox.handleEvent(event);
    passwordBox.handleEvent(event);

    if (signInButton.handleEvent(event, window)) {
        // TODO: check DB logic
        return AppState::MovieList;
    }

    if (backButton.handleEvent(event, window)) {
        return AppState::MainMenu;
    }

    return AppState::SignIn;
}

void SignInPage::draw() {
    window.clear();
    window.draw(titleText);
    accountBox.draw(window);
    passwordBox.draw(window);
    signInButton.draw(window);
    backButton.draw(window);
    window.display();
}
