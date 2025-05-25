// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <string>

// class Button {
// public:
//     sf::RectangleShape shape;
//     sf::Text text;

//     Button(const sf::Vector2f& size, const sf::Font& font, const std::string& label) {
//         shape.setSize(size);
//         shape.setFillColor(sf::Color(70, 130, 180)); // SteelBlue
//         shape.setOutlineColor(sf::Color::White);
//         shape.setOutlineThickness(2);

//         text.setFont(font);
//         text.setString(label);
//         text.setCharacterSize(24);
//         text.setFillColor(sf::Color::White);
//     }

//     void setPosition(float x, float y) {
//         shape.setPosition(x, y);
//         // căn giữa chữ trong button
//         sf::FloatRect textRect = text.getLocalBounds();
//         text.setPosition(
//             x + (shape.getSize().x - textRect.width) / 2 - textRect.left,
//             y + (shape.getSize().y - textRect.height) / 2 - textRect.top
//         );
//     }

//     bool isMouseOver(const sf::RenderWindow& window) const {
//         sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//         return shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
//     }
// };

// int main() {
//     sf::RenderWindow window(sf::VideoMode(1280, 720), "Movie Booking Ticket System");
//     window.setFramerateLimit(60);

//     // Load font
//     sf::Font font;
//     if (!font.loadFromFile("arial.ttf")) {
//         return -1;
//     }

//     // Load background texture
//     sf::Texture bgTexture;
//     if (!bgTexture.loadFromFile("background.jpg")) {
//         return -1;
//     }
//     sf::Sprite bgSprite(bgTexture);
//     bgSprite.setScale(
//         float(window.getSize().x) / bgTexture.getSize().x,
//         float(window.getSize().y) / bgTexture.getSize().y);

//     // Load logo texture
//     sf::Texture logoTexture;
//     if (!logoTexture.loadFromFile("logo.png")) {
//         return -1;
//     }
//     sf::Sprite logoSprite(logoTexture);
//     logoSprite.setPosition(
//         (window.getSize().x - logoTexture.getSize().x) / 2.f,
//         100.f);

//     // Welcome text
//     sf::Text welcomeText("Welcome back to Movie Booking Ticket System", font, 30);
//     welcomeText.setFillColor(sf::Color::White);
//     welcomeText.setPosition(
//         (window.getSize().x - welcomeText.getLocalBounds().width) / 2.f,
//         100.f + logoTexture.getSize().y + 20.f);

//     // Buttons
//     std::vector<Button> buttons;
//     buttons.emplace_back(sf::Vector2f(160, 50), font, "Sign In");
//     buttons.emplace_back(sf::Vector2f(160, 50), font, "Sign Up");
//     buttons.emplace_back(sf::Vector2f(160, 50), font, "Guest");
//     buttons.emplace_back(sf::Vector2f(160, 50), font, "Exit");

//     // Position buttons - nằm ngang, cách đều, căn giữa dưới cùng
//     float totalWidth = buttons.size() * 160 + (buttons.size() - 1) * 30; // 30 là khoảng cách
//     float startX = (window.getSize().x - totalWidth) / 2.f;
//     float posY = window.getSize().y - 120.f;

//     for (int i = 0; i < buttons.size(); ++i) {
//         buttons[i].setPosition(startX + i * (160 + 30), posY);
//     }

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if(event.type == sf::Event::Closed)
//                 window.close();

//             if(event.type == sf::Event::MouseButtonPressed) {
//                 for (size_t i = 0; i < buttons.size(); ++i) {
//                     if (buttons[i].isMouseOver(window)) {
//                         if (buttons[i].text.getString() == "Exit") {
//                             window.close();
//                         }
//                         else if (buttons[i].text.getString() == "Sign In") {
//                             // TODO: Chuyển sang màn hình Sign In
//                         }
//                         else if (buttons[i].text.getString() == "Sign Up") {
//                             // TODO: Chuyển sang màn hình Sign Up
//                         }
//                         else if (buttons[i].text.getString() == "Guest") {
//                             // TODO: Xử lý Guest
//                         }
//                     }
//                 }
//             }
//         }

//         window.clear();
//         window.draw(bgSprite);
//         window.draw(logoSprite);
//         window.draw(welcomeText);
//         for (auto& btn : buttons) {
//             // Nếu muốn, tô màu nổi bật khi hover chuột
//             if (btn.isMouseOver(window))
//                 btn.shape.setFillColor(sf::Color(100, 149, 237)); // LightSteelBlue
//             else
//                 btn.shape.setFillColor(sf::Color(70, 130, 180)); // SteelBlue
//             window.draw(btn.shape);
//             window.draw(btn.text);
//         }
//         window.display();
//     }

//     return 0;
// }
