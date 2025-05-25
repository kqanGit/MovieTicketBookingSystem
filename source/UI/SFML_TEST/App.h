#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "MainMenu.h"
#include "SignInPage.h"
#include "SignUpPage.h"

class App {
public:
    App();
    void run();

private:
    sf::RenderWindow window;
    AppState currentState;

    MainMenu mainMenu;
    SignInPage signInPage;
    SignUpPage signUpPage;

    void processEvents();
    void update();
    void render();
};

#endif // APP_H