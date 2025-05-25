#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow& window) : window(window) {
    backgroundTexture.loadFromFile("./image/background@.png");

    backgroundSprite.setTexture(backgroundTexture);

    // Scale ảnh để vừa với kích thước cửa sổ
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    backgroundSprite.setScale(scaleX, scaleY);

    font.loadFromFile("./image/arial.ttf");

    titleText.setFont(font);
    //titleText.setString("Welcome back to Movie Booking Ticket System");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);

    // Canh giữa tiêu đề theo chiều ngang
    sf::FloatRect textBounds = titleText.getLocalBounds();
    titleText.setPosition((windowSize.x - textBounds.width) / 2, 350);

    float x = 200.0; // vị trí nút bắt đầu theo chiều ngang
    float y = 500.0; // vị trí nút theo chiều dọc
    float w = 200.0; //150
    float h = 50.0; //chiều cao nút
    float gap = 30.0; // khoang cách giữa các nút

    buttons.emplace_back("Sign In", font, x, y, w, h);
    buttons.emplace_back("Sign Up", font, x + w + gap, y, w, h);
    buttons.emplace_back("Guest",  font, x + 2*(w + gap), y, w, h);
    buttons.emplace_back("Exit",   font, x + 3*(w + gap), y, w, h);

    for (auto& btn : buttons) {
        btn.setFillColor(sf::Color::White);           // Màu nền (cornflower blue)
        btn.setTextColor(sf::Color(102, 51, 153));                   // Màu chữ
        btn.setTextStyle(sf::Text::Bold);                     // In đậm
    }

}

AppState MainMenu::handleEvent(sf::Event& event) {
    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons[i].handleEvent(event, window)) {
            switch (i) {
                case 0: return AppState::SignIn;
                case 1: return AppState::SignUp;
                case 2: return AppState::Guest;
                case 3: return AppState::Exit;
            }
        }
    }
    return AppState::MainMenu;
}

void MainMenu::draw() {
    window.draw(backgroundSprite);
    window.draw(titleText);
    for (auto& button : buttons)
        button.draw(window);
}
