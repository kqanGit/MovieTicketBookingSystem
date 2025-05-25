#include "App.h"

App::App()
    : window(sf::VideoMode(1280, 720), "Movie Booking Ticket System")
    , mainMenu(window)
    , signInPage(window)
    , signUpPage(window)
    , currentState(AppState::MainMenu) {}

void App::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void App::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        switch (currentState) {
            case AppState::MainMenu:
                currentState = mainMenu.handleEvent(event);
                break;
            case AppState::SignIn:
                currentState = signInPage.handleEvent(event, window);
                break;
            case AppState::SignUp:
                currentState = signUpPage.handleEvent(event, window);
                break;
            case AppState::Exit:
                window.close();
                break;
            default:
                break;
        }
    }
}

void App::update() {
    // Logic update nếu cần
}

void App::render() {
    window.clear();

    switch (currentState) {
        case AppState::MainMenu:
            mainMenu.draw();
            break;
        case AppState::SignIn:
            signInPage.draw();
            break;
        case AppState::SignUp:
            signUpPage.render(window);
            break;
        default:
            break;
    }

    window.display();
}
