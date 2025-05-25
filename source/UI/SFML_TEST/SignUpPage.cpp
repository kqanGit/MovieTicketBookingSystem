#include "SignUpPage.h"
#include <vector>

SignUpPage::SignUpPage(sf::RenderWindow& window)
    : emailBox(300.f, 100.f, 300.f, 40.f, false),
      usernameBox(300.f, 180.f, 300.f, 40.f, false),
      passwordBox(300.f, 260.f, 300.f, 40.f, true),
      confirmPasswordBox(300.f, 340.f, 300.f, 40.f, true),
      registerButton("Register", font, 500, 600, 300, 60), // x, y, width, height

      backButton("Back", font, 0, 0, 500, 45)
{
    if (!font.loadFromFile("./image/arial.ttf")) {
        // Xử lý lỗi font không load được
    }

    float width = window.getSize().x;
    float height = window.getSize().y;

    // Set lại font cho các TextBox
    emailBox.setFont(font);
    usernameBox.setFont(font);
    passwordBox.setFont(font);
    confirmPasswordBox.setFont(font);

    std::vector<std::string> labelStrings = {
        "Email", "Username", "Password", "Confirm Password"
    };

    std::vector<sf::Text*> labelTargets = {
        &emailLabel, &usernameLabel, &passwordLabel, &confirmPasswordLabel
    };
    std::vector<TextBox*> boxTargets = {
        &emailBox, &usernameBox, &passwordBox, &confirmPasswordBox
    };

    float startY = 100;
    float stepY = 80;
    float labelX = 100;
    float boxX = 500;
    float boxWidth = 300;

    for (int i = 0; i < labelStrings.size(); ++i) {
        labelTargets[i]->setFont(font);
        labelTargets[i]->setString(labelStrings[i]);
        labelTargets[i]->setCharacterSize(24);
        labelTargets[i]->setFillColor(sf::Color::White);
        labelTargets[i]->setPosition(labelX, startY + i * stepY);

        boxTargets[i]->setPosition(boxX, startY + i * stepY);
        boxTargets[i]->setSize(boxWidth, 40);
    }

    registerButton = Button("Register", font, width / 2 - 140, startY + labelStrings.size() * stepY + 30, 120, 45);
    backButton = Button("Back", font, width / 2 + 20, startY + labelStrings.size() * stepY + 30, 120, 45);
}

void SignUpPage::render(sf::RenderWindow& window) {
    window.draw(emailLabel);
    window.draw(usernameLabel);
    window.draw(passwordLabel);
    window.draw(confirmPasswordLabel);

    emailBox.draw(window);
    usernameBox.draw(window);
    passwordBox.draw(window);
    confirmPasswordBox.draw(window);

    registerButton.draw(window);
    backButton.draw(window);
}

AppState SignUpPage::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (registerButton.isClicked(window)) {
            // TODO: xử lý đăng ký tài khoản
        }
        if (backButton.isClicked(window)) {
            return AppState::MainMenu;
        }
    }

    emailBox.handleEvent(event);
    usernameBox.handleEvent(event);
    passwordBox.handleEvent(event);
    confirmPasswordBox.handleEvent(event);

    return AppState::SignUp;
}
